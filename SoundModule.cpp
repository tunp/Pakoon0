//
// Full-access sound module for all users of the game
// 

//#include "stdafx.h"
#include <iostream>
#include <ctime>

using namespace std;

#include "SoundModule.h"

bool SoundModule::m_bRunning = false;

/*int  SoundModule::m_chaMenuMusic = 0;
int  SoundModule::m_chaVehicleSounds = 0;
int  SoundModule::m_chaSkidSound = 0;
int  SoundModule::m_chaEngineSound1 = 0;
int  SoundModule::m_chaEngineSound2 = 0;
int  SoundModule::m_chaMessageSound = 0;
int  SoundModule::m_chaCrashSound = 0;
int  SoundModule::m_chaHeliSound = 0;
int  SoundModule::m_chaJetSound = 0;
int  SoundModule::m_cha43AASound = 0;
Mix_Chunk *SoundModule::m_pMenuMusic = 0;
Mix_Chunk *SoundModule::m_pVehicleSounds = 0;
Mix_Chunk *SoundModule::m_pEngineSound1 = 0;
Mix_Chunk *SoundModule::m_pEngineSound2 = 0;
Mix_Chunk *SoundModule::m_pMessageSound = 0;
Mix_Chunk *SoundModule::m_pSkidSound = 0;
Mix_Chunk *SoundModule::m_pCrashSound = 0;
Mix_Chunk *SoundModule::m_pHeliSound = 0;
Mix_Chunk *SoundModule::m_pJetSound = 0;
Mix_Chunk *SoundModule::m_p43AASound = 0;*/

int  SoundModule::m_nMenuMusicVolume = 128;
int  SoundModule::m_nVehicleSoundsVolume = 128;
int  SoundModule::m_nSkidSoundVolume = 0;
int  SoundModule::m_nEngineSoundVolume = 160;
int  SoundModule::m_nMessageSoundVolume = 128;
int  SoundModule::m_nHeliSoundVolume = 255;
int  SoundModule::m_nJetSoundVolume = 255;

Sound SoundModule::sMenuMusic;
Sound SoundModule::sVehicleMusic;
Sound SoundModule::sEngineSound1;
Sound SoundModule::sEngineSound2;
Sound SoundModule::sMessageSound;
Sound SoundModule::sSkidSound;
Sound SoundModule::sCrashSound;
Sound SoundModule::sHeliSound;
Sound SoundModule::sJetSound;
Sound SoundModule::s43AASound;

int  SoundModule::m_nSpace = 1;

//int SoundModule::freq = 0;

SoundModule::SoundModule() {
}

void SoundModule::Initialize() {
  /*if(FSOUND_Init(44100, 32, 0)) {
    m_bRunning = true;
  } else {
    AfxMessageBox("Cannot initialize sound system (maybe some other program is using the sound device).\nSounds will not be supported.", MB_OK);
  }
  FSOUND_3D_SetRolloffFactor(0.1f); // Make distant sounds louder
  FSOUND_Update();*/
	/*if(Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG) {
		cout << "Failed to init OGG support. " << Mix_GetError() << endl;
	} else {
		if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == 0) {
			m_bRunning = true;	
		} else {
			cout << "Cannot initialize sound system (maybe some other program is using the sound device).\nSounds will not be supported. " << Mix_GetError() << endl;
		}
	}*/
	
	SDL_AudioSpec *spec = new SDL_AudioSpec;
	spec->freq = 44100;
	spec->format = AUDIO_S16;
	spec->samples = 4096;
	spec->callback = mix;
	spec->userdata = NULL;
	spec->channels = 2;
	if (SDL_OpenAudio(spec, NULL) < 0) {
        cout << "AudioMixer, Unable to open audio: " << SDL_GetError() << endl;
    } else {
		m_bRunning = true;
		SDL_PauseAudio(0);
	}

  //int nChannels = FSOUND_GetMaxChannels();
  //int nChannels = Mix_AllocateChannels(-1);

  // Play a tiny silence file to wake up the MP3 support???
  /*
  FSOUND_SAMPLE *pTmp = FSOUND_Sample_Load(FSOUND_FREE, ".\\Sounds\\Engine2.wav", FSOUND_LOOP_OFF, 0, 0);
  if(pTmp) {
    int nTmp = FSOUND_PlaySound(FSOUND_FREE, pTmp);
    FSOUND_SetVolume(nTmp, 255);
  }
  */
}

/*void SoundModule::FreeSound(Mix_Chunk **pSound) {
  if(*pSound) {
    Mix_FreeChunk(*pSound);
    *pSound = 0;
  }
}*/

void SoundModule::Close() {
  /*FreeSound(&m_pMenuMusic);
  FreeSound(&m_pVehicleSounds);
  FreeSound(&m_pSkidSound);
  FreeSound(&m_pEngineSound1);
  // FreeSound(&m_pEngineSound2);
  FreeSound(&m_pMessageSound);
  FreeSound(&m_pCrashSound);
  FreeSound(&m_pHeliSound);
  Mix_CloseAudio();
  Mix_Quit();*/
  
  m_bRunning = false;
}

void SoundModule::SetMenuMusicVolume(int nVol) {
  m_nMenuMusicVolume = nVol;
  sMenuMusic.setVolume(m_nMenuMusicVolume);
  /*if(m_chaMenuMusic && m_bRunning) {
    //FSOUND_SetVolume(m_chaMenuMusic, m_nMenuMusicVolume);
    Mix_Volume(m_chaMenuMusic, m_nMenuMusicVolume / 2);
  }*/
}

void SoundModule::SetVehicleSoundsVolume(int nVol) {
  m_nVehicleSoundsVolume = nVol;
  //sVehicleSounds.setVolume(m_nVehicleSoundsVolume);
  /*if(m_chaVehicleSounds && m_bRunning) {
    //FSOUND_SetVolume(m_chaVehicleSounds, m_nVehicleSoundsVolume);
    Mix_Volume(m_chaVehicleSounds, m_nVehicleSoundsVolume / 2);
  }*/
}

void SoundModule::SetSkidSoundVolume(int nVol) {
  m_nSkidSoundVolume = nVol;
  sSkidSound.setVolume(int(double(m_nSkidSoundVolume) * double(m_nVehicleSoundsVolume) / 255.0));
  /*if(m_chaSkidSound && m_bRunning) {
    //FSOUND_SetVolume(m_chaSkidSound, int(double(m_nSkidSoundVolume) * double(m_nVehicleSoundsVolume) / 255.0));
    Mix_Volume(m_chaSkidSound, int(double(m_nSkidSoundVolume) * double(m_nVehicleSoundsVolume) / 255.0) / 2);
  }*/
}

void SoundModule::SetEngineSoundVolume(int nVol) {
  m_nEngineSoundVolume = nVol;
  sEngineSound1.setVolume(int(double(m_nEngineSoundVolume) * double(m_nVehicleSoundsVolume) / 255.0));
  /*if(m_chaEngineSound1 && m_bRunning) {
    //FSOUND_SetVolume(m_chaEngineSound1, int(double(m_nEngineSoundVolume) * double(m_nVehicleSoundsVolume) / 255.0));
    // FSOUND_SetVolume(m_chaEngineSound2, int(double(m_nEngineSoundVolume) * double(m_nVehicleSoundsVolume) / 255.0));
    Mix_Volume(m_chaEngineSound1, int(double(m_nEngineSoundVolume) * double(m_nVehicleSoundsVolume) / 255.0) / 2);
  }*/
}

void SoundModule::SetCrashSoundVolume(int cha, int nVol) {
  int nNewVol = int(double(nVol) * double(m_nVehicleSoundsVolume) / 255.0);
  if(nNewVol > 255) {
    nNewVol = 255;
  }
  //FSOUND_SetVolume(cha, nNewVol);
  //Mix_Volume(cha, nNewVol / 2);
  sCrashSound.setVolume(nNewVol);
}

void SoundModule::SetHeliSoundVolume(int nVol) {
  m_nHeliSoundVolume = nVol;
  //if(m_chaHeliSound && m_bRunning) {
    int nNewVol = int(double(m_nHeliSoundVolume) * double(m_nVehicleSoundsVolume) / 255.0);
    if(nNewVol > 255) {
      nNewVol = 255;
    }
    //FSOUND_SetVolume(m_chaHeliSound, nNewVol);
    //Mix_Volume(m_chaHeliSound, nNewVol / 2);
    sHeliSound.setVolume(nNewVol);
  //}
}

void SoundModule::SetJetSoundVolume(int nVol) {
  m_nJetSoundVolume = nVol;
  //if(m_chaJetSound && m_bRunning) {
    int nNewVol = int(double(m_nJetSoundVolume) * double(m_nVehicleSoundsVolume) / 255.0);
    if(nNewVol > 255) {
      nNewVol = 255;
    }
    //FSOUND_SetVolume(m_chaJetSound, nNewVol);
    //Mix_Volume(m_chaJetSound, nNewVol / 2);
    sJetSound.setVolume(nNewVol);
  //}
}

void SoundModule::SetMessageSoundVolume(int nVol) {
  m_nMessageSoundVolume = nVol;
  //if(m_chaMessageSound && m_bRunning) {
    //FSOUND_SetVolume(m_chaMessageSound, m_nMessageSoundVolume);
    //Mix_Volume(m_chaMessageSound, m_nMessageSoundVolume / 2);
    sMessageSound.setVolume(m_nMessageSoundVolume);
  //}
}


void SoundModule::SetSoundSpace(int nSpace) {
  m_nSpace = nSpace;
}

void SoundModule::SetOutput(int nOutput) {
  /*switch(nOutput) {
    case 0: 
      FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
    break;
    case 1: 
      FSOUND_SetOutput(FSOUND_OUTPUT_WINMM);
    break;
    case 2: 
      FSOUND_SetOutput(FSOUND_OUTPUT_A3D);
    break;
    case 3:  
      FSOUND_SetOutput(FSOUND_OUTPUT_NOSOUND);
    break;
  }*/
}

int SoundModule::GetOutput() {
  /*int i = FSOUND_GetOutput();
  switch(i) {
    case FSOUND_OUTPUT_DSOUND:
      return 0;
      break;
    case FSOUND_OUTPUT_WINMM:
      return 1;
      break;
    case FSOUND_OUTPUT_A3D:
      return 2;
      break;
    case FSOUND_OUTPUT_NOSOUND:
      return 3;
      break;
    default:
      return -1;
      break;
  }*/
  return -1;
}

void SoundModule::SetDriver(int nDriver) {
  //FSOUND_SetDriver(nDriver);
}

int SoundModule::GetNumDrivers() {
  //return FSOUND_GetNumDrivers();
  return -1;
}

char* SoundModule::GetDriverName(int nDriver) {
  /*if(nDriver >= 0) {
    return (char*) FSOUND_GetDriverName(nDriver);
  } else {
    return (char*) FSOUND_GetDriverName(FSOUND_GetDriver());
  }*/
  return NULL;
}



void SoundModule::StartMenuMusic() {
  if(m_bRunning) {
    if(!sMenuMusic.isLoaded()) {
      //m_pMenuMusic = FSOUND_Sample_Load(FSOUND_FREE, ".\\Sounds\\mixdown3.mp3", FSOUND_LOOP_NORMAL | FSOUND_MPEGACCURATE, 0, 0);
      //m_pMenuMusic = Mix_LoadWAV("Sounds/MixDown3.ogg");
      sMenuMusic.loadSound("Sounds/MixDown3.ogg");
      
    }
    if(sMenuMusic.isLoaded()) {
      //m_chaMenuMusic = FSOUND_PlaySound(FSOUND_FREE, m_pMenuMusic);
      //m_chaMenuMusic = Mix_PlayChannel(-1, m_pMenuMusic, -1);
      sMenuMusic.play();
      //FSOUND_SetVolume(m_chaMenuMusic, m_nMenuMusicVolume);
      //Mix_Volume(m_chaMenuMusic, m_nMenuMusicVolume / 2);
      sMenuMusic.setVolume(m_nMenuMusicVolume);
    }
  }
}

void SoundModule::StopMenuMusic() {
  if(m_bRunning) {
    //FSOUND_StopSound(m_chaMenuMusic);
    //Mix_HaltChannel(m_chaMenuMusic);
    sMenuMusic.stop();
  }
  //m_chaMenuMusic = 0;
}


void SoundModule::StartSkidSound() {
  return;
  if(m_bRunning) {
    if(!sSkidSound.isLoaded()) {
      //m_pSkidSound = FSOUND_Sample_Load(FSOUND_FREE, ".\\Sounds\\WhiteNoise.mp3", FSOUND_LOOP_NORMAL, 0, 0);
      sSkidSound.loadSound("Sounds/WhiteNoise.ogg");
    }
    if(sSkidSound.isLoaded()) {
      //m_chaSkidSound = FSOUND_PlaySound(FSOUND_FREE, m_pSkidSound);
      //m_chaSkidSound = Mix_PlayChannel(-1, m_pSkidSound, -1);
      sSkidSound.play();
      //FSOUND_SetVolume(m_chaSkidSound, m_nSkidSoundVolume);
      //Mix_Volume(m_chaSkidSound, m_nSkidSoundVolume / 2);
      sSkidSound.setVolume(m_nSkidSoundVolume);
    }
  }
}

void SoundModule::StopSkidSound() {
  return;
  if(m_bRunning) {
    //FSOUND_StopSound(m_chaSkidSound);
    //Mix_HaltChannel(m_chaSkidSound);
    sSkidSound.stop();
  }
  //m_chaSkidSound = 0;
}

void SoundModule::StartEngineSound() {
  if(m_bRunning) {
    if(!sEngineSound1.isLoaded()) {
      //m_pEngineSound1 = FSOUND_Sample_Load(FSOUND_FREE, ".\\Sounds\\EngineJuliet2.wav", FSOUND_LOOP_NORMAL | FSOUND_HW3D, 0, 0);
      //m_pEngineSound1 = Mix_LoadWAV("Sounds/EngineJuliet2.ogg");
      sEngineSound1.loadSound("Sounds/EngineJuliet2.ogg");
    }
    if(sEngineSound1.isLoaded()) {
      //m_chaEngineSound1 = FSOUND_PlaySound(FSOUND_FREE, m_pEngineSound1);
      //m_chaEngineSound1 = Mix_PlayChannel(-1, m_pEngineSound1, -1);
      sEngineSound1.play();
      // m_chaEngineSound2 = FSOUND_PlaySound(FSOUND_FREE, m_pEngineSound2);
      // SetEngineSoundVolume(160);
      SetEngineSoundVolume(255);
    }
  }
}

void SoundModule::StopEngineSound() {
  if(m_bRunning) {
    //FSOUND_StopSound(m_chaEngineSound1);
    //Mix_HaltChannel(m_chaEngineSound1);
    sEngineSound1.stop();
    // FSOUND_StopSound(m_chaEngineSound2);
  }
  //m_chaEngineSound1 = 0;
  // m_chaEngineSound2 = 0;
}

void SoundModule::SetEngineSoundRPM(int nRPM) {
  static int nFluctuation = 0;
  nFluctuation += rand() % 200 - 99;
  if(abs(nFluctuation) > 999) {
    nFluctuation = 999 * nFluctuation / 1000;
  }
  // FSOUND_SetFrequency(m_chaEngineSound, 22050 + nRPM + nFluctuation);
  // double dRatio = (double(nRPM) / 44100.0);
  // FSOUND_SetVolume(m_chaEngineSound1, int((1.0 - dRatio) * double(m_nEngineSoundVolume) * double(m_nVehicleSoundsVolume) / 255.0));
  // FSOUND_SetVolume(m_chaEngineSound2, int(dRatio * double(m_nEngineSoundVolume) * double(m_nVehicleSoundsVolume) / 255.0));
  // FSOUND_SetFrequency(m_chaEngineSound1, 44100 + int(double(nRPM) / 1.5) + nFluctuation / 2);
  // KORJAA FSOUND_SetFrequency(m_chaEngineSound1, 38000 + int(double(nRPM) * 0.8) + nFluctuation / 2);
	//Mix_UnregisterAllEffects(m_chaEngineSound1);
	//if(!Mix_RegisterEffect(m_chaEngineSound1, frequency, NULL, NULL)) {
	//	cout << "Mix_RegisterEffect: " << Mix_GetError() << endl;
	//}
  // FSOUND_SetFrequency(m_chaEngineSound1, 22050 + nRPM + nFluctuation / 2);
  // FSOUND_SetFrequency(m_chaEngineSound2, -44100 + 44100 + nRPM + nFluctuation / 2);
  sEngineSound1.setFreq(44100 + int(double(nRPM) / 1.5) + nFluctuation / 2);
}


void SoundModule::StartMessageSound() {
  if(m_bRunning) {
    if(!sMessageSound.isLoaded()) {
      //m_pMessageSound = FSOUND_Sample_Load(FSOUND_FREE, ".\\Sounds\\IncomingMessage.wav", FSOUND_LOOP_NORMAL, 0, 0);
      //m_pMessageSound = Mix_LoadWAV("Sounds/IncomingMessage.ogg");
      sMessageSound.loadSound("Sounds/IncomingMessage.ogg");
    }
    if(sMessageSound.isLoaded()) {
      //m_chaMessageSound = FSOUND_PlaySound(FSOUND_FREE, m_pMessageSound);
      //m_chaMessageSound = Mix_PlayChannel(-1, m_pMessageSound, -1);
      sMessageSound.play();
      SetMessageSoundVolume(128);
    }
  }
}

void SoundModule::StopMessageSound() {
  if(m_bRunning) {
    //FSOUND_StopSound(m_chaMessageSound);
    //Mix_HaltChannel(m_chaMessageSound);
    sMessageSound.stop();
  }
  //m_chaMessageSound = 0;
}


void SoundModule::PreCache43AASound() {
  /*if(m_bRunning) {
    if(!m_p43AASound) {
      //m_p43AASound = FSOUND_Sample_Load(FSOUND_FREE, ".\\Sounds\\43aa.mp3", FSOUND_LOOP_OFF, 0, 0);
      m_p43AASound = Mix_LoadWAV("Sounds/43aa.ogg");
    }
    if(m_p43AASound) {
      //m_cha43AASound = FSOUND_PlaySound(FSOUND_FREE, m_p43AASound);
      m_cha43AASound = Mix_PlayChannel(-1, m_p43AASound, 0);
      //FSOUND_SetVolume(m_cha43AASound, 0);
      Mix_Volume(m_cha43AASound, 0);
    }
  }*/
}

void SoundModule::Start43AASound() {
  if(m_bRunning) {
    if(!s43AASound.isLoaded()) {
      //m_p43AASound = FSOUND_Sample_Load(FSOUND_FREE, ".\\Sounds\\43aa.mp3", FSOUND_LOOP_OFF | FSOUND_MPEGACCURATE, 0, 0);
      //m_p43AASound = Mix_LoadWAV("Sounds/43aa.ogg");
      s43AASound.loadSound("Sounds/43aa.ogg");
    }
    if(s43AASound.isLoaded()) {
      //m_cha43AASound = FSOUND_PlaySound(FSOUND_FREE, m_p43AASound);
      //m_cha43AASound = Mix_PlayChannel(-1, m_p43AASound, 0);
      s43AASound.play();
      //FSOUND_SetVolume(m_cha43AASound, 255);
      //Mix_Volume(m_cha43AASound, MIX_MAX_VOLUME);
      s43AASound.setVolume(255);
    }
  }
}

void SoundModule::Stop43AASound() {
  if(m_bRunning) {
    //FSOUND_StopSound(m_cha43AASound);
    //Mix_HaltChannel(m_cha43AASound);
    s43AASound.stop();
    //FreeSound(&m_p43AASound);
    //Mix_FreeChunk(m_p43AASound);
    s43AASound.unloadSound();
  }
  //m_cha43AASound = 0;
}


void SoundModule::StartHeliSound() {
  if(m_bRunning) {
    if(!sHeliSound.isLoaded()) {
      //m_pHeliSound = FSOUND_Sample_Load(FSOUND_FREE, ".\\Sounds\\Huey.wav", FSOUND_LOOP_NORMAL | FSOUND_HW3D, 0, 0);
      //m_pHeliSound = Mix_LoadWAV("Sounds/huey.ogg");
      sHeliSound.loadSound("Sounds/huey.ogg");
    }
    if(sHeliSound.isLoaded()) {
      //m_chaHeliSound = FSOUND_PlaySound(FSOUND_FREE, m_pHeliSound);
      //m_chaHeliSound = Mix_PlayChannel(-1, m_pHeliSound, 0);
      sHeliSound.play();
      SetHeliSoundVolume(m_nHeliSoundVolume);
    }
  }
}

void SoundModule::StopHeliSound() {
  if(m_bRunning) {
    //FSOUND_StopSound(m_chaHeliSound);
    //Mix_HaltChannel(m_chaHeliSound);
    sHeliSound.stop();
  }
  //m_chaHeliSound = 0;
}

void SoundModule::SetHeliSoundPhase(double dPhase, double dBladePower) {
  SetHeliSoundVolume(int(dPhase * 255.0 * dBladePower));
  //FSOUND_SetFrequency(m_chaHeliSound, int(44100.0 * dPhase));
  sHeliSound.setFreq(int(44100.0 * dPhase));
}


void SoundModule::StartJetSound() {
  if(m_bRunning) {
    if(!sJetSound.isLoaded()) {
      //m_pJetSound = FSOUND_Sample_Load(FSOUND_FREE, ".\\Sounds\\JetMono.wav", FSOUND_LOOP_NORMAL | FSOUND_HW3D, 0, 0);
      //m_pJetSound = Mix_LoadWAV("Sounds/JetMono.ogg");
      sJetSound.loadSound("Sounds/JetMono.ogg");
    }
    if(sJetSound.isLoaded()) {
      //m_chaJetSound = FSOUND_PlaySound(FSOUND_FREE, m_pJetSound);
      //m_chaJetSound = Mix_PlayChannel(-1, m_pJetSound, -1);
      sJetSound.play();
      SetJetSoundVolume(255);
    }
  }
}

void SoundModule::StopJetSound() {
  if(m_bRunning) {
    //FSOUND_StopSound(m_chaJetSound);
    //Mix_HaltChannel(m_chaJetSound);
    sJetSound.stop();
  }
  //m_chaJetSound = 0;
}

void SoundModule::SetJetSoundPhase(double dPhase) {
  // SetJetSoundVolume(128 + int(dPhase * 127.0));
  //FSOUND_SetFrequency(m_chaJetSound, 44100 + int(20000.0 * dPhase));
  sJetSound.setFreq(44100 + int(20000.0 * dPhase));
}



void SoundModule::PlayCrashSound(double dVolume) {
  static clock_t clockPrev = clock();
  clock_t clockNow = clock();
  if(m_bRunning && 
     ((double(clockNow - clockPrev) / double(CLOCKS_PER_SEC)) > 0.02) && 
     (dVolume > 0.1)) {
    if(dVolume > 1.0) {
      dVolume = 1.0;
    }
    clockPrev = clockNow;
    if(!sCrashSound.isLoaded()) {
      //m_pCrashSound = FSOUND_Sample_Load(FSOUND_FREE, ".\\Sounds\\crash.wav", FSOUND_LOOP_OFF | FSOUND_HW3D, 0, 0);
      //m_pCrashSound = Mix_LoadWAV("Sounds/crash.ogg");
      sCrashSound.loadSound("Sounds/crash.ogg");
      sCrashSound.setLoop(false);
    }
    if(sCrashSound.isLoaded()) {
      //m_chaCrashSound = FSOUND_PlaySound(FSOUND_FREE, m_pCrashSound);
      //m_chaCrashSound = Mix_PlayChannel(-1, m_pCrashSound, 0);
      //DOES NOT USE CHANNELS NOW
      sCrashSound.play();
      //FSOUND_SetFrequency(m_chaCrashSound, 22050 + rand() % 10000);
      //SetCrashSoundVolume(m_chaCrashSound, 55 + int(dVolume * 200.0));
    }
  }
}




void SoundModule::Update3DSounds(const BVector& rvSouLoc, 
                                 const BVector& rvSouVel, 
                                 const BVector& rvLisLoc, 
                                 const BOrientation& roLis, 
                                 const BVector& rvLisVel) {
/*  static float fSouLoc[3];
  static float fSouVel[3];
  static float fLisLoc[3];
  static float fLisVel[3];
  if(m_nSpace == 1) {
    // update 3D sound location
    fSouLoc[0] = float(rvSouLoc.m_dX);
    fSouLoc[1] = float(-rvSouLoc.m_dZ);
    fSouLoc[2] = float(rvSouLoc.m_dY);
    fSouVel[0] = float(rvSouVel.m_dX);
    fSouVel[1] = float(-rvSouVel.m_dZ);
    fSouVel[2] = float(rvSouVel.m_dY);

    FSOUND_3D_SetAttributes(
      m_chaEngineSound1, 
      fSouLoc, 
      fSouVel);*/
    /*
    FSOUND_3D_SetAttributes(
      m_chaEngineSound2, 
      fSouLoc, 
      fSouVel);
    */
    /*if(m_chaCrashSound != 0) {
      FSOUND_3D_SetAttributes(
        m_chaCrashSound, 
        fSouLoc, 
        fSouVel);
    }
    if(m_chaHeliSound != 0) {
      FSOUND_3D_SetAttributes(
        m_chaHeliSound, 
        fSouLoc, 
        fSouVel);
    }
    if(m_chaJetSound != 0) {
      FSOUND_3D_SetAttributes(
        m_chaJetSound, 
        fSouLoc, 
        fSouVel);
    }

    fLisLoc[0] = float(rvLisLoc.m_dX);
    fLisLoc[1] = float(-rvLisLoc.m_dZ);
    fLisLoc[2] = float(rvLisLoc.m_dY);
    fLisVel[0] = float(rvLisVel.m_dX);
    fLisVel[1] = float(-rvLisVel.m_dZ);
    fLisVel[2] = float(rvLisVel.m_dY);

    FSOUND_3D_Listener_SetAttributes(
      fLisLoc,
      fLisVel,
      float(roLis.m_vForward.m_dX),
      float(-roLis.m_vForward.m_dZ),
      float(roLis.m_vForward.m_dY),
      float(roLis.m_vUp.m_dX),
      float(-roLis.m_vUp.m_dZ),
      float(roLis.m_vUp.m_dY));

    FSOUND_Update();
  }*/
}

/*void SoundModule::frequency(int chan, void *stream, int len, void *udata) {
	double rate = 44100.0 / freq;
	for (int x = 0; x < len / 2 && (int)(x * rate) < len / 2; x++) {
		int pos = x * rate;
		((unsigned short *)stream)[pos] = ((unsigned short *)stream)[x];
	}
}*/

void SoundModule::mix(void *userdata, Uint8 *stream, int len) {
	vector<Sound *> sounds;
	
	sounds.push_back(&sMenuMusic);
	sounds.push_back(&sVehicleMusic);
	sounds.push_back(&sEngineSound1);
	sounds.push_back(&sEngineSound2);
	sounds.push_back(&sMessageSound);
	sounds.push_back(&sSkidSound);
	sounds.push_back(&sCrashSound);
	sounds.push_back(&sHeliSound);
	sounds.push_back(&sJetSound);
	sounds.push_back(&s43AASound);
	
	for (int y = 0; y < sounds.size(); y++) {
		char *samples = new char[len];
		sounds[y]->getSamples(samples, len);
		Sint16 *p_stream = (Sint16 *)stream;
		Sint16 *p_samples = (Sint16 *)samples;
		for (int x = 0; x < len; x += 2) {
			Sint32 val = (Sint32) *p_stream + *p_samples;
			if (val > 32767) {
				val = 32767;
			} else if (val < -32766) {
				val = -32766;
			}
			//*p_stream += *p_samples;
			*p_stream = val;
			p_stream++;
			p_samples++;
		}
	}
}
