#include "./Pipeline/include/OPfmod.h"

#ifdef OPIFEX_OPTION_FMOD

#include "./Core/include/OPlog.h"
#include "./Core/include/Assert.h"

OPfmod* CURRENT_FMOD;

void OPfmodInit() {

		FMOD_RESULT result;
		unsigned int version;

		result = FMOD::System_Create(&CURRENT_FMOD);
		OPlog("FMod Initialized %d", result);

		result = CURRENT_FMOD->getVersion(&version);
		OPlog("FMod Get Version %d", result);

		OPlog("FMod Version %u", version);
		OPlog("FMod Header Version %u", FMOD_VERSION);

		ASSERT(version >= FMOD_VERSION, "FMOD lib version doesn't match header version");

		result = CURRENT_FMOD->init(32, FMOD_INIT_NORMAL, 0);
}

OPfmodSound* OPfmodLoad(OPchar* name) {
	OPfmodSound* result = NULL;
	CURRENT_FMOD->createSound(name, FMOD_DEFAULT, 0, &result);
    return result;
}

OPfmodChannel* OPfmodPlay(OPfmodSound* sound) {
    OPfmodChannel* result = NULL;
		CURRENT_FMOD->playSound(sound, 0, false, &result);
	return result;
}

OPfmodChannelGroup* OPfmodCreateChannelGroup(const OPchar* name) {
	OPfmodChannelGroup* result = NULL;
		CURRENT_FMOD->createChannelGroup(name, &result);
	return result;
}

OPfmodChannel* OPfmodPlay(OPfmodSound* sound, OPfmodChannelGroup* group) {
	OPfmodChannel* result = NULL;		CURRENT_FMOD->playSound(sound, group, true, &result);
	return result;
}

void OPfmodUpdate() {
	CURRENT_FMOD->update();
}

void OPfmodSetVolume(OPfmodChannel* channel, OPfloat volume) {
	channel->setVolume(volume);
}

void OPfmodSetVolume(OPfmodChannelGroup* group, OPfloat volume) {
	group->setVolume(volume);
}
#endif
