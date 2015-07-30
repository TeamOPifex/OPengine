#ifndef OPENGINE_PIPELINE_FMOD
#define OPENGINE_PIPELINE_FMOD

#include "./Core/include/OPtypes.h"
#ifdef OPIFEX_OPTION_FMOD
	#include "./fmod.hpp"
#endif

#ifdef OPIFEX_OPTION_FMOD
typedef FMOD::ChannelGroup OPfmodChannelGroup;
typedef FMOD::System OPfmod;
typedef FMOD::Sound OPfmodSound;
typedef FMOD::Channel OPfmodChannel;
#else
typedef void OPfmodChannelGroup;
typedef void OPfmod;
typedef void OPfmodSound;
typedef void OPfmodChannel;
#endif

extern OPfmod* CURRENT_FMOD;

void OPfmodInit();
OPfmodSound* OPfmodLoad(OPchar* name);
OPfmodChannel* OPfmodPlay(OPfmodSound* sound);
OPfmodChannelGroup* OPfmodCreateChannelGroup(const OPchar* name);
OPfmodChannel* OPfmodPlay(OPfmodSound* sound, OPfmodChannelGroup* group);
void OPfmodSetVolume(OPfmodChannel* channel, OPfloat volume);
void OPfmodSetVolume(OPfmodChannelGroup* channel, OPfloat volume);
void OPfmodUpdate();

inline OPint OPfmodIsPlaying(OPfmodChannel* channel) {
	bool isPlaying;
#ifdef OPIFEX_OPTION_FMOD
	channel->isPlaying(&isPlaying);
#endif
	return isPlaying;
}

inline void OPfmodSetPause(OPfmodChannel* channel, i8 pause) {
	#ifdef OPIFEX_OPTION_FMOD
	channel->setPaused(pause);
	#endif
}

#endif
