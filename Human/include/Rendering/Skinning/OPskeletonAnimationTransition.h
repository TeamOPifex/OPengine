#pragma once

struct OPskeletonAnimationTransition;
typedef struct OPskeletonAnimationTransition OPskeletonAnimationTransition;

#include "OPskeletonAnimation.h"

struct OPskeletonAnimationTransition {
	OPskeletonAnimation* Start;
	OPskeletonAnimation* End;
	OPfloat Duration;
	OPfloat Current;
};

OPskeletonAnimationTransition OPskeletonAnimationTransitionInit(OPskeletonAnimation* start, OPskeletonAnimation* end, OPfloat duration);
OPint OPskeletonAnimationTransitionUpdate(OPskeletonAnimationTransition* transition, OPtimer* timer);
void OPskeletonAnimationTransitionApply(OPskeletonAnimationTransition* transition, OPskeleton* skeleton);
void OPskeletonAnimationTransitionReset(OPskeletonAnimationTransition* transition);