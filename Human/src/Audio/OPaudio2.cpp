#include "./Human/include/Audio/OPaudio2.h"

#define OPAUDIO_SWAP(s1, s2){\
	i16 sTemp = s1;\
	s1 = s2;\
	s2 = sTemp;\
}\

//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
#ifdef OPIFEX_ANDROID
SLObjectItf SLES_engineObject;
SLEngineItf SLES_engineEngine;
SLObjectItf SLES_outputMixObject;
#else  
ALCdevice*  AL_OPaudioDevice;
ALCcontext* AL_OPaudioContext;
#endif
LPOVCLEAR           fn_ov_clear;
LPOVREAD            fn_ov_read;
LPOVPCMTOTAL        fn_ov_pcm_total;
LPOVINFO            fn_ov_info;
LPOVCOMMENT         fn_ov_comment;
LPOVOPENCALLBACKS   fn_ov_open_callbacks;
//-----------------------------------------------------------------------------
//   ____  _____                _ _         ______                _   _                 
//  / __ \|  __ \              | (_)       |  ____|              | | (_)                
// | |  | | |__) |_ _ _   _  __| |_  ___   | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
// | |  | |  ___/ _` | | | |/ _` | |/ _ \  |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | |__| | |  | (_| | |_| | (_| | | (_) | | |  | |_| | | | | (__| |_| | (_) | | | \__ \
//  \____/|_|   \__,_|\__,_|\__,_|_|\___/  |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPint OPaudInit(){
	OPLog("Initializing OP audio...\n");
// USE OGG VORBIS FOR DESKTOP PLATFORMS
#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	HINSTANCE _hVorbisFileDLL = LoadLibrary("vorbisfile.dll");
	if(_hVorbisFileDLL){
		fn_ov_clear = (LPOVCLEAR)GetProcAddress(_hVorbisFileDLL, "ov_clear");
		fn_ov_read = (LPOVREAD)GetProcAddress(_hVorbisFileDLL, "ov_read");
		fn_ov_pcm_total = (LPOVPCMTOTAL)GetProcAddress(_hVorbisFileDLL, "ov_pcm_total");
		fn_ov_info = (LPOVINFO)GetProcAddress(_hVorbisFileDLL, "ov_info");
		fn_ov_comment = (LPOVCOMMENT)GetProcAddress(_hVorbisFileDLL, "ov_comment");
		fn_ov_open_callbacks = (LPOVOPENCALLBACKS)GetProcAddress(_hVorbisFileDLL, "ov_open_callbacks");

		if (!(fn_ov_clear && fn_ov_read && fn_ov_pcm_total && fn_ov_info &&
			fn_ov_comment && fn_ov_open_callbacks)){
			return -2;
		}
	}
#else
	// copy the function pointers directly from
	// the linked SO file.
	fn_ov_clear = (LPOVCLEAR)ov_clear;
	fn_ov_read = (LPOVREAD)ov_read;
	fn_ov_pcm_total = (LPOVPCMTOTAL)ov_pcm_total;
	fn_ov_info = (LPOVINFO)ov_info;
	fn_ov_comment = (LPOVCOMMENT)ov_comment;
	fn_ov_open_callbacks = (LPOVOPENCALLBACKS)ov_open_callbacks;
#endif

#ifdef OPIFEX_ANDROID // USE OpenSL ES for sound
	//   _____ _      ______  _____      
	//  / ____| |    |  ____|/ ____|     
	// | (___ | |    | |__  | (___       
	//  \___ \| |    |  __|  \___ \      
	//  ____) | |____| |____ ____) | _ _ 
	// |_____/|______|______|_____(_|_|_)                                   
    // create engine
	SLresult result;
	result = slCreateEngine(&SLES_engineObject, 0, NULL, 0, NULL, NULL);
    if(SL_RESULT_SUCCESS != result) {
		OPLog("Jukebox::Error 1");
		return false;
	}
	
    // realize the engine
    if(SL_RESULT_SUCCESS != (*SLES_engineObject)->Realize(
		SLES_engineObject, SL_BOOLEAN_FALSE)){

		OPLog("Jukebox::Error 2");
		return false;
	}
	
    // get the engine interface, which is needed in order to create other objects
    if(SL_RESULT_SUCCESS != (*SLES_engineObject)->GetInterface(
    	SLES_engineObject, SL_IID_ENGINE, &SLES_engineEngine)){

		OPLog("Jukebox::Error 3");
		return false;
	}

	OPLog("OPaudio: engineEngine VVV");
	//OPLog_i32(SLES_engineEngine);

	// create output mix, with environmental reverb specified as a non-required interface    
	const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};

    if(SL_RESULT_SUCCESS != (*SLES_engineEngine)->CreateOutputMix(SLES_engineEngine, &SLES_outputMixObject, 1, ids, req)){

		OPLog("Jukebox::Error 4");
	}
		
    // realize the output mix
    result = (*SLES_outputMixObject)->Realize(SLES_outputMixObject, SL_BOOLEAN_FALSE);
    if(SL_RESULT_SUCCESS != result) {

		OPLog("Jukebox::Error 5");
		return false;
	}
	OPLog("OPaudio::Initialized");
#else // USE OpenAL FOR SOUND
	//   ____                            _            
	//  / __ \                     /\   | |           
	// | |  | |_ __   ___ _ __    /  \  | |           
	// | |  | | '_ \ / _ \ '_ \  / /\ \ | |           
	// | |__| | |_) |  __/ | | |/ ____ \| |____ _ _ _ 
	//  \____/| .__/ \___|_| |_/_/    \_\______(_|_|_)
	//        | |                                     
	//        |_|                                     
	// setup the device and stuff
	AL_OPaudioDevice = alcOpenDevice(NULL);
	if(!AL_OPaudioDevice) return 0;
	AL_OPaudioContext = alcCreateContext(AL_OPaudioDevice, NULL);
	alcMakeContextCurrent(AL_OPaudioContext);
	if(!AL_OPaudioContext) return -1;

	alDistanceModel(AL_LINEAR_DISTANCE);
#endif
	OPLog("OP audio Initialized!!!");
	return 1;
}
//-----------------------------------------------------------------------------
Vector3 OPaudEarPosition(Vector3* pos){
#ifdef OPIFEX_ANDROID	
	Vector3 out;
	return out;
#else
	if(pos) alListenerfv(AL_POSITION, &(pos->_x));

	// return the stored value
	Vector3 out;
	alGetListenerfv(AL_POSITION, &(pos->_x));
	return out;
#endif
}
//-----------------------------------------------------------------------------
Vector3 OPaudEarVelocity(Vector3* pos){
#ifdef OPIFEX_ANDROID	
	Vector3 out;
	return out;
#else
	if(pos) alListenerfv(AL_VELOCITY, &(pos->_x));

	// return the stored value
	Vector3 out;
	alGetListenerfv(AL_VELOCITY, &(pos->_x));
	return out;
#endif
}
//-----------------------------------------------------------------------------
Vector3 OPaudEarForward(Vector3* pos){
#ifdef OPIFEX_ANDROID	
	Vector3 out;
	return out;
#else
	if(pos) alListenerfv(AL_ORIENTATION, &(pos->_x));

	// return the stored value
	Vector3 out;
	alGetListenerfv(AL_ORIENTATION, &(pos->_x));
	return out;
#endif
}
//-----------------------------------------------------------------------------