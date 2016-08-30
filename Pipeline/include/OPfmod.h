// TODO: (garrett) Move this into an addon
#pragma once

#include "./Core/include/OPtypes.h"

#ifdef OPIFEX_OPTION_FMOD
#include "./fmod.hpp"

typedef FMOD::ChannelGroup OPfmodChannelGroup;
typedef FMOD::System OPfmod;
typedef FMOD::Sound OPfmodSound;
typedef FMOD::Channel OPfmodChannel;

extern OPfmod* CURRENT_FMOD;

void OPfmodInit(); 
OPfmodSound* OPfmodLoadStream(OPchar* name);
OPfmodSound* OPfmodLoad(OPchar* name);
OPfmodChannel* OPfmodPlay(OPfmodSound* sound);
OPfmodChannelGroup* OPfmodCreateChannelGroup(const OPchar* name);
OPfmodChannel* OPfmodPlay(OPfmodSound* sound, OPfmodChannelGroup* group);
void OPfmodSetVolume(OPfmodChannel* channel, OPfloat volume);
void OPfmodSetVolume(OPfmodChannelGroup* channel, OPfloat volume);
void OPfmodUpdate();

inline bool OPfmodIsPlaying(OPfmodChannel* channel) {
	bool isPlaying;
	channel->isPlaying(&isPlaying);
	return isPlaying;
}

inline void OPfmodSetPause(OPfmodChannel* channel, bool pause) {
	channel->setPaused(pause);
}

#endif