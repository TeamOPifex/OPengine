#ifndef OPENGINE_PIPELINE_FMOD
#define OPENGINE_PIPELINE_FMOD

#include "./Core/include/OPtypes.h"
#ifdef OPIFEX_OPTION_FMOD
	#include "./fmod.hpp"
#endif

struct OPfmod {
	#ifdef OPIFEX_OPTION_FMOD
	FMOD::System *System;
	#endif
};

struct OPfmodSound {
	#ifdef OPIFEX_OPTION_FMOD
	FMOD::Sound *Sound;
	#endif
};

struct OPfmodChannel {
#ifdef OPIFEX_OPTION_FMOD
	FMOD::Channel *Channel;
#endif
};

extern OPfmod CURRENT_FMOD;

void OPfmodInit();
void OPfmodLoad(OPfmodSound* sound, OPchar* name);
OPfmodChannel OPfmodPlay(OPfmodSound* sound);
void OPfmodUpdate();

inline OPint OPfmodIsPlaying(OPfmodChannel channel) {
	bool isPlaying;
#ifdef OPIFEX_OPTION_FMOD
	channel.Channel->isPlaying(&isPlaying);
#endif
	return isPlaying;
}

#endif