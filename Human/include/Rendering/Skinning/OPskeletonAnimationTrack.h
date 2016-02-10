#ifndef OPIFEX_RENDERING_SKINNING_ANIMATION_TRACK
#define OPIFEX_RENDERING_SKINNING_ANIMATION_TRACK

#include "./Human/include/Rendering/Skinning/OPskeletonAnimation.h"

struct OPskeletonAnimationTrack {
	OPskeletonAnimation* current;
	OPskeletonAnimation* next;

};
typedef OPskeletonAnimationTrack OPskeletonAnimationTrack;

#endif