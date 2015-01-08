#ifndef OPIFEX_RENDERING_SKINNING_ANIMATION
#define OPIFEX_RENDERING_SKINNING_ANIMATION

#include "OPskeleton.h"
#include "./Core/include/OPtimer.h"

typedef struct {
	OPskeleton* Skeleton;
	OPmat4* JointFrames;
	OPint FrameCount;
	OPint Frame;
	ui64 Elapsed;
	ui64 FramesPer;
} OPskeletonAnimation;

void OPskeletonAnimationInit(OPskeletonAnimation* skelAnim, OPskeleton* skeleton, OPmat4* frames, i32 count);
OPskeletonAnimation* OPskeletonAnimationCreate(OPskeleton* skeleton, OPmat4* frames, i32 count);
void OPskeletonAnimationUpdate(OPskeletonAnimation* skeleton, OPtimer* timer);



#endif