#include "Audio.h"
#include "./Data/include/OPfile.h"
#include "Jukebox.h"

#include "./Core/include/Log.h"

Audio::Audio(char* filename, bool loop){
	_looping = loop;
#ifdef OPIFEX_ANDROID
	SLresult result;


	FileInformation fileInfo = OPreadFileInformation(filename);
	_fd = fileInfo.fileDescriptor;
	
	// configure audio source
	SLDataLocator_AndroidFD loc_fd = {SL_DATALOCATOR_ANDROIDFD, fileInfo.fileDescriptor, fileInfo.start, fileInfo.length};
	SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
	SLDataSource audioSrc = {&loc_fd, &format_mime};
		
	SLObjectItf outputMixer = Jukebox::Mixer();	
	SLEngineItf engine = Jukebox::Engine();
	
	// configure audio sink
	SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixer};
	SLDataSink audioSnk = {&loc_outmix, NULL};

	// create audio player
	const SLInterfaceID ids2[3] = {SL_IID_SEEK, SL_IID_MUTESOLO, SL_IID_VOLUME};
	const SLboolean req2[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_FALSE, SL_BOOLEAN_TRUE};
	result = (*engine)->CreateAudioPlayer(engine, &_fdPlayerObject, &audioSrc, &audioSnk, 3, ids2, req2);
	if(SL_RESULT_SUCCESS != result) {
		OPLog("Audio::Error 1");
		return;
	}
		
	// realize the player
	result = (*_fdPlayerObject)->Realize(_fdPlayerObject, SL_BOOLEAN_FALSE);
	if(SL_RESULT_SUCCESS != result) {
		OPLog("Audio::Error 2");
		return;
	}
		
	// get the play interface
	result = (*_fdPlayerObject)->GetInterface(_fdPlayerObject, SL_IID_PLAY, &_fdPlayerPlay);
	if(SL_RESULT_SUCCESS != result) {
		OPLog("Audio::Error 3");
		return;
	}
	
	
	// get the volume interface
	result = (*_fdPlayerObject)->GetInterface(_fdPlayerObject, SL_IID_VOLUME, &_fdPlayerVolume);
	if(SL_RESULT_SUCCESS != result) {
		OPLog("Audio::Error 4");
		return;
	}

	// get the pitch interface
	//result = (*_fdPlayerObject)->GetInterface(_fdPlayerObject, SL_IID_PITCH, &_fdPlayerPitch);
	//if(SL_RESULT_SUCCESS != result) {
	//	OPLog("Audio::Error 5");
	//	return;
	//}

	// get the seek interface
	result = (*_fdPlayerObject)->GetInterface(_fdPlayerObject, SL_IID_SEEK, &_fdPlayerSeek);
	if(SL_RESULT_SUCCESS != result) {
			OPLog("Audio::Error 6");
		return;
	}

	if(_looping){
		(*_fdPlayerSeek)->SetLoop(_fdPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
	}
		
#else

#endif
}

Audio::~Audio(){
#ifdef OPIFEX_ANDROID
	(*_fdPlayerObject)->Destroy(_fdPlayerObject);
	_fdPlayerPlay = NULL;
	_fdPlayerSeek = NULL;
	_fdPlayerVolume = NULL;
	OPLog("Audio Killed");
#else

#endif
}

bool Audio::Pause(){
#ifdef OPIFEX_ANDROID
	if(SL_RESULT_SUCCESS != (*_fdPlayerPlay)->SetPlayState(_fdPlayerPlay, SL_PLAYSTATE_PAUSED))
		return false;
#endif
	return true;
}

bool Audio::Stop(){
#ifdef OPIFEX_ANDROID
	if(SL_RESULT_SUCCESS != (*_fdPlayerPlay)->SetPlayState(_fdPlayerPlay, SL_PLAYSTATE_STOPPED))
		return false;
#endif
	return true;
}

bool Audio::Play(){
#ifdef OPIFEX_ANDROID
	Stop();
	if(SL_RESULT_SUCCESS != (*_fdPlayerPlay)->SetPlayState(_fdPlayerPlay, SL_PLAYSTATE_PLAYING))
		return false;
#endif
	return true;
}

bool Audio::SetVolume(i32 level){
#ifdef OPIFEX_ANDROID
	if(SL_RESULT_SUCCESS != (*_fdPlayerVolume)->SetVolumeLevel(_fdPlayerVolume, level))
		return false;
#endif
	return true;
}

//bool Audio::SetPitch(i32 level){
//#ifdef OPIFEX_ANDROID
//	if(SL_RESULT_SUCCESS != (*_fdPlayerPitch)->SetPitch(_fdPlayerPitch, level))
//		return false;
//#endif
//	return true;
//}

bool Audio::Looping(){
	return _looping;
}

ui32 Audio::Loops(){
	return 0;
}

bool Audio::Done(){
#ifdef OPIFEX_ANDROID
	SLuint32 state;
	if(SL_RESULT_SUCCESS != (*_fdPlayerPlay)->GetPlayState(_fdPlayerPlay, &state)){
		return false;
	}
	if(SL_PLAYSTATE_PLAYING == state)
		return false;
#endif
	return true;
}