#include "./Pipeline/include/OPfmod.h"

#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"

OPfmod CURRENT_FMOD;

void OPfmodInit() {

	#ifdef OPIFEX_OPTION_FMOD
		FMOD_RESULT result;
		unsigned int version;

		result = FMOD::System_Create(&CURRENT_FMOD.System);
		OPlog("FMod Initialized %d", result);

		result = CURRENT_FMOD.System->getVersion(&version);
		OPlog("FMod Get Version %d", result);

		OPlog("FMod Version %u", version);

		ASSERT(version >= FMOD_VERSION, "FMOD lib version doesn't match header version");

		result = CURRENT_FMOD.System->init(32, FMOD_INIT_NORMAL, 0);
    #else
	    OPlog("FMOD is not built into the engine");
    #endif
}

void OPfmodLoad(OPfmodSound* sound, OPchar* name) {
	#ifdef OPIFEX_OPTION_FMOD
	CURRENT_FMOD.System->createSound(name, FMOD_DEFAULT, 0, &sound->Sound);
	#endif
}

OPfmodChannel OPfmodPlay(OPfmodSound* sound) {
	OPfmodChannel result;
	#ifdef OPIFEX_OPTION_FMOD
		CURRENT_FMOD.System->playSound(sound->Sound, 0, false, &result.Channel);
	#endif
	return result;
}

void OPfmodUpdate() {
	#ifdef OPIFEX_OPTION_FMOD
	CURRENT_FMOD.System->update();
	#endif
}