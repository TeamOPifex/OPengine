#ifndef OPIFEX_RENDERING_SKINNING_ANIMATION
#define OPIFEX_RENDERING_SKINNING_ANIMATION

#include "OPskeleton.h"
#include "./Core/include/OPtimer.h"

#define OPSKELETONANIMATION_MAX_EVENTS 6

struct _OPskeletonAnimation;
typedef _OPskeletonAnimation OPskeletonAnimation;

struct _OPskeletonAnimationEvent
{
	void(*Event)(OPskeletonAnimation*, OPuint, void*);
	OPuint Frame;
	OPuint End;
	OPint OnFrameChange;
};
typedef _OPskeletonAnimationEvent OPskeletonAnimationEvent;

struct _OPskeletonAnimation{
	OPskeleton* Skeleton;
	OPmat4* JointFrames;
	OPuint FrameCount;
	OPuint Frame;
	ui64 Elapsed;
	ui64 FramesPer;

	OPint Loop;
	OPuint LoopsCompleted;
	OPuint LastFrame;
	OPskeletonAnimationEvent* Events;
	OPuint EventCount;
};


void OPskeletonAnimationInit(OPskeletonAnimation* skelAnim, OPint boneCount, OPmat4* frames, i32 count);
OPskeletonAnimation* OPskeletonAnimationCreate(OPint boneCount, OPmat4* frames, i32 count);
void OPskeletonAnimationUpdate(OPskeletonAnimation* skelAnim, OPtimer* timer);
void OPskeletonAnimationUpdateEvents(OPskeletonAnimation* skelAnim, void* data);
void OPskeletonAnimationApply(OPskeletonAnimation* skelAnim, OPskeleton* skeleton);
void OPskeletonAnimationMerge(OPskeletonAnimation* skelAnim1, OPskeletonAnimation* skelAnim2, OPfloat merge, OPskeleton* skeleton);
void OPskeletonAnimationSetEvents(OPskeletonAnimation* skelAnim, OPuint frames, OPskeletonAnimationEvent* events);
inline void OPskeletonAnimationReset(OPskeletonAnimation* skelAnim) {
	skelAnim->Frame = 0;
	skelAnim->LoopsCompleted = 0;
}

inline OPskeletonAnimation* OPskeletonAnimationCopy(OPskeletonAnimation* source, OPint boneCount) {
	return OPskeletonAnimationCreate(boneCount, source->JointFrames, source->FrameCount);
}

#endif