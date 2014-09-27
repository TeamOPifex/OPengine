#include "./Human/include/Audio/AudioSource.h"


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
OPint OPaudOpenWave(const OPchar* filename, OPaudioSource** source){
	OPstream* str = OPreadFileLarge(filename, 1024);

	if(!str) OPlog("Error: couldn't open '%s'\n", filename);
	else{
		ui8* type;
		OPaudioDescription desc = { 0 };


		type = OPread(str, sizeof(i8) * 4);
		if(memcmp(type, "RIFF", 4) != 0){
			OPlog("No RIFF\n");
		}

		OPmemcpy(&desc.Length, OPread(str, sizeof(i32)), sizeof(i32));
		type = OPread(str, sizeof(i8) * 4);
		if(memcmp(type, "WAVE", 4) != 0){
			OPlog((char const*)type);
			OPlog("Not WAVE\n");
		}

		type = OPread(str, sizeof(i8) * 4);
		if(memcmp(type, "fmt ", 4) != 0){
			OPlog("Not fmt\n");
		}

		OPread(str, sizeof(i32)); // throw away chunksize //OPmemcpy(&chunkSize, ), sizeof(i32));
		OPmemcpy(&desc.Format, OPread(str, sizeof(i16)), sizeof(i16));
		OPmemcpy(&desc.Channels, OPread(str, sizeof(i16)), sizeof(i16));
		OPmemcpy(&desc.SamplesPerSecond, OPread(str, sizeof(i32)), sizeof(i32));
		OPread(str, sizeof(i32)); // throw away avg Bytes/sec , sizeof(i32));
		OPread(str, sizeof(i16)); // throw away bytes/sample
		OPmemcpy(&desc.BitsPerSample, OPread(str, sizeof(i16)), sizeof(i16));

		//OPlog("Format: %d\nChannels: %d\nSamples/Sec: %d\n", desc.Format, desc.Channels, desc.SamplesPerSecond);

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
			OPlog("Missing data\n");
		}
		OPmemcpy(&desc.Length, OPread(str, sizeof(ui32)), sizeof(ui32));

		OPaudioSource src = {
			OPaudReadWave,
			OPaudSeekWave,
			desc,
			str
		};

		// allocate space for the source to be returned
		*source = (OPaudioSource*)OPalloc(sizeof(OPaudioSource));
		OPmemcpy(*source, &src, sizeof(OPaudioSource));

		return 1;
	}

	source = NULL;
	return 0;
}

OPint OPaudOpenOgg(const OPchar* filename, OPaudioSource** source){
	// Open Ogg Stream
	//ov_callbacks	sCallbacks;
	OggVorbis_File*	sOggVorbisFile = (OggVorbis_File*)OPalloc(sizeof(OggVorbis_File));
	vorbis_info		*psVorbisInfo;

	*sOggVorbisFile = OggVorbis_File();

	//sCallbacks.read_func = ov_read_func;
	//sCallbacks.seek_func = ov_seek_func;
	//sCallbacks.close_func = ov_close_func;
	//sCallbacks.tell_func = ov_tell_func;

	
	//if (ov_open_callbacks(song, sOggVorbisFile, NULL, 0, sCallbacks) == 0){
#ifndef OPIFEX_ANDROID
	ov_fopen(filename, sOggVorbisFile);
#else
	FILE* song = OPreadFileInformation(filename).file;
	ov_open(song, sOggVorbisFile, NULL, 0);
#endif
			psVorbisInfo = ov_info(sOggVorbisFile, -1);
			if(psVorbisInfo){
				OPaudioDescription desc = {
					(ui64)ov_pcm_total(sOggVorbisFile, -1),
					(ui32)psVorbisInfo->rate,
					0,                      // bits / sample
					(ui16)psVorbisInfo->channels,
					0                       // format
				};
				OPaudioSource src = {
					OPaudReadOgg,
					OPaudSeekOgg,
					{0},
					NULL
				};
#ifdef OPIFEX_ANDROID
				switch(psVorbisInfo->channels){
					case 1:
						desc.BitsPerSample = 16;
						break;
					case 2:
						desc.BitsPerSample = 32;
						break;
					case 4:
						desc.BitsPerSample = 64;
						break;
					case 6:
						desc.BitsPerSample = 96;
						break;
				}
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

				//printf("Len: %lu\nSampleRate: %u\nChann: %u\n", desc.Length, desc.SamplesPerSecond, desc.Channels);

				*source = (OPaudioSource*)OPalloc(sizeof(OPaudioSource));
				OPmemcpy(*source, &src, sizeof(OPaudioSource));

				return 1;
			}
		

	source = NULL;
	return 0;
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
	if(OPstreamDestroy((OPstream*)src->DataSource)){
		OPfree(src);
		return 1;
	}
	return 0;
}

OPint OPaudCloseOgg (OPaudioSource* src){
	if(!ov_clear((OggVorbis_File*)src->DataSource)){
		OPfree(src);
		return 1;
	}
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
OPint OPaudReadWave(OPaudioSource* src, ui64* position, ui8* dest, ui32 len){
	ui64 pos = *position, srcLen = src->Description.Length, leng = (ui64)len;

	// clip length to read if we are at / near the end
	leng = pos + leng >= srcLen ? srcLen - pos : leng;
	OPstream* str = (OPstream*)src->DataSource;
	OPmemcpy(dest, OPreadAt(str, pos + WAVE_HEADER, leng), leng);
	*position += leng;

	return leng;
}

OPint OPaudReadOgg (OPaudioSource* src, ui64* position, ui8* dest, ui32 len){
	ui32 origLen = len, decoded = 0;
	i32 current_section;
	OggVorbis_File* sOggVorbisFile = (OggVorbis_File*)src->DataSource;

	while(decoded <= 2048){
		(*position) += (len = ov_read(
			sOggVorbisFile,
			(char*)(dest + decoded),
			len,
			0,
			2,
			1,
			&current_section
		));
		if(!len) break;
		decoded += len;
	}
	
	return decoded;
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
OPint OPaudSeekWave(OPaudioSource* src, ui64* pos){

	//*pos += WAVE_HEADER;

	// make sure it is not negative / out of bounds
	if(*pos >= src->Description.Length) return -1;
	return 1;
}

OPint OPaudSeekOgg (OPaudioSource* src, ui64* pos){
	OggVorbis_File* sOggVorbisFile = (OggVorbis_File*)src->DataSource;

	return ov_raw_seek(
		sOggVorbisFile,
		*pos
	);
}
//-----------------------------------------------------------------------------
