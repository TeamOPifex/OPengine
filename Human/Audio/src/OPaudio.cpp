#include "../include/OPaudio.h"

#define OPAUDIO_SWAP(s1, s2){\
	short sTemp = s1;\
	s1 = s2;\
	s2 = sTemp;\
}\

static int OPAudio::Init(){
	// setup the device and stuff
	_OPaudioDevice = alcOpenDevice(NULL);
	if(!_OPaudioDevice) return 0;
	_OPaudioContext = alcCreateContext(_OPaudioDevice, NULL);
	alcMakeContextCurrent(_OPaudioContext);
	if(!_OPaudioContext) return -1;

	alDistanceModel(AL_LINEAR_DISTANCE);

#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	HINSTANCE _hVorbisFileDLL = LoadLibrary(L"vorbisfile.dll");
	if(_hVorbisFileDLL){
		fn_ov_clear = (LPOVCLEAR)GetProcAddress(_hVorbisFileDLL, "ov_clear");
		fn_ov_read = (LPOVREAD)GetProcAddress(_hVorbisFileDLL, "ov_read");
		fn_ov_pcm_total = (LPOVPCMTOTAL)GetProcAddress(_hVorbisFileDLL, "ov_pcm_total");
		fn_ov_info = (LPOVINFO)GetProcAddress(_hVorbisFileDLL, "ov_info");
		fn_ov_comment = (LPOVCOMMENT)GetProcAddress(_hVorbisFileDLL, "ov_comment");
		fn_ov_open_callbacks = (LPOVOPENCALLBACKS)GetProcAddress(_hVorbisFileDLL, "ov_open_callbacks");

		if (!(fn_ov_clear && fn_ov_read && fn_ov_pcm_total && fn_ov_info &&
			fn_ov_comment && fn_ov_open_callbacks)){
			return -2;
		}
	}
#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	// copy the function pointers directly from
	// the linked SO file.
	fn_ov_clear = (LPOVCLEAR)ov_clear;
	fn_ov_read = (LPOVREAD)ov_read;
	fn_ov_pcm_total = (LPOVPCMTOTAL)ov_pcm_total;
	fn_ov_info = (LPOVINFO)ov_info;
	fn_ov_comment = (LPOVCOMMENT)ov_comment;
	fn_ov_open_callbacks = (LPOVOPENCALLBACKS)ov_open_callbacks;
#endif
	return 1;
}
/*---------------------------------------------------------------------------*/
OPsound OPAudio::ReadWave(const char* filename){
	FILE* fp = NULL;
	OPsound ERR = {0};
	fp = fopen(filename, "rb");

	if(!fp) printf("Error: couldn't open '%s'\n", filename);
	else{
		char type[4];
		long size, chunkSize;
		short formatType, channels;
		long sampleRate, avgBytesPerSec;
		short bytesPerSample, bitsPerSample;
		long dataSize;

		fread(type, sizeof(char), 4, fp);
		if(memcmp(type, "RIFF", 4) != 0){
			printf("No RIFF\n");
		}

		fread(&size, sizeof(long), 1, fp);
		fread(type, sizeof(char), 4, fp);
		if(memcmp(type, "WAVE", 4) != 0){
			printf("Not WAVE\n");
		}

		fread(type, sizeof(char), 4, fp);
		if(memcmp(type, "fmt ", 4) != 0){
			printf("Not fmt\n");
		}

		fread(&chunkSize, sizeof(long), 1, fp);
		fread(&formatType, sizeof(short), 1, fp);
		fread(&channels, sizeof(short), 1, fp);
		fread(&sampleRate, sizeof(long), 1, fp);
		fread(&avgBytesPerSec, sizeof(long), 1, fp);
		fread(&bytesPerSample, sizeof(short), 1, fp);
		fread(&bitsPerSample, sizeof(short), 1, fp);

		printf("Chunk Size: %d\nChannels: %d\nSample Rate: %d\nBytes/Sec: %d\n",
			(int)chunkSize, (int)channels, (int)sampleRate, (int)avgBytesPerSec);

		fread(type, sizeof(char), 4, fp);
		if(memcmp(type, "data", 4) != 0){
			printf("Missing data\n");
		}

		fread(&dataSize, sizeof(long), 1, fp);

		unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * dataSize);
		fread(data, dataSize, 1, fp);

		fclose(fp);

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
			dataSize,
			data
		};

		return out;
	}


	return ERR;
}
/*---------------------------------------------------------------------------*/
unsigned long DecodeOggVorbis(OggVorbis_File *psOggVorbisFile, char *pDecodeBuffer, unsigned long ulBufferSize, unsigned long ulChannels){
	int current_section;
	long lDecodeSize;
	unsigned long ulSamples;
	short *pSamples;

	unsigned long ulBytesDone = 0;
	while (1)
	{
		lDecodeSize = fn_ov_read(psOggVorbisFile, pDecodeBuffer + ulBytesDone, ulBufferSize - ulBytesDone, 0, 2, 1, &current_section);
		if (lDecodeSize > 0)
		{
			ulBytesDone += lDecodeSize;

			if (ulBytesDone >= ulBufferSize)
				break;
		}
		else
		{
			break;
		}
	}

	// Mono, Stereo and 4-Channel files decode into the same channel order as WAVEFORMATEXTENSIBLE,
	// however 6-Channels files need to be re-ordered
	if (ulChannels == 6){		
		pSamples = (short*)pDecodeBuffer;
		for (ulSamples = 0; ulSamples < (ulBufferSize>>1); ulSamples+=6){
			// WAVEFORMATEXTENSIBLE Order : FL, FR, FC, LFE, RL, RR
			// OggVorbis Order            : FL, FC, FR,  RL, RR, LFE
			OPAUDIO_SWAP(pSamples[ulSamples+1], pSamples[ulSamples+2]);
			OPAUDIO_SWAP(pSamples[ulSamples+3], pSamples[ulSamples+5]);
			OPAUDIO_SWAP(pSamples[ulSamples+4], pSamples[ulSamples+5]);
		}
	}

	return ulBytesDone;
}
/*---------------------------------------------------------------------------*/
int fetchOggData(OPsound* sound, long pos, long len){
	OggVorbis_File* ogg = (OggVorbis_File*)sound->dataSource;

	long length = DecodeOggVorbis(ogg, (char*)sound->Data, sound->DataSize, sound->Channels);
	//printf("Song length: %l\n", length);

	printf("Fetched %d\n", (int)length);

	return length;
}
/*---------------------------------------------------------------------------*/
OPsound OPAudio::ReadOgg(const char* filename){
	// Open Ogg Stream
	ov_callbacks	sCallbacks;
	OggVorbis_File	*sOggVorbisFile = new OggVorbis_File();
	vorbis_info		*psVorbisInfo;

	sCallbacks.read_func = ov_read_func;
	sCallbacks.seek_func = ov_seek_func;
	sCallbacks.close_func = ov_close_func;
	sCallbacks.tell_func = ov_tell_func;

	FILE* song = fopen(filename, "rb");

	if(song){
		printf("Song loaded!\n");
		// Create an OggVorbis file stream
		if (fn_ov_open_callbacks(song, sOggVorbisFile, NULL, 0, sCallbacks) == 0){
			long ulFrequency = 0, ulBufferSize = 0;
			int ulChannels = 0, ulFormat = 0, bitsPerSample = 0;


			// Get some information about the file (Channels, Format, and Frequency)
			psVorbisInfo = fn_ov_info(sOggVorbisFile, -1);
			if (psVorbisInfo)
			{
				ulFrequency = psVorbisInfo->rate;
				ulChannels = psVorbisInfo->channels;
				if (psVorbisInfo->channels == 1)
				{
					ulFormat = AL_FORMAT_MONO16;
					printf("Format AL_FORMAT_MONO16\n");
					// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
					ulBufferSize = ulFrequency >> 1;
					// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
					ulBufferSize -= (ulBufferSize % 2);
					bitsPerSample = 16;
				}
				else if (psVorbisInfo->channels == 2)
				{
					ulFormat = AL_FORMAT_STEREO16;
					printf("Format AL_FORMAT_STEREO16\n");
					// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
					ulBufferSize = ulFrequency;
					// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
					ulBufferSize -= (ulBufferSize % 4);
					bitsPerSample = 32;
				}
				else if (psVorbisInfo->channels == 4)
				{
					ulFormat = alGetEnumValue("AL_FORMAT_QUAD16");
					// Set BufferSize to 250ms (Frequency * 8 (16bit 4-channel) divided by 4 (quarter of a second))
					ulBufferSize = ulFrequency * 2;
					// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
					ulBufferSize -= (ulBufferSize % 8);
				}
				else if (psVorbisInfo->channels == 6)
				{
					ulFormat = alGetEnumValue("AL_FORMAT_51CHN16");
					// Set BufferSize to 250ms (Frequency * 12 (16bit 6-channel) divided by 4 (quarter of a second))
					ulBufferSize = ulFrequency * 3;
					// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
					ulBufferSize -= (ulBufferSize % 12);
				}

				printf("Buffer size: %d Channels: %d\n", (long)ulBufferSize, ulChannels);

				long length = fn_ov_pcm_total(sOggVorbisFile, -1);
				printf("Song length: %d\n", length);

				//length = ulFrequency * 2;
				unsigned char* buff = new unsigned char[length];
				DecodeOggVorbis(sOggVorbisFile, (char*)buff, length, ulChannels);

				OPsound song = {
					ulFrequency,
					bitsPerSample,
					ulChannels,
					ulFormat,
					(void*)sOggVorbisFile,
					fetchOggData,
					length,
					buff
				};

				return song;
			}
		}
	}
}

