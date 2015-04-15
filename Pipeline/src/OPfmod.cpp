#include "./Pipeline/include/OPfmod.h"

#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"

void OPfmodInit(OPfmod* fmod) {

	#ifdef OPIFEX_OPTION_FMOD
		FMOD_RESULT result;
		unsigned int version;

		result = FMOD::System_Create(&fmod->System);
		OPlog("FMod Initialized %d", result);

	    result = fmod->System->getVersion(&version);
		OPlog("FMod Get Version %d", result);

		OPlog("FMod Version %u", version);

		ASSERT(version >= FMOD_VERSION, "FMOD lib version doesn't match header version");

	    result = fmod->System->init(32, FMOD_INIT_NORMAL, 0);
    #else
	    OPlog("FMOD is not built into the engine");
    #endif
}

void OPfmodLoad(OPfmod* fmod, OPfmodSound* sound, OPchar* name) {
	#ifdef OPIFEX_OPTION_FMOD
		fmod->System->createSound(name, FMOD_DEFAULT, 0, &sound->Sound);
	#endif
}

void OPfmodPlay(OPfmod* fmod, OPfmodSound* sound) {
	#ifdef OPIFEX_OPTION_FMOD
		FMOD::Channel    *channel = 0;
		fmod->System->playSound(sound->Sound, 0, false, &channel);
	#endif
}

void OPfmodUpdate(OPfmod* fmod) {
	#ifdef OPIFEX_OPTION_FMOD
		fmod->System->update();
	#endif
}