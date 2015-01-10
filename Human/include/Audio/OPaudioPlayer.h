#ifndef OPENGINE_HUMAN_AUDIO_AUDIOPLAYER
#define OPENGINE_HUMAN_AUDIO_AUDIOPLAYER

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmath.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"
#include "./Data/include/OPfile.h"
#include "./Math/include/OPvec3.h"

#include "./Human/include/Audio/OPaudio.h"
#include "./Human/include/Audio/OPaudioEmitter.h"

struct OPaudioPlayer{
	OPaudioEmitter** Emitters;
	OPint Count;
	OPint Current;
};


//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
extern IMPORT_METHOD OPaudioPlayer* OPAUD_CURR_PLAYER;
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _               
// |  __ \                   |  __ \                |  __ \(_)             | | (_)              
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___ 
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/                                                                            
#define OPaudPlayerSet(player){OPAUD_CURR_PLAYER = player;};\
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
EXPORT_METHOD OPaudioPlayer OPaudPlayerCreate(OPaudioSource* src, OPint sounds, OPint looping);

EXPORT_METHOD void OPaudPlayerDestroy(OPaudioPlayer* player);
EXPORT_METHOD void OPaudPlayerPlay();
EXPORT_METHOD void OPaudPlayerStop();
EXPORT_METHOD void OPaudPlayerPause();

EXPORT_METHOD void OPaudPlayerUpdate(void(*Proc)(OPaudioEmitter* emit, OPint length));

EXPORT_METHOD void OPaudPlayerPosition(OPvec3* position);
EXPORT_METHOD void OPaudPlayerVelocity(OPvec3* velocity);
EXPORT_METHOD void OPaudPlayerVolume(OPfloat gain);
EXPORT_METHOD void OPaudPlayerPitch(OPfloat pitch);


#endif