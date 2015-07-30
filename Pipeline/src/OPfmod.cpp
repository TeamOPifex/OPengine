#include "./Pipeline/include/OPfmod.h"

#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"

OPfmod* CURRENT_FMOD;

void OPfmodInit() {

	#ifdef OPIFEX_OPTION_FMOD
		FMOD_RESULT result;
		unsigned int version;

		result = FMOD::System_Create(&CURRENT_FMOD);
		OPlog("FMod Initialized %d", result);

		result = CURRENT_FMOD->getVersion(&version);
		OPlog("FMod Get Version %d", result);

		OPlog("FMod Version %u", version);

		ASSERT(version >= FMOD_VERSION, "FMOD lib version doesn't match header version");

		result = CURRENT_FMOD->init(32, FMOD_INIT_NORMAL, 0);
    #else
	    OPlog("FMOD is not built into the engine");
    #endif
}

OPfmodSound* OPfmodLoad(OPchar* name) {
	OPfmodSound* result = NULL;
	#ifdef OPIFEX_OPTION_FMOD
	CURRENT_FMOD->createSound(name, FMOD_DEFAULT, 0, &result);
	#endif
	return result;
}

OPfmodChannel* OPfmodPlay(OPfmodSound* sound) {
	OPfmodChannel* result;
	#ifdef OPIFEX_OPTION_FMOD
		CURRENT_FMOD->playSound(sound, 0, false, &result);
	#endif
	return result;
}

OPfmodChannelGroup* OPfmodCreateChannelGroup(const OPchar* name) {
	OPfmodChannelGroup* result;
	#ifdef OPIFEX_OPTION_FMOD
		CURRENT_FMOD->createChannelGroup(name, &result);
	#endif
	return result;
}

OPfmodChannel* OPfmodPlay(OPfmodSound* sound, OPfmodChannelGroup* group) {
	OPfmodChannel* result;
	#ifdef OPIFEX_OPTION_FMOD
		CURRENT_FMOD->playSound(sound, group, true, &result);
	#endif
	return result;
}

void OPfmodUpdate() {
	#ifdef OPIFEX_OPTION_FMOD
	CURRENT_FMOD->update();
	#endif
}

void OPfmodSetVolume(OPfmodChannel* channel, OPfloat volume) {
	#ifdef OPIFEX_OPTION_FMOD
	channel->setVolume(volume);
	#endif
}

void OPfmodSetVolume(OPfmodChannelGroup* group, OPfloat volume) {
	#ifdef OPIFEX_OPTION_FMOD
	group->setVolume(volume);
	#endif
}
