#include "../include/OPsoundEmitter.h"

OPSoundEmitter::OPSoundEmitter(OPsound* sound, OPint sections){
	OPLog("OPsoundEmitter: -1");
	OPint bytesPerBuffer = (sound->SampleRate * sound->BitsPerSample) /  sections;
	_sound = sound;

	_intermediateBuffer = new unsigned char[_bufferSize = bytesPerBuffer];
	for(OPint i = bytesPerBuffer; i--; _intermediateBuffer[i] = 0);

#ifdef OPIFEX_ANDROID
	_playerObject = NULL;
	OPLog("OPsoundEmitter: 0");
    // configure audio source
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM format_pcm = {
    	SL_DATAFORMAT_PCM,
    	1,
    	SL_SAMPLINGRATE_8,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_SPEAKER_FRONT_CENTER,
        SL_BYTEORDER_LITTLEENDIAN
    };

    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, OPAudio::OutputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};
	
    const SLInterfaceID ids[3] = {SL_IID_BUFFERQUEUE, SL_IID_EFFECTSEND,
            /*SL_IID_MUTESOLO,*/ SL_IID_VOLUME};
    const SLboolean req[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE,
            /*SL_BOOLEAN_TRUE,*/ SL_BOOLEAN_TRUE};
    OPLog("OPsoundEmitter: 1");

    OPLog_ui32((OPint)OPAudio::EngineEngine);

    (*OPAudio::EngineEngine)->CreateAudioPlayer(OPAudio::EngineEngine, &_playerObject, &audioSrc, &audioSnk,
            3, ids, req);

	OPLog_ui32((OPint)_playerObject);    
    OPLog("OPsoundEmitter: 2");

	(*_playerObject)->GetInterface(_playerObject, SL_IID_BUFFERQUEUE,
	            &_bqPlayerBufferQueue);

	(*_playerObject)->Realize(_playerObject, SL_BOOLEAN_FALSE);

    OPLog("OPsoundEmitter: 3");

    // get the play interface
    (*_playerObject)->GetInterface(_playerObject, SL_IID_PLAY, &_playerPlay);

	(*_bqPlayerBufferQueue)->RegisterCallback(
		_bqPlayerBufferQueue,
		enqueueBuffer,
		NULL
	);

    OPLog("OPsoundEmitter: 4");
#else // OPENAL FOR DESKTOPS
	alGenBuffers(BUFFERS, _buffers); // backbuffer and forebuffer
	alGenSources(1, &_alSrc);
	alSourcei(_alSrc, AL_LOOPING, AL_FALSE);
#endif

	// fill all buffers with silence
	for(OPint i = BUFFERS; i--;)
#ifdef OPIFEX_ANDROID
	_playingBuffers[i] = 0x00;
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
	while(!_queued) process();
	(*_playerPlay)->SetPlayState(_playerPlay, SL_PLAYSTATE_PLAYING);
#else
	while(_freeBuffers &&  process());
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
				alSourceUnqueueBuffers(_alSrc, buffsPlayed, &playedBuff);
#endif
				_bytesPlayed += _bufferSize;
				_freeBuffers += buffsPlayed;
			}
			_oldBuffsPlayed = buffsPlayed;
		}

		if(_queued + playPos >= _sound->DataSize){
			//printf("Played: %d @ %d\n", _bytesPlayed, _sound->DataSize);
			if(Looping){
				_queued = _bytesPlayed = _bytesInBuffer = _chunksProcessed = 0;
				_activeBuffer = 0;
				_freeBuffers = BUFFERS;
			}
			else{
				if(_sound->FillCallback){
					if(_sound->FillCallback(_sound, 0, 0) > 0){
						_queued = _bytesPlayed = _bytesInBuffer = _chunksProcessed = 0;
						_activeBuffer = 0;
					}
					else
						Stop();

				}
				else{
					// TODO
					// ^ what? I don't know...
				}
			}
		}

		// if there are avalible buffers, fill them with data
		if(_freeBuffers) process();

		// can we continue playing after we've run out of data?
#ifdef OPIFEX_ANDROID

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

		if(!toProcess) return 0; // no more data! we are done

		// this is where processing would happen, for now just simply copy
		OPmemcpy((&_intermediateBuffer[offset]), (&(_sound->Data + _queued)[offset]), toProcess);


		OPLog("To process VVV");
		OPLog_i32(toProcess);
		OPLog_i32(_bytesInBuffer);
		_bytesInBuffer += toProcess;

		// have all the chunks of this buffer been processed?
		if(_chunksProcessed == CHUNKS){
			// push processed data to the buffer

#ifdef OPIFEX_ANDROID
			OPmemcpy(_playingBuffers[_activeBuffer], _intermediateBuffer, _bytesInBuffer);
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
		}

		return toProcess;
}

#ifdef OPIFEX_ANDROID
void OPSoundEmitter::enqueueBuffer(SLAndroidSimpleBufferQueueItf bq, void *context){
	(*bq)->Enqueue(
		bq,
		_intermediateBuffer,
		_bytesInBuffer
	);
}
#endif
