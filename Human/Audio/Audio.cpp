#include "Audio.h"

Audio::Audio(char* filename, bool loop){
	_looping = loop;
#ifdef OPIFEX_ANDROID
	SLresult result;



	 // TODO: LOAD FROM ANDROID
	int fd, start, length;



	// configure audio source
	SLDataLocator_AndroidFD loc_fd = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
	SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
	SLDataSource audioSrc = {&loc_fd, &format_mime};
		
	// configure audio sink
	SLObjectItf* outputMixer = (SLObjectItf*)JukeBox::Mixer();

	SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, (*outputMixer)};
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
	(*_fdPlayerObject)->GetInterface(_fdPlayerObject, SL_IID_PLAY, &_fdPlayerObject);

	if(_looping){
		// get the seek interface
		if(SL_RESULT_SUCCESS == (*_fdPlayerObject)->GetInterface(_fdPlayerSeek, SL_IID_SEEK, &_fdPlayerSeek))
		{
			(*_fdPlayerSeek)->SetLoop(_fdPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
		}
	}
#else

#endif
}

bool Audio::Pause(){
#ifdef OPIFEX_ANDROID
	if(SL_RESULT_SUCCESS != (*_fdPlayerObject)->SetPlayState(_fdPlayerPlay, SL_PLAYSTATE_PAUSED))
		return false;
#endif
	return true;
}

bool Audio::Stop(){
#ifdef OPIFEX_ANDROID
	if(SL_RESULT_SUCCESS != (*_fdPlayerObject)->SetPlayState(_fdPlayerPlay, SL_PLAYSTATE_STOPPED))
		return false;
#endif
	return true;
}

bool Audio::Play(){
#ifdef OPIFEX_ANDROID
	if(SL_RESULT_SUCCESS != (*_fdPlayerObject)->SetPlayState(_fdPlayerPlay, SL_PLAYSTATE_PLAYING))
		return false;
#endif
	return true;
}

bool Audio::Looping(){
	return _looping;
}

ui32 Audio::Loops(){
	return 0;
}

bool Audio::Done(){
	return false;
}