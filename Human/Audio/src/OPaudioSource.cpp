
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
	// TODO all of this VVVVVVVVVVVVVVV
	//FILE* fp = NULL;
	OPsound ERR = {0};
	//fp = fopen(filename, "rb");
	OPstream* str = OPreadFile(filename);

	if(!str) printf("Error: couldn't open '%s'\n", filename);
	else{
		ui8* type;
		i32 size = 0, chunkSize = 0;
		i16 formatType = 0, channels = 0;
		i32 sampleRate = 0, avgBytesPerSec = 0;
		i16 bytesPerSample = 0, bitsPerSample = 0;
		i32 dataSize = 0;

		type = OPread(str, sizeof(i8) * 4);
		if(memcmp(type, "RIFF", 4) != 0){
			OPLog("No RIFF\n");
		}

		OPmemcpy(&size, OPread(str, sizeof(i32)), sizeof(i32));
		type = OPread(str, sizeof(i8) * 4);
		if(memcmp(type, "WAVE", 4) != 0){
			OPLog((char const*)type);
			OPLog("Not WAVE\n");
		}

		type = OPread(str, sizeof(i8) * 4);
		if(memcmp(type, "fmt ", 4) != 0){
			OPLog("Not fmt\n");
		}

		OPmemcpy(&chunkSize, OPread(str, sizeof(i32)), sizeof(i32));
		OPmemcpy(&formatType, OPread(str, sizeof(i16)), sizeof(i16));
		OPmemcpy(&channels, OPread(str, sizeof(i16)), sizeof(i16));
		OPmemcpy(&sampleRate, OPread(str, sizeof(i32)), sizeof(i32));
		OPmemcpy(&avgBytesPerSec, OPread(str, sizeof(i32)), sizeof(i32));
		OPmemcpy(&bytesPerSample, OPread(str, sizeof(i16)), sizeof(i16));
		OPmemcpy(&bitsPerSample, OPread(str, sizeof(i16)), sizeof(i16));

		printf("Chunk Size: %d\nChannels: %d\nSample Rate: %d\nBytes/Sec: %d\n",
			(OPint)chunkSize, (OPint)channels, (OPint)sampleRate, (OPint)avgBytesPerSec);

		type = OPread(str, sizeof(i8) * 4);
		if(memcmp(type, "data", 4) != 0){
			OPLog("Missing data\n");
		}

		OPmemcpy(&dataSize, OPread(str, sizeof(i32)), sizeof(i32));

		ui8* data = (ui8*)OPalloc(sizeof(ui8) * dataSize);
		OPmemcpy(data, OPread(str, dataSize), dataSize);

		OPstreamDestroy(str);

#ifdef OPIFEX_ANDROID
		OPsound out = {  
			(SLuint32)sampleRate,
			(SLuint32)bitsPerSample,
			(SLuint32)channels, 
			{0},
			NULL,
			NULL,
			NULL,
			dataSize,
			data
		};

		OPLog("Channels:"); OPLog_i32(channels);
		OPLog("sampleRate:"); OPLog_i32(sampleRate);
		OPLog("bps:"); OPLog_i32(avgBytesPerSec);

		SLDataFormat_PCM slFormat = {
			SL_DATAFORMAT_PCM, // specifies the format
			(SLuint32)channels,   
			(SLuint32)sampleRate,
			(SLuint32)bitsPerSample,
			OPceil(bitsPerSample / 32) * 32,
			SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
			SL_BYTEORDER_LITTLEENDIAN
		};

		// keep the SL data format in the OPsound
		*((SLDataFormat_PCM*)out.SLdataFormat) = slFormat;
#else
		ALenum format = 0;

		switch (bitsPerSample)
		{
			case 8:
				format = (channels == 2 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8);
				break;
			case 16:
				format = (channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16);
				break;
			default:
				break;
		}

		OPsound out = {
			sampleRate,
			bitsPerSample,
			channels,
			format,
			NULL,
			NULL,
			NULL,
			dataSize,
			data
		};
#endif

		return out;
	}


	return ERR;
}
OPaudioSource OPaudOpenOgg (const OPchar* filename){

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

}
OPint OPaudCloseOgg (OPaudioSource* src){

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

}
OPint OPaudReadOgg (OPaudioSource* src, ui8* dest, ui32 len){

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

}
OPint OPaudSeekOgg (OPaudioSource* src, ui64 pos){

}
//-----------------------------------------------------------------------------
