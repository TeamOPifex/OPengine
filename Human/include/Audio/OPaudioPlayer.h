#pragma once
#ifndef OPAUD_AUDIOPLAYER
#define OPAUD_AUDIOPLAYER

#include "Core/include/Target.h"
#include "Core/include/Types.h"
#include "Core/include/MathHelpers.h"
#include "Core/include/DynamicMemory.h"
#include "Core/include/Log.h"
#include "Data/include/OPfile.h"
#include "./Math/include/Vector3.h"

#include "./Human/include/Audio/OPaudio2.h"
#include "./Human/include/Audio/OPaudioEmitter.h"

struct OPaudioPlayer{
	OPaudioEmitter* Emitters;
	OPint Count;
	OPint Current;
};

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
extern OPaudioPlayer* OPAUD_CURR_PLAYER;
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _               
// |  __ \                   |  __ \                |  __ \(_)             | | (_)              
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___ 
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/                                                                            
#define OPaudSetPlayer(player){OPAUD_CURR_PLAYER = player;};\
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPaudioPlayer OPaudPlayerCreate(OPaudioSource* src, OPint sounds, OPint looping);

void OPaudPlayerDestroy(OPaudioPlayer* player);
void OPaudPlayerPlay();
void OPaudPlayerStop();
void OPaudPlayerPause();

void OPaudPlayerUpdate(void(*Proc)(OPaudioEmitter* emit, OPint length));

void OPaudPlayerPosition(Vector3* position);
void OPaudPlayerVelocity(Vector3* velocity);
void OPaudPlayerVolume  (OPfloat gain);
void OPaudPlayerPitch   (OPfloat pitch);
//-----------------------------------------------------------------------------
#ifdef __cplusplus
};
#endif

#endif