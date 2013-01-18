#pragma once

#include "Audio.h"

class Jukebox {
public:
	Jukebox();
	static bool Initialize();
	static Audio* Play(char* filename, bool loop);
	static void Stop(Audio* audio);
	static void Pause(Audio* audio);
	static void Play(Audio* audio);
	static void* Mixer();

	// Once vector is implemented
	//void StopAll();
	//void Update();
private:
#ifdef OPIFEX_ANDROID
	static SLObjectItf _engineObject;
	static SLEngineItf _engineEngine;
	static SLObjectItf _outputMixObject;
#endif
};