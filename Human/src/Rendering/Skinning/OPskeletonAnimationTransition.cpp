#include "./Human/include/Rendering/Skinning/OPskeletonAnimationTransition.h"

OPskeletonAnimationTransition OPskeletonAnimationTransitionInit(OPskeletonAnimation* start, OPskeletonAnimation* end, OPfloat duration) {
	OPskeletonAnimationTransition result = {
		start,
		end,
		duration,
		0.0f
	};

	return result;
}

OPint OPskeletonAnimationTransitionUpdate(OPskeletonAnimationTransition* transition, OPtimer* timer) {
	transition->Current += timer->Elapsed;
	if(transition->Current >= transition->Duration) {
		transition->Current = transition->Duration;
		return 1;
	}
	return 0;
}


void OPskeletonAnimationTransitionApply(OPskeletonAnimationTransition* transition, OPskeleton* skeleton) {
	OPskeletonAnimationMerge(transition->Start, transition->End, transition->Current / transition->Duration);
	OPskeletonAnimationApply(transition->Start, skeleton);
}

void OPskeletonAnimationTransitionReset(OPskeletonAnimationTransition* transition) {
	transition->Current = 0;
}
