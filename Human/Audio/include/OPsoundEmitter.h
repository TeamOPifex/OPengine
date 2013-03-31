#pragma once
#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
#include "../include/al.h"
#include "../include/alc.h"
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
		bool Looping;

		OPSoundEmitter(OPsound* sound, int bytesPerBuffer);
		~OPSoundEmitter();
/*---------------------------------------------------------------------------*/
		long Length(){ return _sound->DataSize; }
		void Play();
		void Pause();
		void Stop();
		void Update();
		void SetSound(OPsound* sound);
/*---------------------------------------------------------------------------*/
		void SetPosition(ALfloat position[]){
			alSourcefv(_alSrc, AL_POSITION, position);
		}
		void SetVelocity(ALfloat velocity[]){
			alSourcefv(_alSrc, AL_VELOCITY, velocity);
		}
		void SetGain(float gain){
			alSourcef(_alSrc, AL_GAIN, gain);
		}
		void SetPitch(float pitch){
			alSourcef(_alSrc, AL_PITCH, pitch);
		}
#pragma endregion

	private:
		OPsound* _sound;
		ALuint _buffers[BUFFERS], _alSrc;
		unsigned char* _intermediateBuffer;

		int _activeBuffer, _freeBuffers;
		int _queued, _chunksProcessed, _oldBuffsPlayed;
		int _bufferSize, _chunkSize;
		int _bytesInBuffer, _bytesPlayed, _oldBytesPlayed;
		OPsoundEmitterState _state;

		int process();

};
