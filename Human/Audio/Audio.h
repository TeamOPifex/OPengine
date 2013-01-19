#pragma once

#include "./Core/include/Types.h"

#ifdef OPIFEX_ANDROID
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#else
// OpenAL stuff here
#endif

class Audio {
public:
	Audio(char* filename, bool loop);
	bool Pause();
	bool Stop();
	bool Play();
	bool SetVolume(i32 level);
	bool Looping();
	ui32 Loops();
	bool Done();
private:
	i32 _fd;
	bool _looping;
#ifdef OPIFEX_ANDROID
	SLObjectItf _fdPlayerObject;	
	SLPlayItf _fdPlayerPlay;
	SLSeekItf _fdPlayerSeek;
	SLVolumeItf _fdPlayerVolume;
#endif
};