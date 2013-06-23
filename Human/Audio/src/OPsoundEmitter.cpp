#include "../include/OPsoundEmitter.h"

#ifdef OPIFEX_ANDROID
void OPSoundEmitter::ENQUEUE_BUFFER(SLAndroidSimpleBufferQueueItf bq, void *context){
	OPLog("OPSoundEmitter: Callback entered...");

	OPSoundEmitter* emitter = (OPSoundEmitter*)context;
	emitter->_buffersProcessed++;
	emitter->_freeBuffers++;
	//OPLog_i32(emitter->_activeBuffer);
	//OPLog("OPSoundEmitter: Callback exited.");
}
#endif

OPSoundEmitter::OPSoundEmitter(OPsound* sound, OPint sections){
	OPLog("OPsoundEmitter: -1");
	OPint bytesPerBuffer = (sound->SampleRate * sound->BitsPerSample) / sections;
	_sound = sound;

	_intermediateBuffer = new unsigned char[_bufferSize = bytesPerBuffer];
	for(OPint i = bytesPerBuffer; i--; _intermediateBuffer[i] = 0);

#ifdef OPIFEX_ANDROID
	_playerObject = NULL;
	_bqPlayerBufferQueue = NULL;

	_buffersQueued = _buffersProcessed = 0;

	OPLog("OPsoundEmitter: 0");
    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, BUFFERS};
    SLDataFormat_PCM format_pcm = {
    	SL_DATAFORMAT_PCM,
    	1,
    	SL_SAMPLINGRATE_44_1,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_SPEAKER_FRONT_CENTER,
        SL_BYTEORDER_LITTLEENDIAN
    };

    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    OPLog("OPSoundEmitter: 0.5");
	OPLog_i32((OPint)OPAudio::OutputMixObject);

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, OPAudio::OutputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};
	
    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND,
            /*SL_IID_MUTESOLO,*/ SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE,
            /*SL_BOOLEAN_TRUE,*/ SL_BOOLEAN_TRUE};
    OPLog("OPsoundEmitter: 1");

    OPLog_ui32((OPint)OPAudio::EngineEngine);

    (*OPAudio::EngineEngine)->CreateAudioPlayer(
    	OPAudio::EngineEngine, 
    	&_playerObject,
    	&audioSrc,
    	&audioSnk,
        3, ids, req
    );

    OPLog("OPsoundEmitter: 1.25");
    OPLog_ui32((OPint)_playerObject);

	(*_playerObject)->Realize(_playerObject, SL_BOOLEAN_FALSE);

    OPLog("OPsoundEmitter: 1.75");
    OPLog_ui32((OPint)_playerObject);

    OPLog("OPsoundEmitter: 3");

    // get the play interface
    (*_playerObject)->GetInterface(_playerObject, SL_IID_PLAY, &_playerPlay);


	OPLog_ui32((OPint)_playerObject);    
    OPLog("OPsoundEmitter: 2");

	(*_playerObject)->GetInterface(_playerObject, SL_IID_BUFFERQUEUE,
	            &_bqPlayerBufferQueue);


    OPLog("OPsoundEmitter: 3.5");
    OPLog("OPSoundEmitter: _bqPlayerBufferQueue VVV");
    OPLog_i32((OPint)_bqPlayerBufferQueue);

	(*_bqPlayerBufferQueue)->RegisterCallback(
		_bqPlayerBufferQueue,
		ENQUEUE_BUFFER,
		this
	);

    OPLog("OPsoundEmitter: 4");
#else // OPENAL FOR DESKTOPS
	alGenBuffers(BUFFERS, _buffers); // backbuffer and forebuffer
	alGenSources(1, &_alSrc);
	alSourcei(_alSrc, AL_LOOPING, AL_FALSE);
#endif

	OPLog("Buffer size: ");
	OPLog_i32(_bufferSize);
	// fill all buffers with silence
	for(OPint i = BUFFERS; i--;)
#ifdef OPIFEX_ANDROID
	{
		_playingBuffers[i] = (ui8*)OPalloc(sizeof(ui8) * _bufferSize);

		for(OPint j = _bufferSize; j--;)
			_playingBuffers[i][j] = 0x00;
	}
#else
	alBufferData(
		_buffers[i],
		_sound->Format,
		_intermediateBuffer,
		bytesPerBuffer,
		_sound->SampleRate
	);
#endif

	Looping = false;
	_activeBuffer = 0; // start with the forebuffer
	_oldBuffsPlayed = _queued = 0;
	_bytesPlayed = _oldBytesPlayed = 0; // bytes played
	_freeBuffers = BUFFERS;

	_chunkSize = bytesPerBuffer / CHUNKS;
	_chunksProcessed = 0;
	_bytesInBuffer = 0;
	_queued = 0;
}
/*---------------------------------------------------------------------------*/
OPSoundEmitter::~OPSoundEmitter(){
#ifdef OPIFEX_ANDROID

#else
	alDeleteBuffers(BUFFERS, _buffers);
	alDeleteSources(1, &_alSrc);
#endif
}
/*---------------------------------------------------------------------------*/
void OPSoundEmitter::Play(){
#ifdef OPIFEX_ANDROID
	OPLog("OPSoundEmitter: Play start...");
	while(_freeBuffers && process());
	OPLog_i32(_freeBuffers);
	OPLog("OPSoundEmitter: Invoking play...");
	(*_playerPlay)->SetPlayState(_playerPlay, SL_PLAYSTATE_PLAYING);
	_state = Playing;
	OPLog("OPSoundEmitter: \"playing\"");
#else
	while(_freeBuffers && process());
	alSourcePlay(_alSrc);
	_state = Playing;
#endif
}
/*---------------------------------------------------------------------------*/
void OPSoundEmitter::Pause(){
#ifdef OPIFEX_ANDROID

#else
	alSourcePause(_alSrc);
	_state = Paused;
#endif
}
/*---------------------------------------------------------------------------*/
void OPSoundEmitter::Stop(){
#ifdef OPIFEX_ANDROID

#else
	alSourceStop(_alSrc);
	_state = Stopped;
#endif
}
/*---------------------------------------------------------------------------*/
void OPSoundEmitter::Update(){

	if(_state == Playing){
		OPint buffsPlayed = 0, buffsQueued = 0, state = 0, playPos = 0;

#ifdef OPIFEX_ANDROID
		SLmillisecond slPosition;
		buffsQueued = _buffersQueued;
		buffsPlayed = _buffersProcessed;
		(*_playerPlay)->GetPosition(_playerPlay, &slPosition);

		slPosition -= _oldPlayPos;

		//playPos = (OPint)((slPosition / 1000.0f) * (_sound->SampleRate / 2) * (_sound->BitsPerSample >> 3));
		playPos = (_buffersQueued * _bufferSize);
#else
		alGetSourcei(_alSrc, AL_BUFFERS_PROCESSED, &buffsPlayed);
		alGetSourcei(_alSrc, AL_BUFFERS_QUEUED, &buffsQueued);
		alGetSourcei(_alSrc, AL_BYTE_OFFSET, &playPos);
#endif

		if(buffsPlayed != _oldBuffsPlayed){

			if(buffsQueued){
				OPint playedBuff = 0;
#ifdef OPIFEX_ANDROID
#else
				alSourceUnqueueBuffers(_alSrc, buffsPlayed, (ALuint*)(&playedBuff));
				_freeBuffers += buffsPlayed;
#endif
				_bytesPlayed += _bufferSize;

			}
			_oldBuffsPlayed = buffsPlayed;
		}

		if(_queued + playPos >= _sound->DataSize){
			if(_sound->FillCallback){
				if(_sound->FillCallback(_sound, 0, 0) > 0)
				{
					_queued = 0;
				}
				else 
				{
					if(!Looping){
						Stop();
					} else {
						_queued = 0;
						Stop();
						if(_sound->Reset){
							_sound->Reset(_sound);
							Play();
							_sound->FillCallback(_sound, 0, 0);
							_queued = 0;
						}
					}
				}
			}
			else{
				// TODO
				// ^ what? I don't know...
			}
		}

		// if there are avalible buffers, fill them with data
		if(_freeBuffers) process();

		// can we continue playing after we've run out of data?
#ifdef OPIFEX_ANDROID
		SLuint32 slState;
		(*_playerPlay)->GetPlayState(_playerPlay, &slState);
		if(slState == SL_PLAYSTATE_STOPPED && _freeBuffers != BUFFERS){
			(*_playerPlay)->SetPlayState(_playerPlay, SL_PLAYSTATE_PLAYING);
			_state = Playing;
		}
#else
		alGetSourcei(_alSrc, AL_SOURCE_STATE, &state);
		if(state == AL_STOPPED && _freeBuffers != BUFFERS){
			alSourcePlay(_alSrc);
		}
#endif
	}
}
/*---------------------------------------------------------------------------*/
void OPSoundEmitter::SetSound(OPsound* sound){
	_sound = sound;

	for(OPint i = BUFFERS; i--;)
#ifdef OPIFEX_ANDROID
	{/*TODO*/}
#else
	alBufferData(
		_buffers[i],
		_sound->Format,
		_intermediateBuffer,
		_bufferSize,
		_sound->SampleRate
	);
#endif
}
/*---------------------------------------------------------------------------*/
OPint OPSoundEmitter::process(){
		OPint toProcess = _sound->DataSize - (_queued + _bytesInBuffer); // # of bytes that need to be queued
		OPint offset = _chunksProcessed++ * _chunkSize;                  // offset in bytes for current chunk
		toProcess = toProcess > _chunkSize ? _chunkSize : toProcess;   // don't process more than a chunk's worth

		if(!Looping && !toProcess) {
			 // no more data! we are done
				return 0;
		}

		// this is where processing would happen, for now just simply copy
		OPmemcpy((&_intermediateBuffer[offset]), (&(_sound->Data + _queued)[offset]), toProcess);


		_bytesInBuffer += toProcess;
		//OPLog("\tprocess: Mem copied");
		// have all the chunks of this buffer been processed?
		if(_chunksProcessed == CHUNKS){
			// push processed data to the buffer
#ifdef OPIFEX_ANDROID
			OPmemcpy(_playingBuffers[_activeBuffer], _intermediateBuffer, _bytesInBuffer);
			OPLog("\tProcess: Queuing buffer"); OPLog_i32(_activeBuffer);
			(*_bqPlayerBufferQueue)->Enqueue(_bqPlayerBufferQueue, _playingBuffers[_activeBuffer], _bytesInBuffer);
			++_buffersQueued;
#else
			alBufferData(
				_buffers[_activeBuffer],
				_sound->Format,
				_intermediateBuffer,
				_bytesInBuffer,
				_sound->SampleRate
			);
			alSourceQueueBuffers(_alSrc, 1, &_buffers[_activeBuffer]); // queue this buffer to be played

#endif
			++_activeBuffer %= BUFFERS;            // swap to the next buffer
			_queued += _bytesInBuffer;             // increment the total number of bytes that have been queued
			_bytesInBuffer = _chunksProcessed = 0; // clear for next buffer
			_freeBuffers--;                        // indicate that one less buffer is free
			
			if(Looping && toProcess < _chunkSize){
				_queued = 0;
			}
		}

		return toProcess;
}
