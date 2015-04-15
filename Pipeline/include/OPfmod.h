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

void OPfmodInit(OPfmod* fmod);
void OPfmodLoad(OPfmod* fmod, OPfmodSound* sound, OPchar* name);
void OPfmodPlay(OPfmod* fmod, OPfmodSound* sound);
void OPfmodUpdate(OPfmod* fmod);

#endif