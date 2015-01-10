#ifndef OPENGINE_HUMAN_AUDIO_EMITTER
#define OPENGINE_HUMAN_AUDIO_EMITTER

#include "OPaudioSource.h"
#include "./Data/include/OPentHeap.h"


#define EMITTER_THREADED 0x0001
#define EMITTER_LOOPING  0x0002

#define BUFFER_COUNT 10
#define BUFFER_SIZE 4096
#define THREADED_EMITTERS 100
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
	OPint				Flags;                // Specifies behavior such as looping and threadedness
	OPaudioEmitterState State;                // current play state of the sound
	ui64                Progress;             // Current play position
	OPaudioSource*      Source;               // data source for the sound
	ui8                 Temp[BUFFER_SIZE];    // BUFFER_SIZE byte temp buffer for processing
	void*               Processor;            // Pointer to data for audio processing
	OPmutex             Lock;                 // Protects the sound from cross thread calls

#ifdef OPIFEX_ANDROID 
	SLObjectItf _outputMixObject, _playerObject;
	SLAndroidSimpleBufferQueueItf _bqPlayerBufferQueue;

	SLPlayItf _playerPlay;
	SLSeekItf _playerSeek;
	SLMuteSoloItf _playerMuteSolo;
	SLVolumeItf _playerVolume;

	OPint _queued;

	ui8* Buffers[BUFFER_COUNT];// Buffers of processed data
#else // All other targets...
	ALuint al_src;
	ALuint Buffers[BUFFER_COUNT]; // AL buffers 
#endif
}OPaudioEmitter;
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
extern IMPORT_METHOD OPaudioEmitter* OPAUD_CURR_EMITTER;
extern IMPORT_METHOD OPmutex         OPAUD_CURR_MUTEX;
extern IMPORT_METHOD OPentHeap       OPAUD_REG_EMITTERS;
extern IMPORT_METHOD OPthread        OPAUD_UPDATE_THREAD;
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _               
// |  __ \                   |  __ \                |  __ \(_)             | | (_)              
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___ 
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/                                                                            
#define OPaudSetEmitter(emitter){OPAUD_CURR_EMITTER = emitter;};\
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//                    _ _       ______           _ _   _              ______                _   _                 
//     /\            | (_)     |  ____|         (_) | | |            |  ____|              | | (_)                
//    /  \  _   _  __| |_  ___ | |__   _ __ ___  _| |_| |_ ___ _ __  | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//   / /\ \| | | |/ _` | |/ _ \|  __| | '_ ` _ \| | __| __/ _ \ '__| |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//  / ____ \ |_| | (_| | | (_) | |____| | | | | | | |_| ||  __/ |    | |  | |_| | | | | (__| |_| | (_) | | | \__ \
// /_/    \_\__,_|\__,_|_|\___/|______|_| |_| |_|_|\__|\__\___|_|    |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
EXPORT_METHOD void OPaudInitThread(OPint maxEmitters);

EXPORT_METHOD OPaudioEmitter* OPaudCreateEmitter(OPaudioSource* src, OPint flags);
EXPORT_METHOD void OPaudDestroyEmitter(OPaudioEmitter* emitter);

EXPORT_METHOD OPaudioEmitter* OPaudGetEmitter(OPaudioSource* src, OPint flags);
EXPORT_METHOD void OPaudRecycleEmitter(OPaudioEmitter* emitter);

EXPORT_METHOD void  OPaudEnqueueBuffer(ui8* buffer, OPint length);

EXPORT_METHOD void OPaudPlay();
EXPORT_METHOD void OPaudPause();
EXPORT_METHOD void OPaudStop();
EXPORT_METHOD void OPaudSafePlay(OPaudioEmitter* emitter);
EXPORT_METHOD void OPaudSafePause(OPaudioEmitter* emitter);
EXPORT_METHOD void OPaudSafeStop(OPaudioEmitter* emitter);

EXPORT_METHOD OPint OPaudUpdate(void(*Proc)(OPaudioEmitter* emit, OPint length));
EXPORT_METHOD OPint OPaudSafeUpdate(OPaudioEmitter* emitter, void(*Proc)(OPaudioEmitter* emit, OPint length));

EXPORT_METHOD OPint OPaudProc(OPaudioEmitter* emitter, void(*Proc)(OPaudioEmitter* emit, OPint length));

EXPORT_METHOD void OPaudPosition(OPaudioEmitter* emitter, OPvec3* position);
EXPORT_METHOD void OPaudVelocity(OPaudioEmitter* emitter, OPvec3* velocity);
EXPORT_METHOD void OPaudVolume(OPaudioEmitter* emitter, OPfloat gain);
EXPORT_METHOD void OPaudPitch(OPaudioEmitter* emitter, OPfloat pitch);
//-----------------------------------------------------------------------------
EXPORT_METHOD void OPaudProcess(OPaudioEmitter* emit, OPint length);
//-----------------------------------------------------------------------------

#endif