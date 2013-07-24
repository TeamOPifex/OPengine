#include "OPaudioSource.h"

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

#define BUFFER_COUNT 10
//-----------------------------------------------------------------------------
// ______                           
// |  ____|                          
// | |__   _ __  _   _ _ __ ___  ___ 
// |  __| | '_ \| | | | '_ ` _ \/ __|
// | |____| | | | |_| | | | | | \__ \
// |______|_| |_|\__,_|_| |_| |_|___/
enum OPaudioEmitterState{
	Playing = 0,
	Stopped = 1,
	Paused = 2
};
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
typedef struct{
	OPint               CurrBuffer;           // index of the buffer currently being filled
	OPint				Looping;              // indicates weather the sounds restarts when finished or not
	OPaudioEmitterState State;                // current play state of the sound
	OPaudioSource*      Source;               // data source for the sound
	ui8*                Buffers[BUFFER_COUNT];// Buffers of processed data
	void*               Processor;            // Pointer to data for audio processing

#ifdef OPIFEX_ANDROID 
	SLObjectItf _outputMixObject, _playerObject;
	SLAndroidSimpleBufferQueueItf _bqPlayerBufferQueue;

	SLPlayItf _playerPlay;
	SLSeekItf _playerSeek;
	SLMuteSoloItf _playerMuteSolo;
	SLVolumeItf _playerVolume;
#else // All other targets...
	ALuint _alSrc;
#endif
}OPaudioEmitter;
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//                    _ _       ______           _ _   _              ______                _   _                 
//     /\            | (_)     |  ____|         (_) | | |            |  ____|              | | (_)                
//    /  \  _   _  __| |_  ___ | |__   _ __ ___  _| |_| |_ ___ _ __  | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//   / /\ \| | | |/ _` | |/ _ \|  __| | '_ ` _ \| | __| __/ _ \ '__| |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//  / ____ \ |_| | (_| | | (_) | |____| | | | | | | |_| ||  __/ |    | |  | |_| | | | | (__| |_| | (_) | | | \__ \
// /_/    \_\__,_|\__,_|_|\___/|______|_| |_| |_|_|\__|\__\___|_|    |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
void  OPaudEnqueueBuffer(OPaudioEmitter* emitter);

OPint OPaudPlay  (OPaudioEmitter* emitter);
OPint OPaudPause (OPaudioEmitter* emitter);
OPint OPaudStop  (OPaudioEmitter* emitter);
OPint OPaudSeek  (OPaudioEmitter* emitter);
OPint OPaudUpdate(OPaudioEmitter* emitter);

OPint OPaudProc(OPaudioEmitter* emitter, void(*Proc)(OPaudioEmitter* emit));

Vector3 OPaudSetPosition(Vector3 position);
Vector3 OPaudSetVelocity(Vector3 velocity);
OPfloat OPaudSetVolume  (OPfloat gain);
OPfloat OPaudSetPitch   (OPfloat pitch);
//-----------------------------------------------------------------------------
#ifdef __cplusplus
};
#endif