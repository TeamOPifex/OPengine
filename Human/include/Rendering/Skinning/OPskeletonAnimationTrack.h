#pragma once

struct OPskeletonAnimationTrack;
typedef OPskeletonAnimationTrack OPskeletonAnimationTrack;

#include "./Human/include/Rendering/Skinning/OPskeletonAnimation.h"

struct OPskeletonAnimationTrack {
	OPskeletonAnimation* current;
	OPskeletonAnimation* next;
};