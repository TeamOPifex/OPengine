#include "./Human/include/Audio/OPaudioEmitter.h"

OPaudioEmitter* OPAUD_CURR_EMITTER;

#ifdef OPIFEX_ANDROID
	void SL_DequeueCallback(SLAndroidSimpleBufferQueueItf bq, void *context){
		OPaudioEmitter* emitter = (OPaudioEmitter*)context;
		emitter->_queued--;
	}
#endif

//-----------------------------------------------------------------------------
//                    _ _       ______           _ _   _              ______                _   _                 
//     /\            | (_)     |  ____|         (_) | | |            |  ____|              | | (_)                
//    /  \  _   _  __| |_  ___ | |__   _ __ ___  _| |_| |_ ___ _ __  | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//   / /\ \| | | |/ _` | |/ _ \|  __| | '_ ` _ \| | __| __/ _ \ '__| |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//  / ____ \ |_| | (_| | | (_) | |____| | | | | | | |_| ||  __/ |    | |  | |_| | | | | (__| |_| | (_) | | | \__ \
// /_/    \_\__,_|\__,_|_|\___/|______|_| |_| |_|_|\__|\__\___|_|    |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

OPaudioEmitter OPaudCreateEmitter(OPaudioSource* src, /*void* processor,*/ OPint looping){
	OPaudioEmitter emitter = { 0 };
	emitter.Looping  = looping;
	emitter.State    = Stopped;
	emitter.Source   = src;
	emitter.Processor= NULL;//processor;


#ifdef OPIFEX_ANDROID
	OPLog("OPsoundEmitter: 0");
	OPLog("OPaudioEmitter: Chann=%d, Samp/Sec=%d\n", src->Description.Channels, src->Description.SamplesPerSecond);
    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, BUFFER_COUNT};
    SLDataFormat_PCM format_pcm = {
    	SL_DATAFORMAT_PCM,
    	src->Description.Channels,
    	src->Description.SamplesPerSecond * 1000,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        src->Description.Channels == 1 ? SL_SPEAKER_FRONT_CENTER : SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
        SL_BYTEORDER_LITTLEENDIAN
    };

    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    OPLog("OPSoundEmitter: 0.5");
	OPLog_i32((OPint)SLES_outputMixObject);

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, SLES_outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};
	
    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND,
            /*SL_IID_MUTESOLO,*/ SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE,
            /*SL_BOOLEAN_TRUE,*/ SL_BOOLEAN_TRUE};
    OPLog("OPsoundEmitter: 1");

    OPLog_ui32((OPint)SLES_engineEngine);

    (*SLES_engineEngine)->CreateAudioPlayer(
    	SLES_engineEngine, 
    	&emitter._playerObject,
    	&audioSrc,
    	&audioSnk,
        3, ids, req
    );
    (*emitter._playerObject)->Realize(emitter._playerObject, SL_BOOLEAN_FALSE);

    OPLog("OPaudioEmitter: 2");

    // get the play interface
    (*emitter._playerObject)->GetInterface(emitter._playerObject, SL_IID_PLAY, &emitter._playerPlay);
OPLog("OPaudioEmitter: 3");
    // get the buffer queue
	(*emitter._playerObject)->GetInterface(emitter._playerObject, SL_IID_BUFFERQUEUE,
	            &emitter._bqPlayerBufferQueue);
OPLog("OPaudioEmitter: 3.5");
	emitter._queued = -1;

	for(OPint i = BUFFER_COUNT; i--;) emitter.Buffers[i] = (ui8*)OPalloc(BUFFER_SIZE);
    OPLog("OPsoundEmitter: 4");
#else
	alGenBuffers(BUFFER_COUNT, emitter.Buffers);
	alGenSources(1, &emitter.al_src);
	alSourcei(emitter.al_src, AL_LOOPING, AL_FALSE);
#endif

	return emitter;
}

void OPaudEnqueueBuffer(ui8* buffer, OPint length){
	OPint active = OPAUD_CURR_EMITTER->CurrBuffer;
#ifdef OPIFEX_ANDROID
	(*OPAUD_CURR_EMITTER->_bqPlayerBufferQueue)->Enqueue(
		OPAUD_CURR_EMITTER->_bqPlayerBufferQueue,
		OPAUD_CURR_EMITTER->Buffers[active++],
		length
	);

	++OPAUD_CURR_EMITTER->_queued;
#else
	alBufferData(
		OPAUD_CURR_EMITTER->Buffers[active],
		OPAUD_CURR_EMITTER->Source->Description.Format,
		buffer,
		length,
		OPAUD_CURR_EMITTER->Source->Description.SamplesPerSecond
	);

	// queue this buffer to be played
	alSourceQueueBuffers(
		OPAUD_CURR_EMITTER->al_src,
		1,
		&OPAUD_CURR_EMITTER->Buffers[active++]
	);
#endif

	// set the active index
	OPAUD_CURR_EMITTER->CurrBuffer = active % BUFFER_COUNT;
}

#ifdef OPIFEX_ANDROID
	void OPaudPlay(){
		OPLog("OPSoundEmitter: Play start...");
		OPLog("OPSoundEmitter: Invoking play...");
		(*OPAUD_CURR_EMITTER->_playerPlay)->SetPlayState(
			OPAUD_CURR_EMITTER->_playerPlay,
			SL_PLAYSTATE_PLAYING
		);
		OPAUD_CURR_EMITTER->State = Playing;
		OPLog("OPSoundEmitter: \"playing\"");
		OPAUD_CURR_EMITTER->State = Playing;
	}

	void OPaudPause(){
		(*OPAUD_CURR_EMITTER->_playerPlay)->SetPlayState(
			OPAUD_CURR_EMITTER->_playerPlay,
			SL_PLAYSTATE_PAUSED
		);
		OPAUD_CURR_EMITTER->State = Paused;
	}

	void OPaudStop(){
		(*OPAUD_CURR_EMITTER->_playerPlay)->SetPlayState(
			OPAUD_CURR_EMITTER->_playerPlay,
			SL_PLAYSTATE_STOPPED
		);
		OPAUD_CURR_EMITTER->State = Stopped;
		OPAUD_CURR_EMITTER->Source->Progress = 0;
	}
#else
	void OPaudPlay(){
		alSourcePlay(OPAUD_CURR_EMITTER->al_src);
		OPAUD_CURR_EMITTER->State = Playing;
	}

	void OPaudPause(){
		alSourcePause(OPAUD_CURR_EMITTER->al_src);
		OPAUD_CURR_EMITTER->State = Paused;
	}

	void OPaudStop(){
		alSourceStop(OPAUD_CURR_EMITTER->al_src);
		OPAUD_CURR_EMITTER->State = Stopped;
		OPAUD_CURR_EMITTER->Source->Progress = 0;
	}
#endif

OPint OPaudUpdate(void(*Proc)(OPaudioEmitter* emit, OPint length)){
	if(OPAUD_CURR_EMITTER->State != Playing) return 0;

	OPint len = 0, queued = 0;
	OPaudioSource* src = OPAUD_CURR_EMITTER->Source;
	OPaudioDescription des = src->Description;
	OPint bps = (des.BitsPerSample >> 3) * des.SamplesPerSecond; // bytes/second

	OPLog("bits/samp: %d samp/sec: %d bytes/sec: %d", des.BitsPerSample, des.SamplesPerSecond, bps);

	OPint shift = bps >= 1024 ? 5 : 0;

	OPLog("OPaudUpdate: 1\n");

#ifdef OPIFEX_ANDROID
	queued = OPAUD_CURR_EMITTER->_queued;
	if(queued < 0){
		(*OPAUD_CURR_EMITTER->_playerPlay)->SetPlayState(
			OPAUD_CURR_EMITTER->_playerPlay,
			SL_PLAYSTATE_STOPPED
		);
		(*OPAUD_CURR_EMITTER->_bqPlayerBufferQueue)->RegisterCallback(
			OPAUD_CURR_EMITTER->_bqPlayerBufferQueue,
			SL_DequeueCallback,
			OPAUD_CURR_EMITTER
		);
		queued = 0;
	}

	if(queued > 0){
        SLuint32 slState;
		SLPlayItf pp = OPAUD_CURR_EMITTER->_playerPlay;
		OPLog("OPaudUpdate: 2\n");
        (*pp)->GetPlayState(pp, &slState);
        OPLog("OPaudUpdate: 3\n");
        if(slState == SL_PLAYSTATE_STOPPED){
	        OPLog("OPaudUpdate: 4\n");
			(*pp)->SetPlayState(pp, SL_PLAYSTATE_PLAYING);
			OPLog("OPaudUpdate: 5\n");
        }

		// all buffers are in use, wait for one to process
        if(queued == BUFFER_COUNT){
        	return 0;
        }
	}
	OPLog("OPaudUpdate: 6\n");
#else
	ALint processed = 0;
	ALuint unqueued[BUFFER_COUNT];

	alGetSourcei(OPAUD_CURR_EMITTER->al_src, AL_BUFFERS_QUEUED, &queued);
	alGetSourcei(OPAUD_CURR_EMITTER->al_src, AL_BUFFERS_PROCESSED, &processed);

	if(queued > 0){
		ALint state;
		alGetSourcei(OPAUD_CURR_EMITTER->al_src, AL_SOURCE_STATE, &state);
		if(state == AL_STOPPED || state == AL_INITIAL) alSourcePlay(OPAUD_CURR_EMITTER->al_src);

		// all buffers are in use, wait for one to process
		if(queued == BUFFER_COUNT && processed == 0){
			return 0;
		}
	}

	// unqueue bufferes that have been consumed
	alSourceUnqueueBuffers(OPAUD_CURR_EMITTER->al_src, processed, unqueued);
#endif

#ifdef OPIFEX_ANDROID
	ui8* buff = OPAUD_CURR_EMITTER->Buffers[OPAUD_CURR_EMITTER->CurrBuffer];
#else
	ui8* buff = OPAUD_CURR_EMITTER->Temp;
#endif

	// read sample rate >> bytes/sec pcm bytes
	if(len = src->Read(src, buff, bps >> shift)){//bps >> shift)){ //)
		OPLog("OPaudUpdate: 7 len=%d\n", len);
		Proc(OPAUD_CURR_EMITTER, len);

		OPaudEnqueueBuffer(buff, len);
		OPLog("OPaudUpdate: 8\n");
		return len;
	}
	else if(OPAUD_CURR_EMITTER->Looping){
		// reset the sound!
		OPLog("OPaudUpdate: 9\n");
		src->Seek(src, 0);
		OPLog("OPaudUpdate: 11\n");
		return 0;
	}
	else if(queued == 0){
		OPLog("OPaudUpdate: 10\n");
		OPaudStop();
		OPLog("OPaudUpdate: 12\n");
	}

	return -1;
}

OPint OPaudProc(void(*Proc)(OPaudioEmitter* emit)){
	return 0;
}

//-----------------------------------------------------------------------------
//  ______           _ _   _              _____                     
// |  ____|         (_) | | |            |  __ \                    
// | |__   _ __ ___  _| |_| |_ ___ _ __  | |__) | __ ___  _ __  ___ 
// |  __| | '_ ` _ \| | __| __/ _ \ '__| |  ___/ '__/ _ \| '_ \/ __|
// | |____| | | | | | | |_| ||  __/ |    | |   | | | (_) | |_) \__ \
// |______|_| |_| |_|_|\__|\__\___|_|    |_|   |_|  \___/| .__/|___/
//                                                       | |        
//                                                       |_|        
void OPaudPosition(Vector3* position){
#ifdef OPIFEX_ANDROID	
	Vector3 out;
#else
	alSourcefv(OPAUD_CURR_EMITTER->al_src, AL_POSITION, position->ptr());
#endif
}

void OPaudVelocity(Vector3* velocity){
#ifdef OPIFEX_ANDROID	
	Vector3 out;
	return out;
#else
	alSourcefv(OPAUD_CURR_EMITTER->al_src, AL_VELOCITY, velocity->ptr());
#endif
}

void OPaudVolume  (OPfloat gain){
#ifdef OPIFEX_ANDROID	
	return 0;
#else
	alSourcef(OPAUD_CURR_EMITTER->al_src, AL_GAIN, gain);
#endif
}

void OPaudPitch   (OPfloat pitch){
#ifdef OPIFEX_ANDROID	
	return 0;
#else
	alSourcef(OPAUD_CURR_EMITTER->al_src, AL_PITCH, pitch);
#endif
}
//-----------------------------------------------------------------------------
void OPaudProcess(OPaudioEmitter* emit, OPint length){
	// Do nothing :)

	/*for(OPint i = 0; i < length; ++i){
		write(1, emit->Temp, length);
	}*/
}
//-----------------------------------------------------------------------------