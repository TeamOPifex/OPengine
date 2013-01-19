#include "Jukebox.h"

#include <stdlib.h>

#include "./Core/include/Log.h"

#ifdef OPIFEX_ANDROID
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

SLObjectItf Jukebox::_engineObject = NULL;
SLEngineItf Jukebox::_engineEngine = NULL;
SLObjectItf Jukebox::_outputMixObject = NULL;

#else
// OpenAL stuff here
#endif

Jukebox::Jukebox(){

}

bool Jukebox::Initialize() {
#ifdef OPIFEX_ANDROID
	SLresult result;
	
    // create engine
	result = slCreateEngine(&_engineObject, 0, NULL, 0, NULL, NULL);
    if(SL_RESULT_SUCCESS != result) {
		OPLog("Jukebox::Error 1");
		return false;
	}
	
    // realize the engine
	result = (*_engineObject)->Realize(_engineObject, SL_BOOLEAN_FALSE);
    if(SL_RESULT_SUCCESS != result) {
		OPLog("Jukebox::Error 2");
		return false;
	}
	
    // get the engine interface, which is needed in order to create other objects
    result = (*_engineObject)->GetInterface(_engineObject, SL_IID_ENGINE, &_engineEngine);
    if(SL_RESULT_SUCCESS != result) {
		OPLog("Jukebox::Error 3");
		return false;
	}

	// create output mix, with environmental reverb specified as a non-required interface    
	const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
    result = (*_engineEngine)->CreateOutputMix(_engineEngine, &_outputMixObject, 1, ids, req);
    if(SL_RESULT_SUCCESS != result) {
		OPLog("Jukebox::Error 4");
		//return false;
	}
		
    // realize the output mix
    result = (*_outputMixObject)->Realize(_outputMixObject, SL_BOOLEAN_FALSE);
    if(SL_RESULT_SUCCESS != result) {
		OPLog("Jukebox::Error 5");
		return false;
	}
	
	OPLog("Jukebox::Initialized");
#endif

	return true;
}

SLObjectItf Jukebox::Mixer(){
#ifdef OPIFEX_ANDROID
	return _outputMixObject;
#else
	return 0;
#endif
}

SLEngineItf Jukebox::Engine(){
#ifdef OPIFEX_ANDROID
	return _engineEngine;
#else
	return 0;
#endif
}

Audio* Jukebox::Load(char* filename, bool loop)
{
	return new Audio(filename, loop);
}

void Jukebox::Stop(Audio* audio){
	audio->Stop();
}

void Jukebox::Pause(Audio* audio){
	audio->Pause();
}

void Jukebox::Play(Audio* audio){
	audio->Play();
}