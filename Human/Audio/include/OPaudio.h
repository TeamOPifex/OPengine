#pragma once

#include "Core/include/Target.h"
#include "Core/include/DynamicMemory.h"

#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
#include "stdafx.h"
#include "vorbisfile.h"
#include "al.h"
#include "alc.h"
#include <WTypes.h>
#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
#include <string.h>
#include <AL/al.h>
#include <AL/alc.h>
#endif

#include "External/Ogg/include/ogg.h"
#include "External/Ogg/include/vorbisfile.h"

extern "C"{
	static ALCdevice* _OPaudioDevice;
	static ALCcontext* _OPaudioContext;

	// Function pointers
	typedef int (*LPOVCLEAR)(OggVorbis_File *vf);
	typedef long (*LPOVREAD)(OggVorbis_File *vf,char *buffer,int length,int bigendianp,int word,int sgned,int *bitstream);
	typedef ogg_int64_t (*LPOVPCMTOTAL)(OggVorbis_File *vf,int i);
	typedef vorbis_info * (*LPOVINFO)(OggVorbis_File *vf,int link);
	typedef vorbis_comment * (*LPOVCOMMENT)(OggVorbis_File *vf,int link);
	typedef int (*LPOVOPENCALLBACKS)(void *datasource, OggVorbis_File *vf,char *initial, long ibytes, ov_callbacks callbacks);


	// Variables
	static LPOVCLEAR           fn_ov_clear;
	static LPOVREAD            fn_ov_read;
	static LPOVPCMTOTAL        fn_ov_pcm_total;
	static LPOVINFO            fn_ov_info;
	static LPOVCOMMENT         fn_ov_comment;
	static LPOVOPENCALLBACKS   fn_ov_open_callbacks;
};

struct OPsound{
	ALuint SampleRate;
	ALuint BitsPerSample;
	ALuint Channels;
	ALenum Format;
	void* dataSource;
	int (*FillCallback)(OPsound* sound, long position, long length);
	long DataSize;
	unsigned char* Data;
};

class OPAudio{
	public:
		static int Init();

		static void SetEarPosition(ALfloat* pos){
			alListenerfv(AL_POSITION, pos);
		}

		static void SetEarVelocity(ALfloat* velo){
			alListenerfv(AL_VELOCITY, velo);
		}

		static void SetEarForward(ALfloat* forward){
			alListenerfv(AL_ORIENTATION, forward);
		}

		static OPsound ReadWave(const char* filename);
		static OPsound ReadOgg(const char* filename);
};
