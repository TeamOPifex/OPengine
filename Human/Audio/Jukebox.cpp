#include "Jukebox.h"

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
	result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    if(SL_RESULT_SUCCESS != result) return false;
	
    // realize the engine
	result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if(SL_RESULT_SUCCESS != result) return false;
	
    // get the engine interface, which is needed in order to create other objects
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    if(SL_RESULT_SUCCESS != result) return false;

	const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);
    if(SL_RESULT_SUCCESS != result) return false;
		
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    if(SL_RESULT_SUCCESS != result) return false;
#endif

	return true;
}

void* Jukebox::Mixer(){
#ifdef OPIFEX_ANDROID
	return &_outputMixObject;
#else
	return 0;
#endif
}