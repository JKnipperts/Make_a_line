//============================================================================================
//			       Gameplay routines
//============================================================================================



void DrawGame();
void GameOver();
void Feld2Cords(_posi feld, int &XP, int &YP);
bool ShowHighscore();
bool Main();
bool DetectLine(_posi last,bool POINTS);



void Draw()  //Redraws the screen
{
	int zx,zy;
    lpddsBack->Restore(); //Make sure the surface is restored		
	

	 Cls(lpddsBack);
  	 DrawBackground();
	 DrawGame();    

	if (marked == TRUE)
	{
 	 Feld2Cords(mark,zx,zy);
	 Kreis(zx,zy,30,RGB(255,0,0));
	}

}



void WaitMouse()
//Waits until  a mouse button has been pressed
{
	while ((HIBYTE(GetAsyncKeyState(VK_LBUTTON)) <= 1) && 
		   (HIBYTE(GetAsyncKeyState(VK_RBUTTON)) <= 1))
	{
	}
}


void ShowVersionInfo(void)
{
 
	Cls(lpddsPrimary);
    lpddsPrimary->Restore(); //Make sure the surface is restored	
	lpddsPrimary->GetDC(&hdc);//Get the DC of the surface


	SetFontColor(255,255,255);
	SetPen(255,0,0,1);	
	Rect(100,100,600,401);
	SetFont("Arial",16,1,0,1);
	Text(125,130,title);
	SetFont("Arial",16);
	Text(125,150,cpr);
	Text(125,200,"Programmversion: ");
	Text(125,220,ver);
	Text(125,250,"Veröffentlicht am:");
	Text(125,270,verdate);
	Text(125,310,"Drücke eine Maustaste um zum Spiel zurückzukehren....");
	SetFont("Arial",14);
	GDI_Cleanup();//Get the DC of the surface


	WaitMouse();	
}


/* Shows Debug information 

void ShowDebugInfo(void)
{
	string ver;

    lpddsPrimary->Restore(); //Make sure the surface is restored	
	
	Cls(lpddsPrimary);
	lpddsPrimary->GetDC(&hdc);//Get the DC of the surface


	SetFontColor(255,255,255);
	SetPen(255,0,0,1);	
	Rect(100,100,600,401);
	SetFont("Arial",16,1,0,1);
	Text(125,130,title);
	SetFont("Arial",16);
	Text(125,150,"Debug - Info:");
	Text(125,170,"Boolean:");
	Text(225,170,"Hardware:");
	Text(125,200,"GameOver: ");
	Text(125,220,tostring(gameover).data());
	Text(125,250,"Move:");
	Text(125,270,tostring(movemode).data());
	Text(125,300,"Marked:");
	Text(125,320,tostring(marked).data());
	

	Text(225,200,"Verfügbarer RAM (in KByte): ");
	Text(225,220,tostring(AvailSysMem()).data());
	Text(225,250,"Verfügbarer Video RAM (in KByte):");
	Text(225,270,tostring(AvailVidMem()).data());
	Text(225,300,"CPU:");
	Text(225,320,GetCPUName().data());

	Text(455,200,"Grafiktreiber: ");
	lpDirectDraw->GetDeviceIdentifier(&DriverInfo,NULL);
	Text(455,220,DriverInfo.szDriver);
	Text(455,250,"Grafikkarten-ID:");
	Text(455,270,GetAdapterName().data());



	

	
	Text(125,360,"Drücke eine Maustaste um zum Spiel zurückzukehren....");

	SetFont("Arial",14);
	GDI_Cleanup();//Get the DC of the surface


	WaitMouse();		
}
*/


void ShowHelp(void)
{
    lpddsBack->Restore(); //Make sure the surface is restored	
	
    Cls(lpddsBack);
	DrawBitmap(0,0,MAKEINTRESOURCE(IDB_BITMAP5));
	lpddsBack->GetDC(&hdc);//Get the DC of the surface

	SetFontColor(255,255,255);
	SetFont("Comic Sans MS",36,1,0,1);
	Text(300,20,"Kurzanleitung :");
	SetFont("Arial",16,1,0,1);
	Text(150,130,"Anleitung:");
	SetFont("Comic Sans MS",16);
	Text(150,150,"Versuchen Sie fünf oder mehr gleichfarbige Kugeln in eine Reihe zu bekommen. ");
	Text(150,170,"Senkrechte, waagerechte und auch diagonale Reihen werden gezählt. ");
    Text(150,190,"Für jede Reihe gibt es zwei Punkte pro beteiligter Kugel. ");
    Text(150,210,"Die Kugeln bewegen Sie mit der Maus. Klicken Sie zunächst auf die Kugel");
    Text(150,230,"und dann auf das Feld zu dem sich die Kugel bewegen soll. ");
    Text(150,250,"Achten Sie aber darauf, dass der Weg dorthin frei ist....  ");
	Text(150,270,"Und mit jeder Bewegung die nicht zum Erfolg führte, tauchen drei weitere Kugeln auf...");
    Text(150,290,"Das Spiel endet, wenn keine weitere Bewegung mehr möglich ist.");
	SetFont("Arial",16,1,0,1);
	Text(180,350,"Tasten:");
	SetFont("Arial",16);
    Text(300,370,"F1 = Diese Hilfe           ");
	Text(300,390,"F2 = Sound an / aus        ");
	Text(300,410,"F3 = Spiel neu starten     ");
	Text(300,430,"F9 = Spiel schnell beenden");
    Text(280,450,"ESC = Zurück zum Hauptmenü ");
	Text(240,470,"LEERTASTE = Highscoreliste ansehen");
	Text(274,490,"Strg+V = Versionsinfo          ");

	Text(1,580,"Drücke eine Maustaste um zum Spiel zurückzukehren....");
	SetFont("Arial",14);

	GDI_Cleanup();//Get the DC of the surface
	Cls(lpddsPrimary);
	lpddsPrimary -> Blt(&rect, lpddsBack, NULL, DDBLT_WAIT, NULL);
	while(lpddsPrimary->Flip(NULL, DDFLIP_WAIT) != DD_OK);

	
	WaitMouse();	
	
}

void ShowInfo(void)
{	
	SetGamma(0,0,0);
    lpddsBack->Restore(); //Make sure the surface is restored		
	Cls(lpddsBack);
	DrawBitmap(0,0,MAKEINTRESOURCE(IDB_BITMAP4));
	lpddsBack->GetDC(&hdc);//Get the DC of the surface
	SetFontColor(255,255,255);
	SetFont("Arial",14);
	Text(1,580,"Drücke eine Maustaste....");
	GDI_Cleanup();//Get the DC of the surface
	Cls(lpddsPrimary);
	SetGamma(0,0,0);
	lpddsPrimary -> Blt(&rect, lpddsBack, NULL, DDBLT_WAIT, NULL);
	DD_Fadeup(10);	  
	while(lpddsPrimary->Flip(NULL, DDFLIP_WAIT) != DD_OK);


	WaitMouse();	
	DD_Fadedown(fadespeed);	  
	SetGamma(0,0,0);	
}


bool CreateNewHighscoretable()
{
	int z;
	unsigned long dwWritten;
	
	file = CreateFile("Scores.dat", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,0);
	if (file == INVALID_HANDLE_VALUE)  //Error creating file for Highscore table?
	{
	  return FALSE;
	}
	else
	{	
		ZeroMemory(&eintrag,sizeof(eintrag));

		for (z = 0; z <= 9; z++)
		{
			strcpy(eintrag[z].Name,"NOBODY");
			eintrag[z].Score = z;
		}

		WriteFile(file, &eintrag, sizeof(eintrag), &dwWritten, NULL);
		if (dwWritten != sizeof(eintrag))
		 {
			CloseHandle(file);
		   return FALSE;
		 }

		CloseHandle(file);
		 

		 
	}

	return TRUE;
}



void SortHighscore()
{
char  name[20];
		//Sort highscore entries (Bubblesort)

int iSwapCount = 0;

for(int i = 0; i < 9; i++) 
{
  for(int j = 0; j < (10 - (i + 1)); j++) 
  {
    if(eintrag[j].Score < eintrag[j + 1].Score) 
	{
      
	  int iTemp = eintrag[j].Score;
	  strcpy(name,eintrag[j].Name);


      strcpy(eintrag[j].Name,eintrag[j + 1].Name);
	  eintrag[j].Score = eintrag[j + 1].Score;

      eintrag[j + 1].Score = iTemp;
	  strcpy(eintrag[j + 1].Name,name);


      iSwapCount++;
    }
  }
  if(iSwapCount == 0) {
    break;
  }else {
    iSwapCount = 0;
  }
}
}


bool ReadHighscore()
{
	unsigned long dwread;
	
	
	file = CreateFile("Scores.dat", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,0);
	
	if (file == INVALID_HANDLE_VALUE) 
	{
	  return FALSE;
	}
	else
	{			
	  ReadFile(file,eintrag,sizeof(eintrag), &dwread, NULL);

	   CloseHandle(file);
	  if (dwread != sizeof(eintrag))
	  {
		return FALSE;
	  }

	
	}


   SortHighscore();

   rekord = eintrag[0].Score;
   rname = eintrag[0].Name;
   highscore = eintrag[9].Score;

 return TRUE;
}


bool ShowHighscore()
{
	int z,y;

	ReadHighscore();
	Cls(lpddsBack);
	SetGamma(0,0,0);
	DrawBitmap(0,0,MAKEINTRESOURCE(IDB_BITMAP1));
	lpddsBack->GetDC(&hdc);//Get the DC of the surface
	SetFontColor(255,255,255);
	SetPen(255,0,0,1);	
	SetFont("Arial",36,1,0,1);
	Text(300,20,"Die Besten :");
	y = 130;
	SetFont("Arial",16,0,0,0);
 for (z = 0; z <= 9; z++)
 {
	Text(150,y,eintrag[z].Name);
	Text(550,y,tostring(eintrag[z].Score).data());
	y = y + 30;
 }
    SetFontColor(0,0,255);
 	Text(320,565,"Drücke eine Maustaste....");
	SetFont("Arial",14);
	GDI_Cleanup();//Get the DC of the surface
	Cls(lpddsPrimary);
	SetGamma(0,0,0);
	lpddsPrimary -> Blt(&rect, lpddsBack, NULL, DDBLT_WAIT, NULL);
	         while(lpddsPrimary->Flip(NULL, DDFLIP_WAIT) != DD_OK);
	DD_Fadeup(fadespeed);


    WaitMouse();

	DD_Fadedown(fadespeed);

	
	

	return TRUE;
}



bool WriteHighscore()
{ 
 unsigned long dwWritten;

	if (ReadHighscore() != TRUE)
	{
		return FALSE;
	}

	 SortHighscore();
	 eintrag[9] = winner;
	 highscore = winner.Score;
	 SortHighscore();

	file = CreateFile("Scores.dat", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,0);
	if (file == INVALID_HANDLE_VALUE) 
	{
	  return FALSE;
	}
	else
	{	
		WriteFile(file, &eintrag, sizeof(eintrag), &dwWritten, NULL);
		if (dwWritten != sizeof(eintrag))
		 {
			CloseHandle(file);
		   return FALSE;
		 }

		CloseHandle(file);
		 

		 
	}

	return TRUE;
}
	 



	
void EnterHighscore()
{
	char z;
	string buffer,temp;
	int c,z1;




  if (punkte >= highscore)
  {
	Cls(lpddsPrimary);
    Cls(lpddsBack);
    DrawBitmap(0,0,MAKEINTRESOURCE(IDB_BITMAP7));
	lpddsPrimary -> Blt(&rect, lpddsBack, NULL, DDBLT_WAIT, NULL);
	while(lpddsPrimary->Flip(NULL, DDFLIP_WAIT) != DD_OK);
	DD_Fadeup(fadespeed);

	lpddsPrimary->GetDC(&hdc);//Get the DC of the surface
	SetFontColor(255,255,255);
	SetPen(255,0,0,1);	
	SetFont("Arial",16,0,0,0);
	Text(150,220,"Bitte Namen / Kürzel eingeben:");

	c = 0;
	buffer = "";

	while  (HIBYTE(GetAsyncKeyState(VK_RETURN)) <= 1)
	{
	   for (z = 0x30; z <= 0x5A; z++)
	   {
		if (KeyHit(z) == TRUE)
		{
		  if (c <= 19)
		  {
			buffer += (char) z;
			c++;
			z = (char) 0;
		  }
		}
	   }

	   	if ((KeyHit(VK_BACK) == TRUE) && (c > 1))
		{		
		  SetFontColor(0,0,0);
		  Text(150,240,buffer.data());

		  temp = "";
		  for (z1 = 0; z1 < c-2; z1++)
		  {
			temp += buffer.data()[z1];
		  }
		  c--;
		  buffer = "";
		  buffer = temp;
		}

		if ((KeyHit(VK_SPACE) == TRUE) && (c <= 19))
		{		
		  buffer += ' ';
		  c++;
		}

	   SetFontColor(255,0,0);
	   Text(150,240,buffer.data());
	  }	
	GDI_Cleanup();//Get the DC of the surface

	winner.Score = punkte;
	strcpy(winner.Name,buffer.data());
	WriteHighscore();
    DD_Fadedown(fadespeed);
	ShowHighscore();


	
  }
}





int InitBitmaps()
{
		
 invBitmap = LoadBitmap (hInstance,MAKEINTRESOURCE(IDB_BITMAP2));
 if (invBitmap == 0) 
 {
	return -1;
 }

  GetObject (invBitmap, sizeof (BITMAP), &bm) ;

    ZeroMemory(&ddsd, sizeof(ddsd));
	
    ddsd.dwSize = sizeof(ddsd); 
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH; 
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; 
    ddsd.dwWidth = bm.bmWidth; 
    ddsd.dwHeight = bm.bmHeight; 
	ddrval =  lpDirectDraw -> CreateSurface (&ddsd, &lpddsBitmap, NULL);
	
	if (ddrval != DD_OK)
	{	
		return -1;
	}

 
  
	
  DDCopyBitmap(lpddsBitmap, invBitmap, 0,0,bm.bmWidth, bm.bmHeight);  
  DD_AddColorKey(lpddsBitmap, 0, 0);
 
  
 
 

  return 0;
			
}
	





void Feld2Cords(_posi feld, int &XP, int &YP)
{
	XP = feld.x * stepX;
	YP = feld.y * stepY;
	YP = YP + .5*stepY;
	XP = XP + .5*stepX;
	YP = YP + 20;
}

_posi Cords2Feld(int XP, int YP)
{
 _posi tfeld;

	tfeld.x = XP / stepX;
	tfeld.y = YP / stepY;

 return tfeld;
}







void DrawSphere(int x,int y,int c)
{
 RECT srcr;
 lpddsBack->Restore(); //Make sure the surface is restored	
	
	    if (c == 1)
		{
		 SetRect(&srcr,1,0,50,50);
		 lpddsBack->BltFast(x-25,y-25,lpddsBitmap,&srcr, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);		 
		}
		
		if (c == 2)
		{	
		 SetRect(&srcr,50,0,100,50);
		 lpddsBack->BltFast(x-25,y-25,lpddsBitmap,&srcr, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);		 
		}

		if (c == 3)
		{
		 SetRect(&srcr,100,0,150,50);
		 lpddsBack->BltFast(x-25,y-25,lpddsBitmap,&srcr, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);		 		 
		}
		
		if (c == 4)
		{
		 SetRect(&srcr,150,0,200,50);
		lpddsBack->BltFast(x-25,y-25,lpddsBitmap,&srcr, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);		  
		}
		
		if (c == 5)
		{
		 SetRect(&srcr,200,0,250,50);
		lpddsBack->BltFast(x-25,y-25,lpddsBitmap,&srcr, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);		  
		}
		
		if (c == 6)
		{
		 SetRect(&srcr,250,0,300,50);
		lpddsBack->BltFast(x-25,y-25,lpddsBitmap,&srcr, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);		  
		}
}





	

void DrawGame()
{
	int mx,my,c,c1;
	_posi tfeld;

	lpddsBack->Restore(); //Make sure the surface is restored	
 
	for (int x = 0; x < 9; x++) 
	{
		for (int y = 0; y < 9;y++)
		{
		
			if (game[x][y] != 0)
			{
				c = ((y*8)+x);
				c1 = ((ziel.y*8)+ziel.x);

				if (gameover == TRUE)
				{
					tfeld.x = x;
					tfeld.y = y;
					Feld2Cords(tfeld,mx,my);				
					DrawSphere(mx,my,game[x][y]);	
				}
				else
				{
					if ( c != c1) 
					{
						tfeld.x = x;
						tfeld.y = y;
						Feld2Cords(tfeld,mx,my);				
						DrawSphere(mx,my,game[x][y]);	
					}
			   }
			}
		}
	}
 
	

}




void HandleLine(_posi start,int l,int direction)
{
	int z;
 
   switch(direction)
   {
	
	   
	   
   case senkrecht:
	{
		for  (z = 1; z <= l; z++)
		{
			game[start.x][start.y+(z-1)] = 0;
			walkability[start.x][start.y+(z-1)] = walkable;
		}
		break;
	}


   case waagerecht:
	{
		for  (z = 1; z <= l; z++)
		{
			game[start.x+(z-1)][start.y] = 0;
			walkability[start.x+(z-1)][start.y] = walkable;
		}
		break;
	}

   case diagonalR:
	{
		for  (z = 1; z <= l; z++)
		{
			game[start.x+(z-1)][start.y+(z-1)] = 0;
			walkability[start.x+(z-1)][start.y+(z-1)] = walkable;
		}
		break;
	}

   case diagonalL:
	{
		for  (z = 1; z <= l; z++)
		{
			game[start.x-(z-1)][start.y+(z-1)] = 0;
			walkability[start.x-(z-1)][start.y+(z-1)] = walkable;
		}
		break;
	}
   
   break;
   }


	
    Draw();

 if (SOUND == TRUE)
 {
	 if (s == TRUE) 
	 {
		 LoadWave(CLEAR);
	 }
	 else
	 {
		 LoadWave(CLEAR2);
	 }
	
	 PlayWave();
 }
}













bool DetectLine(_posi last, bool POINTS)  //Detects if a line has been completed
{
	int col,c;
	_posi pos,start;


	c = 0;
	col = game[last.x][last.y]; // get actual color
	pos = last;

	s = POINTS;


	if (startpos == FALSE)
	{



    //Diagonal line to the right

   if ((game[last.x-1][last.y-1] == col) || (game[last.x+1][last.y+1] == col)) 
   {
	  if (game[last.x-1][last.y-1] == col)
	  {
		while (game[pos.x][pos.y] == col)  //find upper left starting point
		{
			pos.x = pos.x - 1;
			pos.y = pos.y - 1;
		}
	    pos.x++;  //correct position
		pos.y++;  
	  }

		c = 1;

	   
		start = pos;

		while (game[pos.x][pos.y] == col)  //how many stones?
		{
			pos.x = pos.x + 1;
			pos.y = pos.y + 1;
			c++;
		}

		c--;
				ll = c;

		if (c >= linefull)  //Yep! Line completed
		{
			HandleLine(start,c,diagonalR);
			if (POINTS == TRUE) punkte = punkte + (c*2);
			Draw();	
			return TRUE;
		}
	
   } //Diagonal R



	c = 0;
	col = game[last.x][last.y]; // get color
	pos = last;
	ZeroMemory(&start,sizeof(start));




     //Diagonal line to the left?
   if ((game[last.x+1][last.y-1] == col) || (game[last.x-1][last.y+1] == col))
   {

	  if (game[last.x+1][last.y-1] == col)
	  {
		while (game[pos.x][pos.y] == col)  //find upper right starting point
		{
			pos.x = pos.x + 1;
			pos.y = pos.y - 1;
		}

	    pos.x--;  
		pos.y++; 
	  }
	
		c = 1;
	 	start = pos;

	
		while (game[pos.x][pos.y] == col)  //how many stones?
		{
			pos.x = pos.x - 1;
			pos.y = pos.y + 1;
			c++;
		}

		c--;
		ll = c;

		if (c >= linefull)  //Yep! Line completed
		{
			HandleLine(start,c,diagonalL);
			if (POINTS == TRUE) punkte = punkte + (c*2);
			Draw();
			
			return TRUE;
		}
	
	
   } //Diagonal L


	c = 0;
	col = game[last.x][last.y]; // get color
	pos = last;
	ZeroMemory(&start,sizeof(start));
	

   //Vertical line

   if ((game[last.x][last.y+1] == col) || (game[last.x][last.y-1] == col))
   {
	  
	  if (game[last.x][last.y-1] == col)
	  {
		while (game[pos.x][pos.y] == col)  //find upper start 
		{
			pos.y = pos.y - 1;
		}
	    pos.y++;  
	  }
		
		c = 1;
	  
		start = pos;

		while (game[pos.x][pos.y] == col)  //how many stones?
		{
			pos.y = pos.y + 1;		
			c++;
		}

		c--;


		ll = c;
		if (c >= linefull)  //Yep! Line completed
		{
			HandleLine(start,c,senkrecht);
			if (POINTS == TRUE) punkte = punkte + (c*2);			
			Draw();
			return TRUE;
		}


	
   } //vertical L


	c = 0;
	col = game[last.x][last.y]; // get color
	pos = last;
	ZeroMemory(&start,sizeof(start));


    //Horizontal line

   if ((game[last.x-1][last.y] == col) || (game[last.x+1][last.y] == col))
   {

	   
	  if (game[last.x-1][last.y] == col)
	  {
		while (game[pos.x][pos.y] == col)  //Find left starting point
		{
			pos.x = pos.x - 1;
		}
	  
		   pos.x++;  
	  }
		


		c = 1;
	    start = pos;

		while (game[pos.x][pos.y] == col)  //how many stones
		{
			pos.x = pos.x + 1;		
			c++;
		}

		c--;

		ll = c;
		if (c >= linefull)  //Yep! Linie completed
		{
			HandleLine(start,c,waagerecht);
			if (POINTS == TRUE) punkte = punkte + (c*2);		
		    Draw();	
			return TRUE;
		}
	
   } //waagerecht


   	c = 0;
	col = game[last.x][last.y]; 
	pos = last;
	ZeroMemory(&start,sizeof(start));

	Draw();
	}

  	return FALSE;

   }



void GetFree()
	//Count free spaces on the playfield
{
	 ZeroMemory(&freef,sizeof(freef));	 
	  maxfree = 0;
											
	  for (int x = 0; x < 9; x++)	
	  {
		for (int y = 0; y < 9; y++)
		{
			if (game[x][y] == 0)
			{			
				freef[maxfree].x = x;
				freef[maxfree].y = y;		
				maxfree++;									
			}
		}
	  }
}


	
bool Drop(int num) //Drops "num" new stones to the playfield
{
	int cx,cy, c1,c2,c3;
	_posi Feld;

	

 if (SOUND == TRUE)
 {
	 LoadWave(DROP);
	 PlayWave();
 }

 	c3 = 1;
	cx = 1;
	cy = 1;
    c2 = 1;
     


     GetFree();

	  if (maxfree <= num)    //Do we have enough free space?
	  {
	
		  c1 = 0;
		  GetFree();
		 
		for (int x = 0; x < 9; x++)	
		{
		for (int y = 0; y < 9; y++)
		{
			if (game[x][y] == 0)
			{			
				 c3 = zufall(4);			//Random position
				 game[x][y] = c3;
				 walkability[x][y] = unwalkable;
		    
				 Feld.x = x;
				 Feld.y = y;
		  
				if (DetectLine(Feld,FALSE) == TRUE) //Line completed by new stone`?
				{
					GetFree();
					lpddsBack->Restore(); 
					Draw();	
					return TRUE;
				}
				maxfree--;									
			}
		}
	  }


		 
		

		    if (maxfree < 0) maxfree = 0;

		   //No free spaces left? So the game is over

		     game[ziel.x][ziel.y] = col;

			 gameover = TRUE;

		     lpddsBack->Restore(); //Make sure the surface is restored	
			 Draw();
			 lpddsPrimary -> Blt(&rect, lpddsBack, NULL, DDBLT_WAIT, NULL);


			
			 GameOver();	
			 return FALSE;
			
			

		 
	  }
	  else
	  {	        

		  
			for (c2 = 1; c2 < (num+1); c2++)
			{
				c1 = 0;
				c1 = zufall(maxfree);			
				cx = 0;
				cy = 0;		
				cx = freef[c1].x;	
				cy = freef[c1].y;
       		
				c3 = (c1 % 6)+1;								
				game[cx][cy] = c3;
				walkability[cx][cy] = unwalkable;		
			
				lpddsBack->Restore(); //Make sure the surface is restored	
				Draw();	

				Feld.x = cx;
				Feld.y = cy;
				
				if (DetectLine(Feld,FALSE) == TRUE) //Line completed?
				{
					lpddsBack->Restore(); 
					Draw();	
					return TRUE;
				}


				maxfree--;
				  

			}
	    
		}

	  	

		


 return TRUE;
}






int CorrectPath(int &tx, int &ty)
{
	_posi X;
	int ax,ay;

	if (tx   > xPath)  
	{
	 if ((tx - ballspeed) < xPath) 
	 {
		tx = xPath;
	 }
	 else
	 {
	  tx   = tx   - ballspeed;
	 }

	 X = Cords2Feld(tx,ty);
	 Feld2Cords(X,ax,ty);
	 return 0;
	}
	
	if (tx   < xPath)   
	{
	 if ((tx + ballspeed) > xPath) 
	 {
		tx = xPath;
	 }
	 else
	 {
	  tx   = tx   + ballspeed;
	 }

	 X = Cords2Feld(tx,ty);
	 Feld2Cords(X,ax,ty);
	 return 0;
	}
	
	if (ty   > yPath) 
	{
	
	 if ((ty - ballspeed) < yPath) 
	 {
		ty = yPath;
	 }
	 else
	 {
	   ty   = ty   - ballspeed;
	 }

	 X = Cords2Feld(tx,ty);
	 Feld2Cords(X,tx,ay);
	 return 0;
	}

	if (ty   < yPath)   
	{

	 if ((ty + ballspeed) > yPath) 
	 {
		ty = yPath;
	 }
	 else
	 {
	   ty   = ty   + ballspeed;
	 }

	 X = Cords2Feld(tx,ty);
	 Feld2Cords(X,tx,ay);
	 return 0;
	}

	return 1;
}
	


bool MoveSphere()   //Moves a stone across the playield
{
	int c,c1,ax,bx,ay,by;
	_posi X,X1;
	


   
	ReadPath(tx,ty,2); //Get path info

	CorrectPath(tx,ty); //Correct positions
	

	if (pathLocation == pathLength+1)  //End of path
	{	    	  	
		movemode  = FALSE;		
		Feld2Cords(ziel,tx,ty);

		if (abs(tx - xPath) <= 0) tx = xPath;
		if (abs(ty - yPath) <= 0) ty = yPath;	
	}
	





     X = Cords2Feld(tx,ty);	
	X1 = Cords2Feld(tx1,ty1);	

     c = (((X.y) *9)+X.x);
	c1 = (((X1.y)*9)+X1.x);


	if (tx > tx1)
	{
	  ax = tx;
	  bx = tx1;
	}
	else
	{
	  ax = tx1;
	  bx = tx;
	}


	if (ty > ty1)
	{
	  ay = ty;
	  by = ty1;
	}
	else
	{
	  ay = ty1;
	  by = ty;
	}



     if ((c == c1) && (tx == xPath) && (ty == yPath))		//Ending posiion reached?
	 {

			game[startP.x][startP.y] = 0;
			walkability[startP.x][startP.y] = walkable;	
			d = DetectLine(ziel,TRUE);
			if ( d == FALSE) 
			{		 	
 			game[ziel.x][ziel.y] = col;
			walkability[ziel.x][ziel.y] = unwalkable;
			Drop(dropval);			
			}
			else
			{
 			game[ziel.x][ziel.y] = 0;
			walkability[ziel.x][ziel.y] = walkable;
			}
		
			movefinished = TRUE;
			ziel.x = 100; 
			ziel.y = 100;
			movemode = FALSE;
			tx1 = tx;
			ty1 = ty;
	
	 }


	return movemode;

	
}









void Moveto(_posi s, _posi z)
{

	 ziel = z;
	 col = game[s.x][s.y];
     
	
     Feld2Cords(s,tx,ty);
	 Feld2Cords(ziel,tx1,ty1);
   
	 

  

   pathStatus = FindPath(tx,ty,tx1,ty1);

	  
   if (pathStatus != found)  //No path can be calculated
   {

	   if (SOUND == TRUE)
	   {
		 LoadWave(NO);
		 PlayWave();
		}

	 game[s.x][s.y] = col;
	 walkability[s.x][s.y] = unwalkable;

	 game[ziel.x][ziel.y] = 0;
	 walkability[ziel.x][ziel.y] = walkable;

	 tx1 = tx;
	 ty1 = ty;
	 ziel.x = 100; 
	 ziel.y = 100;
	 movemode = FALSE;	
	 movefinished = TRUE;
	 d = TRUE;
   }
   else
   {	   
	   movefinished = FALSE;
	   movemode = TRUE;
	   d = FALSE;

	   game[startP.x][startP.y] = 0;
	   walkability[startP.x][startP.y] = walkable;
       game[ziel.x][ziel.y] = col;
	   walkability[ziel.x][ziel.y] = unwalkable;
	

	   
	 


		MoveSphere();

		



   }
  

   
 
  
 
}

			
		 

void Markit(_posi feld)
{
    mark = feld;
  	marked = TRUE;
	DrawGame();
	       	
}









 void move()
 {
	 _posi feld;
	 ///Stop last movement
	if (movemode == TRUE)
	{
		game[startP.x][startP.y] = 0;
		walkability[startP.x][startP.y] = walkable;	
		d = DetectLine(ziel,TRUE);
		if ( d == FALSE) 
		{		 	
 		 game[ziel.x][ziel.y] = col;
		 walkability[ziel.x][ziel.y] = unwalkable;
		 Drop(dropval);			
		}
		else
		{
 		 game[ziel.x][ziel.y] = 0;
		 walkability[ziel.x][ziel.y] = walkable;
		 DrawGame();
		}
		

		movefinished = TRUE;
		ziel.x = 100; 
		ziel.y = 100;
		movemode = FALSE;
		tx1 = tx;
		ty1 = ty;
	}



	lpddsBack->Restore(); 
    Draw();
    lpddsPrimary -> Blt(&rect, lpddsBack, NULL, DDBLT_WAIT, NULL);
 


  	
     
	

		if ((maus.y > 20) && (maus.y < 523) && (maus.x > 1) && (maus.x < 799)) //New path set by mouse click
		{

				feld = Cords2Feld(maus.x,maus.y-20); //First 20 pixels don't belong to the playfield. Therefore -20

	
			
 
			if (game[feld.x][feld.y] >= 1)  //Click on a playstone
			{ 

					

				// Stone already marked?
				if (marked == TRUE) 
				{				
					marked = FALSE;
				    Draw();
					startP = feld; 
					Markit(startP);						
				}
				else	//No? So mark the stone
				{		
					DetectLine(feld,TRUE);
					Draw();				
					startP = feld; 
					Markit(startP);					
				}
	
	
			}
			else            //Not clicked on a playstone?
			{

				if (marked == TRUE)    //Is a stone marked?
				{
					movemode = TRUE;        //...than move it
					Moveto(startP,feld);
					marked = FALSE;								
				}
			}

		}



 }



// Main menu


void DrawMain()
{
  DrawBitmap(0,0,MAKEINTRESOURCE(IDB_MAIN));
  //flip buffers
  lpddsPrimary -> Blt(&rect, lpddsBack, NULL, DDBLT_WAIT, NULL);
  while(lpddsPrimary->Flip(NULL, DDFLIP_WAIT) != DD_OK);
}




void GameOver()
{
   DD_Fadedown(fadespeed*4);
   SetGamma(0,0,0);
   gameover = TRUE;
   DrawBitmap(0,0,MAKEINTRESOURCE(IDB_BITMAP3));
   lpddsBack->GetDC(&hdc);//Get the DC of the surface
   SetFontColor(255,255,255);
   SetFont("Arial",14);
   Text(150,520,"Drücke eine Maustaste....");

   	GDI_Cleanup();//Release the DC of the surface

	 lpddsPrimary -> Blt(&rect, lpddsBack, NULL, DDBLT_WAIT, NULL);
	         while(lpddsPrimary->Flip(NULL, DDFLIP_WAIT) != DD_OK);
   DD_Fadeup(fadespeed);

   WaitMouse();	
   
	DD_Fadedown(fadespeed);		
    EnterHighscore();

    ClearGame();

    
	Main();

	


}



bool QuitQuestion()
{
	lpddsBack->Restore(); //Make sure the surface is restored	
	
    Cls(lpddsBack);
	DrawBitmap(0,0,MAKEINTRESOURCE(IDB_BITMAP6));
	Cls(lpddsPrimary);
	lpddsPrimary -> Blt(&rect, lpddsBack, NULL, DDBLT_WAIT, NULL);
	while(lpddsPrimary->Flip(NULL, DDFLIP_WAIT) != DD_OK);

	
	  

   
  
  
  while (1)
  {

	  GetCursorPos(&maus);
	  
	 
	  if (KeyHit(VK_LBUTTON) == TRUE)
	  {
		  if (((maus.x > 88) && (maus.x < 375)) &&   //Ja
			  ((maus.y > 291) && (maus.y < 421)))
		  {	
			 return TRUE;
		  }

		  if (((maus.x > 427) && (maus.x < 712)) &&   //Nein
			  ((maus.y > 291) && (maus.y < 421)))
		  {
			
			 return FALSE;
		  }
	  }
  }

}


	
	

 

bool Main()
{

   SetGamma(0,0,0);
   
   DrawMain();

  
   DD_Fadeup(fadespeed);
  
 
  
  while (1)
  {

	  GetCursorPos(&maus);
	  


	  if (KeyHit(VK_ESCAPE) == TRUE) //ESC pressed
	  {	
		  DD_Fadedown(fadespeed);
		  Cls(lpddsPrimary);
		  RestoreOriginalGamma(); //Restore gamma
		  PostMessage(hwnd,WM_DESTROY,0,0);	
		  return FALSE;
	  }




	  if (KeyHit(VK_LBUTTON) == TRUE)
	  {

		   if (((maus.x > 275) && (maus.x < 533)) &&   //Button  "End game" pressed
		   ((maus.y > 518) && (maus.y < 599)) )
			{	
			    if (SOUND == TRUE)
				{
				 LoadWave(CLICK);
				 PlayWave();
				}
			
				DD_Fadedown(fadespeed);
				Cls(lpddsPrimary);
				RestoreOriginalGamma(); //Restore gamma
				PostMessage(hwnd,WM_DESTROY,0,0);	
				return FALSE;
			}



		  if (((maus.x > 271) && (maus.x < 533)) &&   //Play button
			  ((maus.y > 435) && (maus.y < 518)))
		  {

			    if (SOUND == TRUE)
				{
				 LoadWave(CLICK);
				 PlayWave();
				}
				DD_Fadedown(fadespeed);
			  	  	//startparameter setzen		
			
				Cls(lpddsPrimary);
				Cls(lpddsBack);
			    ClearGame();
				Drop(linefull);
				SetGamma(0,0,0);
				lpddsPrimary -> Blt(&rect, lpddsBack, NULL, DDBLT_WAIT, NULL);
	            DD_Fadeup(fadespeed);	  
	           	
				
			return TRUE;
		  }

		   if (((maus.x > 0) && (maus.x < 258)) &&   //Highscore button
			  ((maus.y > 518) && (maus.y < 599)))
		  {
			   if (SOUND == TRUE)
				{
				 LoadWave(CLICK);
				 PlayWave();
				}
			  DD_Fadedown(fadespeed);
			  SetGamma(0,0,0);
			  ShowHighscore();
		      DrawMain();
			  DD_Fadeup(fadespeed);
  
			  
	
		  }

		   if (((maus.x > 542) && (maus.x < 799)) &&   //Info button
			  ((maus.y > 518) && (maus.y < 599)))
		  {		      
			   if (SOUND == TRUE)
				{
				 LoadWave(CLICK);
				 PlayWave();
				}
			   DD_Fadedown(fadespeed);
			   ShowInfo();
			  
		       SetGamma(0,0,0);
		       DrawMain();		
			   SetGamma(0,0,0);
		       DD_Fadeup(fadespeed);
 
	
		  }
	  }

	 
  }



}



bool CheckFile(string name)  //Checks if the specified file can be found
{
		
	file = CreateFile(name.data(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,0);
	
	if (file == INVALID_HANDLE_VALUE)  
	{
	  return FALSE;
	}
	else
	{
		CloseHandle(file);
		return TRUE;
	}
}







    


