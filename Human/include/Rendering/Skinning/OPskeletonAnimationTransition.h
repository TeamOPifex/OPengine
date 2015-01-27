#ifndef OPIFEX_RENDERING_SKINNING_ANIMATION_TRANSITION
#define OPIFEX_RENDERING_SKINNING_ANIMATION_TRANSITION

#include "OPskeletonAnimation.h"

typedef struct {
	OPskeletonAnimation* Start;
	OPskeletonAnimation* End;
	OPfloat Duration;
	OPfloat Current;
} OPskeletonAnimationTransition;

OPskeletonAnimationTransition OPskeletonAnimationTransitionInit(OPskeletonAnimation* start, OPskeletonAnimation* end, OPfloat duration);
OPint OPskeletonAnimationTransitionUpdate(OPskeletonAnimationTransition* transition, OPtimer* timer);
void OPskeletonAnimationTransitionApply(OPskeletonAnimationTransition* transition, OPskeleton* skeleton);
void OPskeletonAnimationTransitionReset(OPskeletonAnimationTransition* transition);


#endif