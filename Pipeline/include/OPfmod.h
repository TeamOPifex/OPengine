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

extern OPfmod CURRENT_FMOD;

void OPfmodInit();
void OPfmodLoad(OPfmodSound* sound, OPchar* name);
void OPfmodPlay(OPfmodSound* sound);
void OPfmodUpdate();

#endif