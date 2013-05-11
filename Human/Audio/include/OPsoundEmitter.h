#pragma once
#ifdef OPIFEX_ANDROID
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
#include <AL/al.h>
#include <AL/alc.h>
#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include "OPaudio.h"

#define CHUNKS 10
#define BUFFERS 5

enum OPsoundEmitterState{
	Playing = 0,
	Stopped = 1,
	Paused = 2
};

class OPSoundEmitter{
	public:
		OPint Looping;

		OPSoundEmitter(OPsound* sound, OPint sections);
		~OPSoundEmitter();
/*---------------------------------------------------------------------------*/
		i64 Length(){ return _sound->DataSize; }
		void Play();
		void Pause();
		void Stop();
		void Update();
		void SetSound(OPsound* sound);
/*---------------------------------------------------------------------------*/
		void SetPosition(Vector3 position){
			alSourcefv(_alSrc, AL_POSITION, position.ptr());
		}
		void SetVelocity(Vector3 velocity){
			alSourcefv(_alSrc, AL_VELOCITY, velocity.ptr());
		}
		void SetVolume(OPfloat gain){
			alSourcef(_alSrc, AL_GAIN, gain);
		}
		void SetPitch(OPfloat pitch){
			alSourcef(_alSrc, AL_PITCH, pitch);
		}
#pragma endregion

	private:
		OPsound* _sound;
#ifdef OPIFEX_ANDROID // openSL ES for android

#else // openAL for desktops
		ALuint _buffers[BUFFERS], _alSrc;
#endif
		ui8* _intermediateBuffer;

		OPint _activeBuffer, _freeBuffers;
		OPint _queued, _chunksProcessed, _oldBuffsPlayed;
		OPint _bufferSize, _chunkSize;
		OPint _bytesInBuffer, _bytesPlayed, _oldBytesPlayed;
		OPsoundEmitterState _state;

		OPint process();

};
