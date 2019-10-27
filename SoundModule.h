//
// All access sound module for all users of the game
// 

//#include "fmod.h"
#include <SDL2/SDL.h>
//#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

using namespace std;

#include "BaseClasses.h"

class Sound {
private:
	int size;
	char *data;
	int volume;
	bool loaded;
	bool playing;
	bool paused;
	int play_pos;
	int channels;
	int play_freq;
	bool loop;
public:
	Sound() {
		loaded = false;
		playing = false;
		paused = false;
		volume = 128;
		play_pos = 0;
		play_freq = 44100;
		loop = true;
		data = 0;
	}
	
	Sound(const Sound &old) {
		size = old.size;
		volume = old.volume;
		loaded = old.loaded;
		playing = old.playing;
		paused = old.paused;
		play_pos = old.play_pos;
		channels = old.channels;
		play_freq = old.play_freq;
		loop = old.loop;
		if (loaded) {
			data = new char[size];
			memcpy(data, old.data, size);
		} else {
			data = 0;
		}
	}
	
	~Sound() {
		unloadSound();
	}

	void loadSound(string file) {
		/*ifstream is(file.c_str());
		if (is) {
			is.seekg(0, is.end);
			size = is.tellg();
			is.seekg(0, is.beg);
			data = new char[size];
			is.read(data, size);
			if (is) {
				loaded = true;
			}
		}
		is.close();*/
		
		FILE *f = fopen(file.c_str(), "rb");
		OggVorbis_File vf;
		/*#ifdef _WIN32
		  _setmode( _fileno( stdin ), _O_BINARY );
		  _setmode( _fileno( stdout ), _O_BINARY );
		#endif*/
		
		if(ov_open_callbacks(f, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0) {
			cout << "Input does not appear to be an Ogg bitstream. " << file << endl;
		} else {
			char **ptr = ov_comment(&vf,-1)->user_comments;
			vorbis_info *vi = ov_info(&vf,-1);
			while (*ptr) {
				cout << *ptr << endl;
				++ptr;
			}
			cout << vi->channels << "channels, " << vi->rate << "Hz rate" << endl;
			cout << (long)ov_pcm_total(&vf, -1) << "samples" << endl;
			cout << ov_comment(&vf, -1)->vendor << endl;
			
			channels = vi->channels;
			size = ov_pcm_total(&vf, -1) * 2 * channels;
			data = new char[size];
			
			int current_section;
			int pos = 0;
			bool eof = false;
			while (!eof) {
				long ret = ov_read(&vf, data + pos, size, 0, 2, 1, &current_section);
				if (ret == 0) {
					eof = true;
				} else if (ret < 0) {
					cout << "error in the stream" << endl;
				} else {
					pos += ret;
				}
			}
			
			if (pos == size) {
				SDL_AudioCVT  cvt;
				int ret = SDL_BuildAudioCVT(&cvt, AUDIO_S16LSB, channels, 44100, AUDIO_S16LSB, 2, 44100);
				if (ret == -1) {
					cout << "Audio convert failed" << endl;
				} else {
					cout << cvt.len_mult << endl;
					cvt.buf = (Uint8 *)new char[size * cvt.len_mult];
					cvt.len = size;
					memcpy(cvt.buf, data, size);
					delete[] data;
					SDL_ConvertAudio(&cvt);
					size = cvt.len_cvt;
					data = (char *)cvt.buf;
					
					loaded = true;
				}
			}
		}
		
		fclose(f);
	}
	
	void unloadSound() {
		if (loaded) {
			delete[] data;
			loaded = false;
			playing = false;
		}
	}
	
	void setVolume(int volume) {
		this->volume = volume;
	}
	
	bool isLoaded() {
		return loaded;
	}
	
	bool isPlaying() {
		return playing;
	}
	
	void play() {
		if (!playing) {
			playing = true;
			if (!paused) {
				play_pos = 0;
			}
			paused = false;
		}
	}
	
	void stop() {
		playing = false;
	}
	
	void pause() {
		playing = false;
		paused = true;
	}
	
	void setFreq(int play_freq) {
		this->play_freq = play_freq;
	}
	
	void setLoop(bool loop) {
		this->loop = loop;
	}
	
	void getSamples(char *out_data, int count) {
		count /= 4;
		int size = this->size / 4;
		double length_multi = (double)play_freq / 44100.0;
		int real_count = count * length_multi;
		Sint32 *val_data = new Sint32[real_count];
		memset(val_data, 0, real_count * 4);
		memset(out_data, 0, count * 4);
		
		if (loaded && playing) {
			if ((play_pos / 4) + real_count > size) {
				int piece1 = size - (play_pos / 4);
				int piece2 = real_count - piece1;
				memmove(val_data, data + play_pos, piece1 * 4);
				if (loop) {
					memmove(val_data + piece1, data, piece2 * 4);
					play_pos = piece2 * 4;
				} else {
					memset(val_data + piece1, 0, piece2 * 4);
					playing = false;
				}
			} else {
				memmove(val_data, data + play_pos, real_count * 4);
				play_pos += real_count * 4;
			}
			
			for (int x = 0; x < count; x++) {
				int val_data_pos = x * length_multi;
				((Sint32 *)out_data)[x] = val_data[val_data_pos];
				((Sint16 *)out_data)[x * 2] *= ((double)volume / 255);
				((Sint16 *)out_data)[x * 2 + 1] *= ((double)volume / 255);
			}
		}
		
		delete[] val_data;
	}
};

class SoundModule {
  static bool m_bRunning;
  
	static Sound sMenuMusic;
	static Sound sVehicleSounds;
	static Sound sEngineSound1;
	static Sound sEngineSound2;
	static Sound sMessageSound;
	static Sound sSkidSound;
	static Sound sCrashSound;
	static Sound sHeliSound;
	static Sound sJetSound;
	static Sound s43AASound;

  /*static int m_chaMenuMusic;
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
  static Mix_Chunk *m_p43AASound;*/

  static int m_nMenuMusicVolume;
  static int m_nVehicleSoundsVolume;
  static int m_nSkidSoundVolume;
  static int m_nEngineSoundVolume;
  static int m_nMessageSoundVolume;
  static int m_nHeliSoundVolume;
  static int m_nJetSoundVolume;
  static int m_nSpace;
  
  static SDL_mutex *mix_mutex;
  static vector<Sound *> playPool;
  static vector<Sound *> sounds;

  //static void FreeSound(Mix_Chunk **pSound);

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
  static void SetCrashSoundVolume(int nVol);

  static void Initialize();
  static void Close();

  static void Update3DSounds(const BVector& rvSouLoc, 
                             const BVector& rvSouVel, 
                             const BVector& rvLisLoc, 
                             const BOrientation& roLis, 
                             const BVector& rvLisVel);
  //static void frequency(int chan, void *stream, int len, void *udata);
  
  static void mix(void *userdata, Uint8 *stream, int len);
  static void mixOne(Sound *sound, Uint8 *stream, int len);
  static void playOnSoundPool(Sound &sound);
};
