#include "../include/OPaudioSource.h"


size_t ov_read_func(void *ptr, size_t size, size_t nmemb, void *datasource){
	return fread(ptr, size, nmemb, (FILE*)datasource);
}
int ov_seek_func(void *datasource, ogg_int64_t offset, int whence) {
	return fseek((FILE*)datasource, (long)offset, whence);
}
int ov_close_func(void *datasource){ return fclose((FILE*)datasource); }
long ov_tell_func(void *datasource){ return ftell((FILE*)datasource); }

//-----------------------------------------------------------------------------
//   ____                   _               ______                      
//  / __ \                 (_)             |  ____|                     
// | |  | |_ __   ___ _ __  _ _ __   __ _  | |__ _   _ _ __   ___ ___   
// | |  | | '_ \ / _ \ '_ \| | '_ \ / _` | |  __| | | | '_ \ / __/ __|  
// | |__| | |_) |  __/ | | | | | | | (_| | | |  | |_| | | | | (__\__ \_ 
//  \____/| .__/ \___|_| |_|_|_| |_|\__, | |_|   \__,_|_| |_|\___|___(_)
//        | |                        __/ |                              
//        |_|                       |___/                               
OPaudioSource OPaudOpenWave(const OPchar* filename){
	OPstream* str = OPreadFile(filename);

	if(!str) printf("Error: couldn't open '%s'\n", filename);
	else{
		ui8* type;
		OPaudioDescription desc = { 0 };


		type = OPread(str, sizeof(i8) * 4);
		if(memcmp(type, "RIFF", 4) != 0){
			OPLog("No RIFF\n");
		}

		OPmemcpy(&desc.Length, OPread(str, sizeof(i32)), sizeof(i32));
		type = OPread(str, sizeof(i8) * 4);
		if(memcmp(type, "WAVE", 4) != 0){
			OPLog((char const*)type);
			OPLog("Not WAVE\n");
		}

		type = OPread(str, sizeof(i8) * 4);
		if(memcmp(type, "fmt ", 4) != 0){
			OPLog("Not fmt\n");
		}

		OPread(str, sizeof(i32)); // throw away chunksize //OPmemcpy(&chunkSize, ), sizeof(i32));
		OPmemcpy(&desc.Format, OPread(str, sizeof(i16)), sizeof(i16));
		OPmemcpy(&desc.Channels, OPread(str, sizeof(i16)), sizeof(i16));
		OPmemcpy(&desc.SamplesPerSecond, OPread(str, sizeof(i32)), sizeof(i32));
		OPread(str, sizeof(i32)); // throw away avg Bytes/sec , sizeof(i32));
		OPread(str, sizeof(i16)); // throw away bytes/sample
		OPmemcpy(&desc.BitsPerSample, OPread(str, sizeof(i16)), sizeof(i16));

#ifdef OPIFEX_ANDROID

#else
		switch (desc.BitsPerSample)
		{
			case 8:
				desc.Format = (desc.Channels == 2 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8);
				break;
			case 16:
				desc.Format = (desc.Channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16);
				break;
			default:
				break;
		}
#endif

		type = OPread(str, sizeof(i8) * 4);
		if(memcmp(type, "data", 4) != 0){
			OPLog("Missing data\n");
		}

		OPmemcpy(&desc.Length, OPread(str, sizeof(i32)), sizeof(i32));

		OPaudioSource source = {
			OPaudReadWave,
			OPaudSeekWave,
			0,   // position,
			desc,
			str
		};

		return source;
	}

	OPaudioSource err = {
		NULL,
		NULL,
		0,   // position,
		{0},
		NULL
	};

	return err;
}

OPaudioSource OPaudOpenOgg (const OPchar* filename){
	// Open Ogg Stream
	ov_callbacks	sCallbacks;
	OggVorbis_File	*sOggVorbisFile = new OggVorbis_File();
	vorbis_info		*psVorbisInfo;

	sCallbacks.read_func = ov_read_func;
	sCallbacks.seek_func = ov_seek_func;
	sCallbacks.close_func = ov_close_func;
	sCallbacks.tell_func = ov_tell_func;

	FILE* song = fopen(filename, "rb");

	if (fn_ov_open_callbacks(song, sOggVorbisFile, NULL, 0, sCallbacks) == 0)
			if(psVorbisInfo){
				OPaudioDescription desc = {
					fn_ov_pcm_total(sOggVorbisFile, -1),
					psVorbisInfo->rate,
					0,                      // bits / sample
					psVorbisInfo->channels,
					0                       // format
				};
				OPaudioSource src = {
					OPaudReadOgg,
					OPaudSeekOgg,
					0,
					{0},
					NULL
				};
#ifdef OPIFEX_ANDROID

#else
			switch(psVorbisInfo->channels){
				case 1:
					desc.Format = AL_FORMAT_MONO16;
					desc.BitsPerSample = 16;
					break;
				case 2:
					desc.Format = AL_FORMAT_STEREO16;
					desc.BitsPerSample = 32;
					break;
				case 4:
					desc.Format = alGetEnumValue("AL_FORMAT_QUAD16");
					desc.BitsPerSample = 64;
					break;
				case 6:
					desc.Format = alGetEnumValue("AL_FORMAT_51CHN16");
					desc.BitsPerSample = 96;
					break;
			}
#endif
			src.Description = desc;
			src.DataSource  = sOggVorbisFile;
		}
}
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//   _____ _           _               ______                      
//  / ____| |         (_)             |  ____|                     
// | |    | | ___  ___ _ _ __   __ _  | |__ _   _ _ __   ___ ___   
// | |    | |/ _ \/ __| | '_ \ / _` | |  __| | | | '_ \ / __/ __|  
// | |____| | (_) \__ \ | | | | (_| | | |  | |_| | | | | (__\__ \_ 
//  \_____|_|\___/|___/_|_| |_|\__, | |_|   \__,_|_| |_|\___|___(_)
//                              __/ |                              
//                             |___/                               
OPint OPaudCloseWave(OPaudioSource* src){
	return OPstreamDestroy((OPstream*)src->DataSource);
}

OPint OPaudCloseOgg (OPaudioSource* src){
	return 0;
}
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//  _____                _ _               ______                _   _                 
// |  __ \              | (_)             |  ____|              | | (_)                
// | |__) |___  __ _  __| |_ _ __   __ _  | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
// |  _  // _ \/ _` |/ _` | | '_ \ / _` | |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | | \ \  __/ (_| | (_| | | | | | (_| | | |  | |_| | | | | (__| |_| | (_) | | | \__ \
// |_|  \_\___|\__,_|\__,_|_|_| |_|\__, | |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                                  __/ |                                              
//                                 |___/                                               
OPint OPaudReadWave(OPaudioSource* src, ui8* dest, ui32 len){
	ui64 pos = src->Progress, srcLen = src->Description.Length;

	// clip length to read if we are at / near the end
	len = pos + len >= srcLen ? srcLen - pos : len;

	OPstream* str = (OPstream*)src->DataSource;
	OPmemcpy(dest, OPreadAt(str, pos + WAVE_HEADER, len), len);

	src->Progress += len;

	return len;
}

OPint OPaudReadOgg (OPaudioSource* src, ui8* dest, ui32 len){
	OPint current_section;
	i64 lDecodeSize;
	ui64 ulSamples;
	i16 *pSamples;
	OggVorbis_File* sOggVorbisFile = (OggVorbis_File*)src->DataSource;

	src->Progress += (len = fn_ov_read(
		sOggVorbisFile,
		dest,
		len,
		0,
		2,
		1,
		&current_section
	));

	// Mono, Stereo and 4-Channel files decode into the same channel order as WAVEFORMATEXTENSIBLE,
	// however 6-Channels files need to be re-ordered
	// if (ulChannels == 6){		
	// 	pSamples = (i16*)pDecodeBuffer;
	// 	for (ulSamples = 0; ulSamples < (ulBufferSize>>1); ulSamples+=6){
	// 		// WAVEFORMATEXTENSIBLE Order : FL, FR, FC, LFE, RL, RR
	// 		// OggVorbis Order            : FL, FC, FR,  RL, RR, LFE
	// 		OPAUDIO_SWAP(pSamples[ulSamples+1], pSamples[ulSamples+2]);
	// 		OPAUDIO_SWAP(pSamples[ulSamples+3], pSamples[ulSamples+5]);
	// 		OPAUDIO_SWAP(pSamples[ulSamples+4], pSamples[ulSamples+5]);
	// 	}
	// }
	return len;
}
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//   _____           _    _               ______                _   _                 
//  / ____|         | |  (_)             |  ____|              | | (_)                
// | (___   ___  ___| | ___ _ __   __ _  | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//  \___ \ / _ \/ _ \ |/ / | '_ \ / _` | |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//  ____) |  __/  __/   <| | | | | (_| | | |  | |_| | | | | (__| |_| | (_) | | | \__ \
// |_____/ \___|\___|_|\_\_|_| |_|\__, | |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                                 __/ |                                              
//                                |___/                                               
OPint OPaudSeekWave(OPaudioSource* src, ui64 pos){
	pos += WAVE_HEADER;

	// make sure it is not negative / out of bounds
	if(pos >= src->Description.Length) return -1;

	src->Progress = pos;
	return 1;
}

OPint OPaudSeekOgg (OPaudioSource* src, ui64 pos){
	OggVorbis_File* sOggVorbisFile = (OggVorbis_File*)src->DataSource;
	

	return 0;
}
//-----------------------------------------------------------------------------
