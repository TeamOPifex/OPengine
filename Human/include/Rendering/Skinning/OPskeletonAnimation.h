#ifndef OPIFEX_RENDERING_SKINNING_ANIMATION
#define OPIFEX_RENDERING_SKINNING_ANIMATION

#include "OPskeleton.h"
#include "./Core/include/OPtimer.h"

#define OPSKELETONANIMATION_MAX_EVENTS 6

struct OPskeletonAnimation;
typedef OPskeletonAnimation OPskeletonAnimation;

struct OPskeletonAnimationEvent
{
	void(*Event)(OPskeletonAnimation*, OPuint, void*);
	OPuint Frame;
	OPuint End;
	OPint OnFrameChange;
};
typedef OPskeletonAnimationEvent OPskeletonAnimationEvent;

struct OPskeletonAnimation {
	OPskeleton* Skeleton;

	OPmat4* JointFrames;
	OPuint FrameCount;
	OPuint Frame;
	ui64 Elapsed;
	ui64 FramesPer;

	OPmat4* CurrentFrame;
	OPint BoneCount;

	OPint Loop;
	OPuint LoopsCompleted;
	OPuint LastFrame;
	OPskeletonAnimationEvent* Events;
	OPuint EventCount;
};


void OPskeletonAnimationInit(OPskeletonAnimation* skelAnim, OPint boneCount, OPmat4* frames, i32 count);
OPskeletonAnimation* OPskeletonAnimationCreate(OPint boneCount, OPmat4* frames, OPuint count);
void OPskeletonAnimationUpdate(OPskeletonAnimation* skelAnim, OPtimer* timer, OPfloat timeScale);
inline void OPskeletonAnimationUpdate(OPskeletonAnimation* skelAnim, OPtimer* timer) {
	OPskeletonAnimationUpdate(skelAnim, timer, 1.0f);
}
void OPskeletonAnimationUpdateEvents(OPskeletonAnimation* skelAnim, void* data);
void OPskeletonAnimationApply(OPskeletonAnimation* skelAnim, OPskeleton* skeleton);
void OPskeletonAnimationApply(OPskeletonAnimation* skelAnim, OPskeleton* skeleton, i16 fromJoint);
void OPskeletonAnimationApply(OPmat4* animationFrame, OPskeleton* skeleton);
void OPskeletonAnimationApply(OPmat4* animationFrame, OPskeleton* skeleton, i16 fromJoint);
void OPskeletonAnimationMerge(OPskeletonAnimation* skelAnim1, OPskeletonAnimation* skelAnim2, OPfloat merge);
void OPskeletonAnimationCombine(OPskeletonAnimation* skelAnim, OPskeletonAnimation* skelAnim2, OPskeleton* skeleton, i16 fromJoint);
void OPskeletonAnimationSetEvents(OPskeletonAnimation* skelAnim, OPuint frames, OPskeletonAnimationEvent* events);
inline void OPskeletonAnimationReset(OPskeletonAnimation* skelAnim) {
	skelAnim->Frame = 0;
	skelAnim->LoopsCompleted = 0;
}

inline OPskeletonAnimation* OPskeletonAnimationCopy(OPskeletonAnimation* source, OPint boneCount) {
	return OPskeletonAnimationCreate(boneCount, source->JointFrames, source->FrameCount);
}

#endif
