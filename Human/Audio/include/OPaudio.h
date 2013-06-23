#pragma once

#include "Core/include/Target.h"
#include "Core/include/Types.h"
#include "Core/include/MathHelpers.h"
#include "Core/include/DynamicMemory.h"
#include "Data/include/OPfile.h"
#include "Human/Math/Vector3.h"

#include "./Core/include/Log.h"5

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
#include <vorbis/vorbisfile.h>

extern "C"{
#ifdef OPIFEX_ANDROID
	#define AL_FORMAT_MONO16 0
	#define AL_FORMAT_STEREO16 1

	static SLObjectItf _engineObject;
	static SLEngineItf _engineEngine;
	static SLObjectItf _outputMixObject;
#else
	static ALCdevice* _OPaudioDevice;
	static ALCcontext* _OPaudioContext;
#endif

	// Function pointers for OGG
	typedef int (*LPOVCLEAR)(OggVorbis_File *vf);
	typedef long (*LPOVREAD)(OggVorbis_File *vf,char *buffer,int length,int bigendianp,int word,int sgned,int *bitstream);
	typedef ogg_int64_t (*LPOVPCMTOTAL)(OggVorbis_File *vf,int i);
	typedef vorbis_info * (*LPOVINFO)(OggVorbis_File *vf,int link);
	typedef vorbis_comment * (*LPOVCOMMENT)(OggVorbis_File *vf,int link);
	typedef int (*LPOVOPENCALLBACKS)(void *datasource, OggVorbis_File *vf,char *initial, long ibytes, ov_callbacks callbacks);

	// Variables for OGG
	static LPOVCLEAR           fn_ov_clear;
	static LPOVREAD            fn_ov_read;
	static LPOVPCMTOTAL        fn_ov_pcm_total;
	static LPOVINFO            fn_ov_info;
	static LPOVCOMMENT         fn_ov_comment;
	static LPOVOPENCALLBACKS   fn_ov_open_callbacks;
};

struct OPsound{
#ifndef OPIFEX_ANDROID
	ALuint SampleRate;
	ALuint BitsPerSample;
	ALuint Channels;
	ALenum Format;
#else
	SLuint32 SampleRate;
	SLuint32 BitsPerSample;
	SLuint32 Channels;
	//ALenum Format;
	ui8 SLdataFormat[32];
#endif
	void* dataSource;
	OPint (*FillCallback)(OPsound* sound, i64 position, i64 length);
	void (*Reset)(OPsound* sound);
	i64 DataSize;
	ui8* Data;
};

class OPAudio{
	public:
		static OPint Init();

#ifdef OPIFEX_ANDROID		
		static SLEngineItf EngineEngine;
		static SLObjectItf EngineObject;
		static SLObjectItf OutputMixObject;
#endif

		static void SetEarPosition(Vector3 pos){
#ifdef OPIFEX_ANDROID
#else
			alListenerfv(AL_POSITION, pos.ptr());
#endif
		}

		static void SetEarVelocity(Vector3 velo){
#ifdef OPIFEX_ANDROID
#else
			alListenerfv(AL_VELOCITY, velo.ptr());
#endif
		}

		static void SetEarForward(Vector3 forward){
#ifdef OPIFEX_ANDROID
#else
			alListenerfv(AL_ORIENTATION, forward.ptr());
#endif
		}

		static OPsound ReadWave(const OPchar* filename);
		static OPsound ReadOgg(const OPchar* filename);

		static void Reset(OPsound* sound);
};
