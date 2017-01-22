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


	void Init(OPint boneCount, OPanimationFrame* frames, OPuint count, OPchar* name);
	void Update(OPtimer* timer, OPfloat timeScale);
	void UpdateEvents(void* data);
	void Apply(OPskeleton* skeleton);
	void Apply(OPskeleton* skeleton, i16 fromJoint);
	void Apply(OPmat4* animationFrame, OPskeleton* skeleton, i16 fromJoint);
	void Merge(OPskeletonAnimation* skelAnim2, OPfloat merge);
	void Combine(OPskeletonAnimation* skelAnim2, OPskeleton* skeleton, i16 fromJoint);
	void SetEvents(OPuint frames, OPskeletonAnimationEvent* events);

	inline void Update(OPtimer* timer) {
		Update(timer, 1.0f);
	}

	inline void Reset() {
		Frame = 0;
		LoopsCompleted = 0;
	}

	inline OPskeletonAnimation* Clone() {
		return OPskeletonAnimation::Create(BoneCount, JointFrames, FrameCount, Name);
	}

	static void Apply(OPmat4* animationFrame, OPskeleton* skeleton);
	static OPskeletonAnimation* Create(OPint boneCount, OPanimationFrame* frames, OPuint count, OPchar* name);
};


