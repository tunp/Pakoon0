//
// All access sound module for all users of the game
// 

//#include "fmod.h"
#include <SDL/SDL_mixer.h>
#include "BaseClasses.h"

class SoundModule {
  static bool m_bRunning;

  static int m_chaMenuMusic;
  static Mix_Chunk *m_pMenuMusic;
  static int m_chaVehicleSounds;
  static Mix_Chunk *m_pVehicleSounds;
  static int m_chaSkidSound;
  static Mix_Chunk *m_pSkidSound;
  static int m_chaEngineSound1;
  static int m_chaEngineSound2;
  static Mix_Chunk *m_pEngineSound1;
  static Mix_Chunk *m_pEngineSound2;
  static int m_chaMessageSound;
  static Mix_Chunk *m_pMessageSound;
  static int m_chaCrashSound;
  static Mix_Chunk *m_pCrashSound;
  static int m_chaHeliSound;
  static Mix_Chunk *m_pHeliSound;
  static int m_chaJetSound;
  static Mix_Chunk *m_pJetSound;
  static int m_cha43AASound;
  static Mix_Chunk *m_p43AASound;

  static int m_nMenuMusicVolume;
  static int m_nVehicleSoundsVolume;
  static int m_nSkidSoundVolume;
  static int m_nEngineSoundVolume;
  static int m_nMessageSoundVolume;
  static int m_nHeliSoundVolume;
  static int m_nJetSoundVolume;
  static int m_nSpace;

  static void FreeSound(Mix_Chunk **pSound);
  
  static int freq;

public:

  SoundModule();

  static int  GetMenuMusicVolume() {return m_nMenuMusicVolume;}
  static void SetMenuMusicVolume(int nVol);
  static int  GetVehicleSoundsVolume() {return m_nVehicleSoundsVolume;}
  static void SetVehicleSoundsVolume(int nVol);

  static int  GetSkidSoundVolume() {return m_nSkidSoundVolume;}
  static void SetSkidSoundVolume(int nVol);

  static int  GetEngineSoundVolume() {return m_nEngineSoundVolume;}
  static void SetEngineSoundVolume(int nVol);
  static void SetEngineSoundRPM(int nRPM);

  static int  GetMessageSoundVolume() {return m_nMessageSoundVolume;}
  static void SetMessageSoundVolume(int nVol);

  static int  GetSoundSpace() {return m_nSpace;}
  static void SetSoundSpace(int nSpace);

  static void SetOutput(int nOutput);
  static int  GetOutput();
  static void SetDriver(int nDriver);

  static int   GetNumDrivers();
  static char* GetDriverName(int nDriver);

  static void StartMenuMusic();
  static void StopMenuMusic();

  static void StartSkidSound();
  static void StopSkidSound();

  static void StartEngineSound();
  static void StopEngineSound();

  static void StartMessageSound();
  static void StopMessageSound();

  static void PreCache43AASound();
  static void Start43AASound();
  static void Stop43AASound();

  static void StartHeliSound();
  static void SetHeliSoundVolume(int nVol);
  static void SetHeliSoundPhase(double dPhase, double dBladePower);
  static void StopHeliSound();

  static void StartJetSound();
  static void SetJetSoundVolume(int nVol);
  static void SetJetSoundPhase(double dPhase);
  static void StopJetSound();

  static void PlayCrashSound(double dVolume);
  static void SetCrashSoundVolume(int cha, int nVol);

  static void Initialize();
  static void Close();

  static void Update3DSounds(const BVector& rvSouLoc, 
                             const BVector& rvSouVel, 
                             const BVector& rvLisLoc, 
                             const BOrientation& roLis, 
                             const BVector& rvLisVel);
  static void frequency(int chan, void *stream, int len, void *udata);
};
