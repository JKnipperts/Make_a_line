//=============================== Soume routines for the graphics ==========================================

void SaveOriginalGamma();
void RestoreOriginalGamma();








string GetAdapterName()
{
	string s1,s2,s3;
	int e;
    HKEY				hkey;
	DWORD			    dwData512 = 512;
  

	unsigned char    videoreg[256];
    unsigned char    videoname[256];


	RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\VIDEO\0", 0, KEY_EXECUTE, &hkey);
    RegQueryValueEx(hkey, "\\Device\\Video0",0,0,videoreg,&dwData512);
    RegCloseKey(hkey);

	s1 = (char*) videoreg;
	e = strlen("\\REGISTRY\\Machine\\System");
	s2 = "SYSTEM";
	s2 += GetStr(s1,e,(strlen(s1.data())-e));
	s3 = s2; 

	 RegOpenKeyEx(HKEY_LOCAL_MACHINE,s3.data(), 0, KEY_EXECUTE, &hkey);
	 RegQueryValueEx(hkey, "Device Description",0,0,videoname,&dwData512);
	 RegCloseKey(hkey);


	 ZeroMemory(&s1,sizeof(s1));
	 s1 = (char*) videoname;
	 return s1;
}


string GetCPUName()
{
	unsigned char       cpuname[256];
	DWORD			    dwData256 = 256;
    HKEY				hkey;
	string				s;
	
  
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_EXECUTE, &hkey);
    RegQueryValueEx(hkey, "ProcessorNameString",0,0,cpuname,&dwData256);
    RegCloseKey(hkey);
    s = (char*) cpuname;
	return s;
}

	
int AvailSysMem(void) 
{ 	
	MEMORYSTATUS stat;
	GlobalMemoryStatus (&stat);
	return(stat.dwAvailPhys);
}

int AvailVidMem(void) 
{
	DWORD totalVRAM=0;
	DWORD freeVRAM=0;
	DDSCAPS2 ddscaps;
	ZeroMemory(&ddscaps, sizeof(ddscaps));//clear memory block for structure
	ddscaps.dwCaps = DDSCAPS_VIDEOMEMORY;  
	lpDirectDraw->GetAvailableVidMem(&ddscaps,&totalVRAM,&freeVRAM);
	return freeVRAM;
} 



int DD_InitDirectDraw()
{

   ddrval = DirectDrawCreateEx(NULL, (VOID**)&lpDirectDraw, IID_IDirectDraw7, NULL);
  
   if (ddrval != DD_OK)
   {

      return (-1);
   }

 

return 0;
 
} 




BOOL DD_SetVideoMode(int vx,int vy,int vbpp) //Sets full screen video mode with the specified resolution
{



ddrval = lpDirectDraw->SetCooperativeLevel (hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);

   
if (ddrval != DD_OK)
   {      
      return (-1);
   }


  ddrval = lpDirectDraw->SetDisplayMode (vx, vy, vbpp, 0, 0);
  

  if (ddrval != DD_OK)
   {      
      return (-1);
   }


   return 0;
}





int DD_InitSurfaces()
{	
//Init surfaces

	ZeroMemory(&ddsd, sizeof(ddsd));
	

	
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT ;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP  | DDSCAPS_COMPLEX;

	ddsd.dwBackBufferCount = 1; 
	ddrval = lpDirectDraw -> CreateSurface (&ddsd, &lpddsPrimary, NULL);
	
	if (ddrval != DD_OK)
	{	
		return (-1);
	}

	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
	lpddsPrimary->GetAttachedSurface(&ddscaps, &lpddsBack);
	SaveOriginalGamma();


 

	return 0;
}



void DD_CopyBlock(LPDIRECTDRAWSURFACE7 SourceSurface, int sx, int sy, int sx1, int sy1,LPDIRECTDRAWSURFACE7 DestSurface, int dx, int dy)
{
 RECT srcr;
 SourceSurface->Restore(); //Make sure the surface is restored	
 SetRect(&srcr,sx,sy,sx1,sy1);
 DestSurface->Restore(); //Make sure the surface is restored	
 DestSurface -> BltFast(dx,dy,SourceSurface,&srcr, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);		  
}



int DD_InitClipper()
{
	//Init clipper so we can write out of the screen borders

	ddrval = lpDirectDraw -> CreateClipper(0, &lpddClipper, NULL);

	if (ddrval != DD_OK)
	{		
	  return -1;
	}

	struct 
	{
		RGNDATAHEADER  rdh;
		RECT           rgndata[1];
	} 
	cliplist;

	cliplist.rdh.dwSize   = sizeof (RGNDATAHEADER);
	cliplist.rdh.iType   = RDH_RECTANGLES;
	cliplist.rdh.nCount   = 1;
	cliplist.rdh.nRgnSize  = 0;
	cliplist.rdh.rcBound.left = 0;
	cliplist.rdh.rcBound.top = 0;
	cliplist.rdh.rcBound.right = GetSystemMetrics(SM_CXSCREEN);
	cliplist.rdh.rcBound.bottom = GetSystemMetrics(SM_CYSCREEN);
	cliplist.rgndata[0].left = 0;
	cliplist.rgndata[0].top  = 0;
	cliplist.rgndata[0].right = GetSystemMetrics(SM_CXSCREEN);
	cliplist.rgndata[0].bottom = GetSystemMetrics(SM_CYSCREEN);

	ddrval = lpddClipper -> SetClipList ((LPRGNDATA) &cliplist, 0);

	if (ddrval != DD_OK)
	{	
		return -1;
	}
	
	ddrval = lpddsBack -> SetClipper(lpddClipper);

	if (ddrval != DD_OK)
	{	
		return -1;
	}

	return 0;
}



//Clears the screen and init paint stuff
int DD_InitPaintProcs()
{


	//Create a big black block :)


	ddbltfx.dwSize      = sizeof(DDBLTFX);
	ddbltfx.dwFillColor = RGB(0, 0, 0); 
	SetRect(&fill_area, 0, 0, GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN)); 


	//And now blit it to the screen to clear it

	HRESULT Blt
	(
	   LPRECT lpDestRect,            //coordinates of the target
	   LPDIRECTDRAWSURFACE7 lpDDSrcSurface, //Source surface
	   LPRECT lpSrcRect,			//coordinates of the source 
	   DWORD dwFlags,
	   LPDDBLTFX lpDDBltFx          //color to blit
	);

   ddrval = lpddsBack -> Blt(&fill_area, NULL, NULL, DDBLT_COLORFILL    | DDBLT_WAIT, &ddbltfx);

   if (ddrval != DD_OK)
	{      
      return (-1);
	}

   return 0;
}


long ConvToUnSignedValue(int intValue)
{

    if (intValue >= 0)
	{
		return intValue;
    }
	else
	{
        return (intValue + 65535);
	}

 return 0;
}

int ConvToSignedValue(long lngValue)
{
    if (lngValue <= 32767) 
	{
       return (int) lngValue;
	}
    else
	{
      return (int) (lngValue - 65535);
	}
return 0;
}


void RestoreOriginalGamma()
{
	lpddsPrimary -> QueryInterface(IID_IDirectDrawGammaControl,(void **)&lpDDGammaControl);
	lpDDGammaControl->SetGammaRamp(0, &OriginalRamp); 
}

void SaveOriginalGamma()
{
	lpddsPrimary -> QueryInterface(IID_IDirectDrawGammaControl,(void **)&lpDDGammaControl);
    lpDDGammaControl->GetGammaRamp(0, &OriginalRamp); //Alte Gamma Werte sichern
}


void GetGamma()
{
 lpddsPrimary -> QueryInterface(IID_IDirectDrawGammaControl,(void **)&lpDDGammaControl);
 
 lpDDGammaControl->GetGammaRamp(0, &GammaRamp); //Werte holen

 for (int i=0; i < 256; i++)
 {
	gammacent[i][1] = ConvToSignedValue(ConvToUnSignedValue(GammaRamp.red[i] / 100));
    gammacent[i][2] = ConvToSignedValue(ConvToUnSignedValue(GammaRamp.green[i] / 100));
    gammacent[i][3] = ConvToSignedValue(ConvToUnSignedValue(GammaRamp.blue[i] / 100));
 }

}



 


void SetGamma(unsigned char Red,unsigned char Green,unsigned char Blue)
//Sets gamma values in percent
{
 lpddsPrimary -> QueryInterface(IID_IDirectDrawGammaControl,(void **)&lpDDGammaControl);
 lpDDGammaControl->GetGammaRamp(0, &GammaRamp); //und in den Puffer zum manipulieren

  

 for (int i=0; i < 256; i++)
 {
	GammaRamp.red[i] = ConvToSignedValue(ConvToUnSignedValue(OriginalRamp.red[i]) * Red / 100);
    GammaRamp.green[i] = ConvToSignedValue(ConvToUnSignedValue(OriginalRamp.green[i]) * Green / 100);
    GammaRamp.blue[i] = ConvToSignedValue(ConvToUnSignedValue(OriginalRamp.blue[i]) * Blue / 100);

 }

 lpDDGammaControl->SetGammaRamp(0, &GammaRamp); 
}

  



void DD_Fadedown(int time)
{

 for (int i=100; i > 1; i--)
 {	 
	 SetGamma(i,i,i);
	 Sleep(time);
 }
 
 
}


void DD_Fadeup(int time)
{
  
	 SetGamma(0,0,0);
 for (int i=1; i < 100; i++)
 {	 
	 SetGamma(i,i,i);
	 Sleep(time);
 }


}







//ColorKey for transparent bitmaps

void DD_AddColorKey (LPDIRECTDRAWSURFACE7 surface, DWORD Low, DWORD High)
{

	DDCOLORKEY key;
	ZeroMemory(&key, sizeof(key));
	key.dwColorSpaceLowValue = Low;
	key.dwColorSpaceHighValue = High;
	surface->SetColorKey (DDCKEY_SRCBLT, &key);
 
}

		  


int Cls (LPDIRECTDRAWSURFACE7 CurrentSurface)
{

	DDBLTFX ddbltfx;
	ZeroMemory( &ddbltfx, sizeof(ddbltfx));
	ddbltfx.dwSize      = sizeof(ddbltfx);
	ddbltfx.dwFillColor = RGB(0, 0, 0); 

	// Blit the entire current buffer with the specified fill color
	HRESULT hr = CurrentSurface->Blt(
	NULL, //destination rectangle of blit (0 = blit entire surface)
	NULL, //address of source surface blitted from (0 used here)
	NULL, //source rectangle of blit (0 = blit from entire surface)
	DDBLT_COLORFILL | DDBLT_WAIT, //blit flags; DDBLT_COLORFILL = use dwFillColor member
	// of the DDBLTFX structure as the RGB color that fills the destination
	&ddbltfx );//address of special effects ddbltfx structure created above

	if (hr == DD_OK)
		return 1;
	else
		return hr;
	
 return 0;
}


void GDI_Cleanup()
{
	   if (Pen != NULL)
	   {
		 DeleteObject(Pen);
	   }
	   if (Font != NULL)
	   {
		 DeleteObject(Font);
	   }

       lpddsPrimary->ReleaseDC(hdc);
	   lpddsBack->ReleaseDC(hdc);
}

void DD_Cleanup()
{
	
	  RestoreOriginalGamma(); 
	
	  lpDirectDraw->RestoreDisplayMode();
		
	   if (lpDirectDraw != NULL)
	   {
         lpDirectDraw -> Release();
         lpDirectDraw = NULL;
	   }

	   if (lpddsPrimary != NULL)
	   {      
         lpddsPrimary = NULL;
	   }

	   if (lpddsBack != NULL)
	   {
         lpddsBack -> Release();
         lpddsBack = NULL;
	   }
	   if (lpddsBitmap != NULL)
	   {
         lpddsBitmap -> Release();
         lpddsBitmap = NULL;
	   }

	   if (lpddClipper != NULL)
	   {
		lpddClipper->Release();
		lpddClipper = NULL;
	   } 
	 
}


int DrawBitmap(int x, int y, const char* resnr)
{
	
		hBitmap = LoadBitmap (hInstance,resnr) ;

		if (hBitmap == 0)
		{
		  return -1;
		}

	    GetObject (hBitmap, sizeof (BITMAP), &bitmap) ;
 
		ZeroMemory(&ddsd, sizeof(ddsd));
	
	    ddsd.dwSize = sizeof(ddsd); 
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH; 
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN; 
		ddsd.dwWidth = bitmap.bmWidth; 
		ddsd.dwHeight = bitmap.bmHeight; 
		ddrval =  lpDirectDraw -> CreateSurface (&ddsd, &lpddsBitmap1, NULL);
	
		if (ddrval != DD_OK)
		{	
		  return -1;
		}
	
		DDCopyBitmap(lpddsBitmap1, hBitmap, 0,0,bitmap.bmWidth, bitmap.bmHeight);  
		DD_AddColorKey(lpddsBitmap1, 0, 0);

		DeleteObject (hBitmap) ;
		SetRect(&rectbitmap,0,0,bitmap.bmWidth, bitmap.bmHeight);
		
		lpddsBack->BltFast(x,y,lpddsBitmap1,&rectbitmap,NULL);
		lpddsBitmap1->Release();

      
	 return 0;
}


void SetFontColor (int r, int g, int b)
{
	
	FontColor = RGB(r,g,b); 

}

void SetPen(int r, int g, int b, int width)
{

	Pen = CreatePen(PS_SOLID,width,RGB(r,g,b));
	SelectObject(hdc,Pen);
 
}



int SetFont (char* typeface, int typesize, int bold=0, int italic=0, 
			 int underline=0)
{

	int weight = 0;
	if (bold==0) weight = 200;
	if (bold==1) weight = 700;

	if (Font != NULL) DeleteObject(Font);
	Font = NULL;
	Font = CreateFont(
		typesize,//height of font
		0,//int average character width
		0,//int angle of escapement
		0,//int base-line orientation angle
		weight,//font weight 0-1000 
		italic,//DWORD italic attribute option
		underline,//DWORD underline attribute option
		0,//DWORD strikeout attribute option
		0,//DEFAULT_CHARSET,//DWORD character set identifier
		0,//OUT_DEFAULT_PRECIS,//DWORD output precision
		0,//CLIP_DEFAULT_PRECIS,//DWORD clipping precision
		0,//DWORD output quality
		0,//DWORD pitch and family
		typeface);//LPCTSTR typeface name


	SelectObject(hdc,Font);

 
	return 0;
}

void Text (int x, int y, const char* pText)
{		

   	SetTextColor( hdc, FontColor);// Set text color
	SetBkMode(hdc, TRANSPARENT); //Make it transparent
	if (Font) SelectObject(hdc,Font);//Use the selected font, if any
	TextOut( hdc, x, y, pText, strlen(pText)); 

}



void Rect(int xp, int yp, int xp1, int yp1)
{
     	  MoveToEx(hdc, xp, yp, (LPPOINT) NULL);
		  LineTo(hdc,xp,yp1);
		  LineTo(hdc,xp1,yp1);
		  LineTo(hdc,xp1,yp);
		  LineTo(hdc,xp,yp);
}

          
		

void DrawBackground()
{
	int xp,yp,c;

	   DrawBitmap(0,0,MAKEINTRESOURCE(IDB_BITMAP1));

	   ZeroMemory(&hdc,sizeof(hdc));
	   lpddsBack->GetDC(&hdc);	 

       yp = (523 / 9)+20;
		
	    SetPen(0,0,0,2);
		for (c = 0; c <= 8; c++)
		{  
		  MoveToEx(hdc, 1, yp, (LPPOINT) NULL);
		  LineTo(hdc,800,yp);
          yp = yp + (523 / 9);
		}

		xp = 0;
		
		for (c = 0; c <= 8; c++)
		{  
		  MoveToEx(hdc, xp, 20, (LPPOINT) NULL);
		  LineTo(hdc,xp,543);
          xp = xp + (800 / 9);
		}


		MoveToEx(hdc, 799, 20, (LPPOINT) NULL);
	    LineTo(hdc,799,544);

		SetFont("Comic Sans MS",20,0,0,0);

			
		if (SOUND == FALSE)
		{
		 SetFontColor(120,0,0);		
		 Text(705,0,"Sound aus");
		}

	



    

	
		SetFontColor(0,0,104);
		Text(5,0,"F1 = Hilfe, F3 = Neu starten");
		SetFont("Comic Sans MS",20,1,1,0);
		Text(400-(strlen(title)*4)-4,0,title);

	
		

		Text(10,560,"Dein Punktestand: ");	
		Text(210,560,"Highscore ab : ");
		Text(390,560,"Rekord : ");
	

		SetFont("Comic Sans MS",25,0,0,0);
		SetFontColor(255,255,255);
		Text(145,556,tostring(punkte).data());

		if (punkte >= highscore)
		{
		 SetFontColor(135,0,0);
		}

		Text(320,556,tostring(highscore).data());

		SetFontColor(255,255,255);
	
	
		if (punkte >= rekord)
		{
		 SetFontColor(135,0,0);
		}

		errormsg = tostring(rekord).data();
		errormsg += " von ";
		errormsg += rname;


		Text(460,556,errormsg.data());

	
		if (debug == TRUE)
		{
		 errormsg = "Kugeln in Reihe: ";
		 errormsg += tostring(ll).data();		 
		 errormsg += "  Freie Felder: ";
	     errormsg += tostring(maxfree).data();		 	
		 Text(460,0,errormsg.data());
		}
		


		
	  
	
		

		
		GDI_Cleanup();
 
}












long zufall(long max)
{
	time_t sek;
	long  b;

	
	time(&sek);
	srand(sek);
	b = ( rand() % max );
 
 return b;
}



void Putpixel(int xp,int yp,COLORREF col)
{	
	lpddsBack->GetDC(&hdc); 	
	SetPixel(hdc,xp+x,yp+y,col);
	GDI_Cleanup();
}

void Kreis(int xp, int yp, float radius, COLORREF col)
{
		const float twoPi = 6.283f;
			  float phi;
			    int x, y;


		lpddsBack->GetDC(&hdc); 			 			
		
		for (phi = 0.0f; phi <= twoPi; phi += 0.001f)
		{
			x = (int)(cos(phi) * radius);  
			y = (int)(sin(phi) * radius);
			SetPixel(hdc,xp+x,yp+y,col);
		}

		GDI_Cleanup();
}



void Kugel(int xp,int yp, float radius,int r, int g, int b)
{
 int c;

 c = radius;
 while (c >= 1)
 {
	 Kreis(xp,yp,c,RGB(r,g,b));

	 c--;
	 if (r <= 255)
	 r = r + ((255-r) / radius);

	 if (g <= 255)
	 g = g + ((255-g) / radius);

	 if (b <= 255)
	 b = b + ((255-b) / radius);
 }

 

}



