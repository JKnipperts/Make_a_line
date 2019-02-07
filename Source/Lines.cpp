/**********************************************
 *	Titel   : Make a Line!            *
 *	Autor	: Jan Knipperts  	  *
 *	Version : 1.3	- 27.12.2005      *
**********************************************/


#include <windows.h>
#include <ddraw.h>
#include <dsound.h>
#include <mmsystem.h>
#include <sstream>
#include <iostream>
#include "ddutil.h"
#include <commctrl.h>
#include "resource.h"
#include <math.h>
#include <time.h>

using namespace std;  

void ClearGame();


struct _posi
{
	int		x;
	int		y;
};

struct _ENTRY
{
	char		Name[20];
	long		Score;
};

//Window:
HINSTANCE			hInstance;							
HWND				hwnd;
MSG				messages;         
WNDCLASSEX			wincl; 

//DirectDraw
LPDIRECTDRAW7			lpDirectDraw;
HRESULT				ddrval;
LPDIRECTDRAWSURFACE7		lpddsPrimary;
LPDIRECTDRAWSURFACE7		lpddsBack;
LPDIRECTDRAWSURFACE7		lpddsBitmap;
LPDIRECTDRAWSURFACE7		lpddsBitmap1;
DDSURFACEDESC2			ddsd;
DDSCAPS2			ddscaps; 
DDBLTFX				ddbltfx;
RECT				fill_area,rect;
LPDIRECTDRAWGAMMACONTROL 	lpDDGammaControl = NULL; 
DDGAMMARAMP			GammaRamp; 
DDGAMMARAMP			OriginalRamp;
unsigned char			gammacent[255][3]; 

RECT				rectbitmap;
LPDIRECTDRAWCLIPPER		lpddClipper;
HDC				hdc;
static HBITMAP			hBitmap ;
static BITMAP			bitmap ;
static HBITMAP			invBitmap;
static BITMAP			bm;
HFONT				Font;
HPEN				Pen;
COLORREF			FontColor;
DDDEVICEIDENTIFIER2     	DriverInfo;


//DirectSound
LPDIRECTSOUND			lpDirectSound;
LPDIRECTSOUNDBUFFER		dsbSound;

//Highscore				
HANDLE				file;
_ENTRY				eintrag[10];
_ENTRY				winner;
					

//Gameplay
int				x,y,z,mx,my,tx,ty,tx1,ty1,dc,dropnum;
bool				startpos,marked,movemode,movefinished,d,active;
POINT				maus;
int				game[9][9]; 
_posi				startP,ziel,mark;
int				col,stepX,stepY;
long				punkte,highscore,rekord;
int				colors[9];
_posi				freef[81];
int				maxfree,size,ll;
bool				gameover,SOUND, cheat,debug,s,dropresult,onend;
string				errormsg,rname;

//Multithreading
HANDLE				hThread; 
DWORD				dwThreadID; 
LPVOID				Data;
HWND				thwnd;


//Konstanten:

const LPTSTR			CLICK  = "sound\\Laser.wav";
const LPTSTR			DROP   = "sound\\blub.wav";
const LPTSTR			CLEAR  = "sound\\clear.wav";
const LPTSTR			CLEAR2 = "sound\\clear2.wav";
const LPTSTR			NO     = "sound\\no.wav";

LPTSTR				title = "Make a Line!";
LPTSTR				ver = "1.3";
LPTSTR			   	verdate = "27.12.2005";
LPTSTR				cpr = "Copyright (c) 2005 by Jan Knipperts";

int				ballspeed = 8; //Geschwindigkeit (in Pixeln) der Kugel beim "wandern"
int				fadespeed = 4; //Dauer des "Fadeeffekts" in Millisekunden
int				colormode = 16; //Farbtiefe in Bit

const int 			linefull = 5;  //How many stone must be aligned to complete a line?
const int 			dropval  = 3;  //How many new stones apear when a line was completed?

const int 			senkrecht  = 1; //The directions
const int 			waagerecht = 2;
const int 			diagonalR  = 3;
const int 			diagonalL  = 4;



//========================== Keyboard control

bool KeyDown (int nVirtKey)
{
	if ((GetAsyncKeyState(nVirtKey) & 0x8000) != 0)
		return true;
	else
		return false;
}


bool KeyHit (int nVirtKey)
{
	static int keyPressed;
	if ((GetAsyncKeyState(nVirtKey) & 0x8000) != 0) //if key is down
	{
		keyPressed = nVirtKey;
		return false;
	}
	else if (keyPressed == nVirtKey) //if key is released after being pressed
	{
		keyPressed = 0;
		return true;
	}
	return false;
}




//=========================== STRING CONVERSION


string tostring (WORD d ) // Word to string conversion
{
	string s;
	stringstream str;
	str << d;
	str >> s;
	return s;
}

string GetStr(string b, int ptr,int slen) //Gets a part of a sting
{
  int	i;
  string s;

  ZeroMemory(&s,sizeof(s));

    if ((ptr+slen) <= strlen(b.data()))
	{
      for (i = 0; i < slen; i++)
	  {
		s += b.data()[ptr];
        ptr = ptr + 1 ;
	  }


	}

	return s;

}


#include "pathfinder.h"
#include "Grafik.h"
#include "sound.h"
#include "Game.h"



void Restore()
{
	  if (lpddsBitmap != NULL)
	  {
        lpddsBitmap -> Release();
        lpddsBitmap = NULL;
	  }
	lpddsPrimary->Restore();
	lpddsBack->Restore();
	ShowCursor(false);			   	
	SetCursor(LoadCursor (hInstance,MAKEINTRESOURCE(IDC_CURSOR)));
	ShowCursor(true);			 
	InitBitmaps();
}


void ClearGame()
{
	marked = FALSE;
	gameover = FALSE;	
	movefinished = TRUE;
	startpos = TRUE;
	d = TRUE;			
	ZeroMemory(&game,sizeof(game));  //Clear the playfield
	ZeroMemory(&walkability,sizeof(walkability));			
	ZeroMemory(&freef,sizeof(freef));	
	ZeroMemory(&startP,sizeof(startP));	
	ziel.x = 100;
	ziel.y = 100;
	Draw();						
	ReadHighscore();
	punkte = 0; //Reset SCORE
	maxfree = 0;
	ll = 0;				
}






//============================ Main routines =====================================



// Processing messages to the main window :

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

				
     switch (message)
     {
	case WM_CREATE:	
         	hInstance = ((LPCREATESTRUCT) lParam)->hInstance ;	
	 return 0 ;
	     
	case WM_PAINT:       		 
	 	if (startpos == TRUE)
	  	{					 
			startpos = FALSE;
	  	}

	  	 if (active == TRUE)
		 {
	     		if (gameover == FALSE) 
			{
				Draw(); //Let's draw to the backbuffer first				     
									
				if ((movemode == TRUE) && (d == FALSE))
				{
					  MoveSphere();
					  DrawSphere(tx,ty,col);						  						
				}								
			}
		 
			
			while (lpddsPrimary->Flip(NULL, DDFLIP_WAIT) != DD_OK)
			{
				if(FAILED(lpddsPrimary->Flip(NULL,DDFLIP_WAIT)))
				{
	 				Restore(); //Restore contents if the main windows gets restored 
				}
			}
	 
		}	   
          return 0 ;

	
	case WM_SYSCOMMAND: //Do we have to handle a systen command?
    	{ 
      		switch (wParam)  
      		{ 
        		case SC_SCREENSAVE: 
        		// Windows wants to start the screensaver... 

        		case SC_MONITORPOWER: 
        		// Windows wants the monitor to go in suspend mode...
        
        		return 0;  // We don't allow that and return a zero... 
      		} 
      		break;  
    	}


	case WM_CLOSE:				//Quit program?
        	PostQuitMessage(0);
	return 0;

	case WM_LBUTTONDOWN: 	  //Left mouse button pressed?
	 	GetCursorPos(&maus);
		move();
	return 0;

	 case WM_RBUTTONDOWN: 	  //Right mouse button pressed?
	 	if (debug == TRUE)
		{
			ll = 0;
			Draw();
		 }
	   return 0;
	
	 case WM_ACTIVATEAPP: //Our window was minimized...
		 if (wParam == 0) 
		 {
		  active = FALSE;
		 }
		 else
		 {
		  active = TRUE;
		 }
	return 0;


	case WM_KEYDOWN :
      		switch(wParam) 		 //Handle keypresses
		{	

		   case VK_ESCAPE:  //ESC brings us back to the main menu
		   {	
			    if (QuitQuestion() == TRUE)
				{
			      DD_Fadedown(10);
		          Main();
				  break;
				}
				else
				{
				 DrawGame();	
				 break;
				}
				break;

		   }

		   case VK_SPACE:  //Space shows the highscore table
		   {				 	 
			    DD_Fadedown(fadespeed);
		        ShowHighscore();
				DrawBackground();
				DrawGame();	
				SetGamma(0,0,0);
				lpddsPrimary -> Blt(&rect, lpddsBack, NULL, DDBLT_WAIT, NULL);	          
				DD_Fadeup(fadespeed);
				break;
		   }

		    case VK_F9:  // F9 = Instant exit from program. (Bosskey)
			   {
				DD_Fadedown(fadespeed/2);
			    ReleaseDSound ();
				DD_Cleanup();		
				PostQuitMessage (0) ;
				break;
			   }

		
		   case VK_F3:  // F3 = Restart game
			   {
				DD_Fadedown(fadespeed/10);			   
		        ClearGame();					
				Drop(linefull);
				lpddsBack->Restore(); //Make sure the surface is restored	
				lpddsPrimary -> Blt(&rect, lpddsBack, NULL, DDBLT_WAIT, NULL);
				Draw();			
				DD_Fadeup(fadespeed/2);	
				break;
			   }


		   case VK_F2:  // F2 = Sound on/off
		   {		
			    if (SOUND == TRUE)
				{
					SOUND = FALSE;
				}
				else
				{
					SOUND = TRUE;
				}

			    Draw();
				break;
		   }
		   
		   case VK_F1:  //F1 = Help
		   {				 	 
		      	ShowHelp();
		
				break;
		   }

		    case 'J':			// J activates cheat mode
			{
				if (cheat == TRUE)
				{
					cheat = FALSE;					
				}
				else
				{
					cheat = TRUE;
				}
				break;
			}


		

			case 'A':		//When cheat mode is activated you can press A to get 10 additional scores
			{
				if (cheat == TRUE)
				{
					punkte = punkte + 10;
				}
				break;
			}

			case 'N':		//...and N for starting again without loosing your scores
			{
				if (cheat == TRUE)
				{			
				ClearGame();					
				lpddsBack->Restore(); //Make sure the surface is restored	
				lpddsPrimary -> Blt(&rect, lpddsBack, NULL, DDBLT_WAIT, NULL);
				Draw();			
				}
			
				break;
			}

		
		    	  


		    case 'V': //CTRL+V = Show game version
			{
				if (HIBYTE(GetAsyncKeyState(VK_CONTROL)) > 1) 
				{
					ShowVersionInfo();					
				}
				break;
			}

			case 'D': //CTRL+D activates debug mode
			{
				if (HIBYTE(GetAsyncKeyState(VK_CONTROL)) > 1) 
				{
				  if (debug == FALSE)
				  {
				      //ShowDebugInfo();									
					  debug = TRUE;
				  }
				  else
				  {
					  debug = FALSE;
				  }
				
				}
				
				
				break;
			}	 
		}
	  return 0;

	


     case WM_DESTROY:			
		 ReleaseDSound ();
		 DD_Cleanup();		
		 PostQuitMessage (0) ;
  	     return 0 ;
	     
          
     }
     return DefWindowProc (hwnd, message, wParam, lParam);
}







int WINAPI  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                                        LPSTR d3, int nCmdShow)
{


	//First check if all necessary files can be found


	if (CheckFile(CLICK) == FALSE)
	{
		errormsg = "Kann die Datei '";
		errormsg += CLICK;
		errormsg += "' nicht finden!";
		MessageBox(hwnd,errormsg.data(), 
                   "ERROR: ", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	if (CheckFile(DROP) == FALSE)
	{
		errormsg = "Kann die Datei '";
		errormsg += DROP;
		errormsg += "' nicht finden!";
		MessageBox(hwnd,errormsg.data(), 
                   "ERROR: ", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}
	if (CheckFile(CLEAR) == FALSE)
	{
		errormsg = "Kann die Datei '";
		errormsg += CLEAR;
		errormsg += "' nicht finden!";
		MessageBox(hwnd,errormsg.data(), 
                   "ERROR: ", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}	
	if (CheckFile(CLEAR2) == FALSE)
	{
		errormsg = "Kann die Datei '";
		errormsg += CLEAR2;
		errormsg += "' nicht finden!";
		MessageBox(hwnd,errormsg.data(), 
                   "ERROR: ", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}
	if (CheckFile(NO) == FALSE)
	{
		errormsg = "Kann die Datei '";
		errormsg += NO;
		errormsg += "' nicht finden!";
		MessageBox(hwnd,errormsg.data(), 
                   "ERROR: ", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}



	if  (ReadHighscore() != TRUE) //Error reading the highscore table?
	{
		 errormsg = "Kann die Highscore-Datei 'Scores.dat' nicht finden!" ;
         errormsg += (char) 13;
		 errormsg += " Es wird eine neue (leere) Highscoreliste erstellt! ",              
             
		MessageBox(hwnd,errormsg.data(), 
               "WARNUNG: ", MB_OK | MB_ICONWARNING);
		
		if (CreateNewHighscoretable() == FALSE)			// if yes, we use a new and empty list 
		{
		  MessageBox(hwnd, 
               "Kann Datei 'Scores.dat' nicht neu anlegen !", //Can't create new file!
                "DATEIFEHLER! ", MB_OK | MB_ICONEXCLAMATION);
		  return -1;
		}
		else
		{
		    punkte = 0; //Eine neue Highscoreliste bringt die werte manchmal durcheinander...
	        highscore = 0;
			rekord = 0;
			ReadHighscore();
		}
	}










   //Create our window class

    wincl.hInstance = hInstance;
    wincl.lpszClassName = "MeineFensterklasse";
    wincl.lpfnWndProc = WndProc;      
    wincl.style = CS_DBLCLKS;                 
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon (NULL, IDI_WINLOGO);
    wincl.hIconSm = LoadIcon (NULL, IDI_WINLOGO);
    wincl.hCursor = NULL;
    wincl.lpszMenuName = NULL;                
    wincl.cbClsExtra = 0;                      
    wincl.cbWndExtra = 0;
 	wincl.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    
    
    if (!RegisterClassEx (&wincl))
        return 0;



	//Create our window



	  hwnd = CreateWindowEx (0, 
		                   "MeineFensterklasse",
				   title, 
		                   WS_POPUP, 
				   CW_USEDEFAULT,
		                   CW_USEDEFAULT,
				   GetSystemMetrics(SM_CXSCREEN), //X-Auflösung
				   GetSystemMetrics(SM_CYSCREEN), //Y-Auflösung
				   HWND_DESKTOP,
		                   0,
				   hInstance,
				   0);

   
	//Initialize DirectSound
	  if (DSoundInit(hwnd) != TRUE)
	  {
	  	errormsg += "Kann DirectSound nicht initialisieren! ";
		errormsg += (char) 13;
            	errormsg += "Stellen Sie sicher, dass auf dem Computer mindestens ";
            	errormsg += (char) 13;
            	errormsg += "DirectX 7.0 installiert ist.", 
               
		MessageBox(hwnd,errormsg.data(),
             	"ERROR: ", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	  

 
 
	//Set fullscreen mode    
	//Initialize DirectDraw and handle DD error codes...

	if(DD_InitDirectDraw() != 0)
	{      
		lpDirectDraw->RestoreDisplayMode();
	    	errormsg += "Kann DirectDraw nicht initialisieren! "; 
            	errormsg += (char) 13;
            	errormsg += "Stellen Sie sicher, dass auf dem Computer mindestens ";
            	errormsg += (char) 13;
	    	errormsg += "DirectX 7.0 installiert ist.", 
            
      		MessageBox(hwnd,errormsg.data(),
             	"ERROR: ", MB_OK | MB_ICONEXCLAMATION);
       		DD_Cleanup();
       		return -1;
	}

	if (DD_SetVideoMode(800,600,colormode) != 0)			
	{
		lpDirectDraw->RestoreDisplayMode();
	 	DD_Cleanup();
		errormsg = "Kann den Grafikmodus 800x600 mit"; 
	      	errormsg += tostring(colormode);
		errormsg += " Bit Farbtiefe nicht initialisieren!",     
      		MessageBox(hwnd,errormsg.data(),"ERROR: ", MB_OK | MB_ICONERROR);	 
      		return -1;
	}

	
	if(DD_InitSurfaces() != 0)
	{      
	     	lpDirectDraw->RestoreDisplayMode();
		errormsg += "Beim Initialisieren der DirectDraw Surfaces ist ein ";             
            	errormsg += (char) 13;
		errormsg += "unerwarteter DirectX Fehler aufgetreten! ";
            	errormsg += (char) 13;
			
      		MessageBox(hwnd,errormsg.data(),               
               "ERROR: ", MB_OK | MB_ICONERROR);
	  	DD_Cleanup();
      		return -1;
	}


	if (DD_InitPaintProcs() != 0)
	{
		lpDirectDraw->RestoreDisplayMode();
		MessageBox(hwnd,                 
                "Ein unerwarteter DirectX Fehler ist aufgetreten! ",
		"ERROR: ", MB_OK | MB_ICONERROR);
	  	DD_Cleanup();
      		return -1;
	}

  
	if (InitBitmaps() != 0)
	{
		lpDirectDraw->RestoreDisplayMode(); 
		MessageBox(hwnd,                 
                "Fataler Fehler beim Laden der Ressourcen! ",
		"ERROR: ", MB_OK | MB_ICONERROR);
      		return -1;
	}


	ShowWindow (hwnd, nCmdShow); //Show window
	if (! hwnd) return -1;
	ShowOwnedPopups(hwnd,TRUE);
	ShowCursor(false);			 //Disable mouse cursor...
    	SetCursor(LoadCursor (hInstance,MAKEINTRESOURCE(IDC_CURSOR))); //...and set our own cursor...
	ShowCursor(true);			 //..and enable mouse cursor again

	startpos = TRUE;
	SOUND = TRUE;
    	punkte = 0; //start games with zero points
    
   	InitializePathfinder (); //Initialize A* pathfinder
   	GetWindowRect(hwnd,&rect);				 	 
   	DD_AddColorKey (lpddsBack,0,0);
   	DD_AddColorKey (lpddsPrimary,0,0);
   
    	stepX = (800 / 9);
	stepY = (524 / 9);
	tileSizeX = stepX;
	tileSizeY = stepY;
    
   	SetCursorPos((rect.right / 2),(rect.bottom / 2));
   	
	Main();
  
		
    	while (1)		//Handle messages 
    	{ 		

		if (PeekMessage(&messages,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (messages.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				break;						// If So quit
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&messages);				// Translate The Message
				DispatchMessage(&messages);				// Dispatch The Message
			}
		}
    	}   
		

 	 EndPathfinder(); //End A*
 	 
	return messages.wParam;
}
