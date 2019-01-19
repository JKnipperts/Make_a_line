bool DSoundInit (HWND hwnd)
{
   if(DirectSoundCreate (NULL, &lpDirectSound, NULL) != DS_OK)
   {
      return (false);
   }

   if(lpDirectSound->SetCooperativeLevel (hwnd, DSSCL_NORMAL) != DS_OK)
   {
      return (false);
   }

   return (true);
}


void ReleaseDSound ()
{
   if (dsbSound != NULL)
   {
      dsbSound->Release();
      dsbSound = NULL;
   }

   if (lpDirectSound != NULL)
   {
      lpDirectSound->Release();
      lpDirectSound = NULL;
   }
}



bool LoadWave (char *filename)
{
   HMMIO wavefile;


 
   wavefile = mmioOpen (filename, 0, MMIO_READ | MMIO_ALLOCBUF);

   if (wavefile == NULL)
   {
      return (false);
   }

   MMCKINFO parent;
   memset (&parent, 0, sizeof(MMCKINFO));
   parent.fccType = mmioFOURCC ('W', 'A', 'V', 'E');
   mmioDescend (wavefile, &parent, 0, MMIO_FINDRIFF);

   MMCKINFO child;
   memset (&child, 0, sizeof(MMCKINFO));
   child.fccType = mmioFOURCC ('f', 'm', 't', ' ');
   mmioDescend (wavefile, &child, &parent, 0);

   WAVEFORMATEX wavefmt;
   mmioRead (wavefile, (char*)&wavefmt, sizeof(wavefmt));

   if(wavefmt.wFormatTag != WAVE_FORMAT_PCM)
   {
      return (false);
   }

   mmioAscend (wavefile, &child, 0);
   child.ckid = mmioFOURCC ('d', 'a', 't', 'a');
   mmioDescend (wavefile, &child, &parent, MMIO_FINDCHUNK);

  DSBUFFERDESC bufdesc;
   memset (&bufdesc, 0, sizeof(DSBUFFERDESC));
   bufdesc.dwSize = sizeof(DSBUFFERDESC);

   bufdesc.dwFlags = 0;
   bufdesc.dwBufferBytes = child.cksize;
   bufdesc.lpwfxFormat = &wavefmt;

   if((lpDirectSound->CreateSoundBuffer (&bufdesc, &dsbSound, NULL))    != DS_OK)
   {
      return (false);
   }



   void *write1 = 0, *write2 = 0;
   unsigned long length1,length2;
   dsbSound->Lock (0, child.cksize, &write1, &length1, &write2, &length2, 0);
   if(write1 > 0)
   mmioRead (wavefile, (char*)write1, length1);
   if (write2 > 0)
   mmioRead (wavefile, (char*)write2, length2);
   dsbSound->Unlock (write1, length1, write2, length2);

   mmioClose (wavefile, 0);

   return (true);
}


void PlayWave ()
{
   dsbSound->SetCurrentPosition (0);
   dsbSound->Play (0, 0, 0);
}


void StopWave ()
{
   dsbSound->Stop ();
}
