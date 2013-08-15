#pragma once
#ifndef OPAUD_AUDIO
#define OPAUD_AUDIO

#include "Core/include/Target.h"
#include "Core/include/Types.h"
#include "Core/include/MathHelpers.h"
#include "Core/include/DynamicMemory.h"
#include "Core/include/Log.h"
#include "Data/include/OPfile.h"
#include "./Human/include/Math/Vector3.h"

#ifdef OPIFEX_ANDROID
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
#include "stdafx.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <WTypes.h>
#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
#include <string.h>
#include <AL/al.h>
#include <AL/alc.h>
#endif

#include <ogg/ogg.h>
#ifdef OPIFEX_ANDROID
#include "./Human/include/Utilities/vorbis/vorbisfile.h"
//#include "./Human/include/Utilities/ogg/ogg.h"
#else
#include <vorbis/vorbisfile.h>
//#include <ogg/ogg.h>
#endif


// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
//           _____ _____  __      __        _       _     _           
//     /\   |  __ \_   _| \ \    / /       (_)     | |   | |          
//    /  \  | |__) || |    \ \  / /_ _ _ __ _  __ _| |__ | | ___  ___ 
//   / /\ \ |  ___/ | |     \ \/ / _` | '__| |/ _` | '_ \| |/ _ \/ __|
//  / ____ \| |    _| |_     \  / (_| | |  | | (_| | |_) | |  __/\__ \
// /_/    \_\_|   |_____|     \/ \__,_|_|  |_|\__,_|_.__/|_|\___||___/
#ifdef OPIFEX_ANDROID
#define AL_FORMAT_MONO16 0
#define AL_FORMAT_STEREO16 1

extern SLObjectItf SLES_engineObject;
extern SLEngineItf SLES_engineEngine;
extern SLObjectItf SLES_outputMixObject;
#else
extern ALCdevice*  AL_OPaudioDevice;
extern ALCcontext* AL_OPaudioContext;
#endif
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//   ____                ______                _   _                 
//  / __ \              |  ____|              | | (_)                
// | |  | | __ _  __ _  | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
// | |  | |/ _` |/ _` | |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | |__| | (_| | (_| | | |  | |_| | | | | (__| |_| | (_) | | | \__ \
//  \____/ \__, |\__, | |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//          __/ | __/ |                                              
//         |___/ |___/                                               
typedef int (*LPOVCLEAR)(OggVorbis_File *vf);
typedef long (*LPOVREAD)(OggVorbis_File *vf,char *buffer,int length,int bigendianp,int word,int sgned,int *bitstream);
typedef ogg_int64_t (*LPOVPCMTOTAL)(OggVorbis_File *vf,int i);
typedef vorbis_info * (*LPOVINFO)(OggVorbis_File *vf,int link);
typedef vorbis_comment * (*LPOVCOMMENT)(OggVorbis_File *vf,int link);
typedef int (*LPOVOPENCALLBACKS)(void *datasource, OggVorbis_File *vf,char *initial, long ibytes, ov_callbacks callbacks);

extern LPOVCLEAR           fn_ov_clear;
extern LPOVREAD            fn_ov_read;
extern LPOVPCMTOTAL        fn_ov_pcm_total;
extern LPOVINFO            fn_ov_info;
extern LPOVCOMMENT         fn_ov_comment;
extern LPOVOPENCALLBACKS   fn_ov_open_callbacks;
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//   ____  _____                _ _         ______                _   _                 
//  / __ \|  __ \              | (_)       |  ____|              | | (_)                
// | |  | | |__) |_ _ _   _  __| |_  ___   | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
// | |  | |  ___/ _` | | | |/ _` | |/ _ \  |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | |__| | |  | (_| | |_| | (_| | | (_) | | |  | |_| | | | | (__| |_| | (_) | | | \__ \
//  \____/|_|   \__,_|\__,_|\__,_|_|\___/  |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPint OPaudInit();

Vector3 OPaudEarPosition(Vector3 pos);
Vector3 OPaudEarVelocity(Vector3 pos);
Vector3 OPaudEarForward(Vector3 pos);
//-----------------------------------------------------------------------------
#ifdef __cplusplus
};
#endif
#endif