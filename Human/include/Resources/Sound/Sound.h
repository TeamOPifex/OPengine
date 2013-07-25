#pragma once

#ifdef OPIFEX_ANDROID
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#else
// OpenAL stuff here
#endif
#include <stdio.h>
#include "./Core/include/Log.h"

class Sound {
protected:
	ui32 check;
public:
	void assert(bool result){
		check++;
		if(!result){
			OPLog("Had an Error");
			OPLog_i32(check);
		}
	}
	
#ifdef OPIFEX_ANDROID

#endif

	Sound(int fd, long start, long length){
		
#ifdef OPIFEX_ANDROID
	SLresult result;
	SLObjectItf outputMixObject = NULL;
	SLObjectItf engineObject = NULL;
	SLEngineItf engineEngine = NULL;
	SLObjectItf fdPlayerObject = NULL;

	SLPlayItf fdPlayerPlay;
	SLSeekItf fdPlayerSeek;
	SLMuteSoloItf fdPlayerMuteSolo;
	SLVolumeItf fdPlayerVolume;
		
    // create engine
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    assert(SL_RESULT_SUCCESS == result);
	
    // realize the engine
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);

    // get the engine interface, which is needed in order to create other objects
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    assert(SL_RESULT_SUCCESS == result);
	
    // create output mix, with environmental reverb specified as a non-required interface
    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);
    assert(SL_RESULT_SUCCESS == result);

    // realize the output mix
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);


		// configure audio source
		SLDataLocator_AndroidFD loc_fd = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
		SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
		SLDataSource audioSrc = {&loc_fd, &format_mime};
		
		// configure audio sink
		SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
		SLDataSink audioSnk = {&loc_outmix, NULL};

		// create audio player
		const SLInterfaceID ids2[3] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME};
		const SLboolean req2[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
		result = (*engineEngine)->CreateAudioPlayer(engineEngine, &fdPlayerObject, &audioSrc, &audioSnk,
				3, ids2, req2);
		assert(SL_RESULT_SUCCESS == result);
		

		// realize the player
		result = (*fdPlayerObject)->Realize(fdPlayerObject, SL_BOOLEAN_FALSE);
		assert(SL_RESULT_SUCCESS == result);

		// get the play interface
		result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_PLAY, &fdPlayerPlay);
		assert(SL_RESULT_SUCCESS == result);

		// get the seek interface
		result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_SEEK, &fdPlayerSeek);
		assert(SL_RESULT_SUCCESS == result);

		// get the mute/solo interface
		result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_MUTESOLO, &fdPlayerMuteSolo);
		assert(SL_RESULT_SUCCESS == result);

		// get the volume interface
		result = (*fdPlayerObject)->GetInterface(fdPlayerObject, SL_IID_VOLUME, &fdPlayerVolume);
		assert(SL_RESULT_SUCCESS == result);

		// enable whole file looping
		result = (*fdPlayerSeek)->SetLoop(fdPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
		assert(SL_RESULT_SUCCESS == result);
		
		result = (*fdPlayerPlay)->SetPlayState(fdPlayerPlay, SL_PLAYSTATE_PLAYING);
        assert(SL_RESULT_SUCCESS == result);

		OPLog("Sound Initialized");
#endif
	}
};