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

EXPORT_METHOD void OPskeletonAnimationInit(OPskeletonAnimation* skelAnim, OPskeleton* skeleton, OPmat4* frames, i32 count);
EXPORT_METHOD OPskeletonAnimation* OPskeletonAnimationCreate(OPskeleton* skeleton, OPmat4* frames, i32 count);
EXPORT_METHOD void OPskeletonAnimationUpdate(OPskeletonAnimation* skeleton, OPtimer* timer);



#endif