#pragma once

struct OPskeletonAnimation;
struct OPskeletonAnimationEvent;
struct OPanimationFrame;

typedef OPskeletonAnimation OPskeletonAnimation;
typedef OPskeletonAnimationEvent OPskeletonAnimationEvent;
typedef OPanimationFrame OPanimationFrame;

#define OPSKELETONANIMATION_MAX_EVENTS 6

#include "OPskeleton.h"
#include "./Core/include/OPtimer.h"

struct OPskeletonAnimationEvent
{
	void(*Event)(OPskeletonAnimation*, OPuint, void*);
	OPuint Frame;
	OPuint End;
	OPint OnFrameChange;
};

struct OPanimationFrame {
	f32 Time;
	OPvec3 Position;
	OPquat Rotation;
	OPvec3 Scale;
};

struct OPskeletonAnimation {
	OPskeleton* Skeleton;
	OPchar* Name;

	OPanimationFrame* JointFrames;
	OPuint FrameCount;
	OPuint Frame;
	OPfloat Elapsed;
	OPfloat FramesPer;

	OPmat4* CurrentFrame;
	OPint BoneCount;

	OPint Loop;
	OPuint LoopsCompleted;
	OPuint LastFrame;
	OPskeletonAnimationEvent* Events;
	OPuint EventCount;
};


void OPskeletonAnimationInit(OPskeletonAnimation* skelAnim, OPint boneCount, OPanimationFrame* frames, i32 count, OPchar* name);
OPskeletonAnimation* OPskeletonAnimationCreate(OPint boneCount, OPanimationFrame* frames, OPuint count, OPchar* name);
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

inline OPskeletonAnimation* OPskeletonAnimationCopy(OPskeletonAnimation* source) {
	return OPskeletonAnimationCreate(source->BoneCount, source->JointFrames, source->FrameCount, source->Name);
}
