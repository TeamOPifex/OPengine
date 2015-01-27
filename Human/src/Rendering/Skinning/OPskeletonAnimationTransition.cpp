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

void OPskeletonAnimationTransitionUpdate(OPskeletonAnimationTransition* transition, OPtimer* timer) {
	transition->Current += timer->Elapsed;
	transition->
}

void OPskeletonAnimationTransitionReset(OPskeletonAnimationTransition* transition) {

}
