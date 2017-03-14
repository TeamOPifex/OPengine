#include "./Human/include/Rendering/Skinning/OPskeletonAnimation.h"
#include "./Data/include/OPstring.h"
#include "./Core/include/Assert.h"

void OPskeletonAnimation::Init(OPint boneCount, OPanimationFrame* frames, OPuint frameCount, OPchar* name) {
	OPbzero(this, sizeof(OPskeletonAnimation));
	FrameCount = frameCount;
	Frame = 0;
	Elapsed = 0;
	FramesPer = 1000.0 / 60.0;//  24.0; // 24 fps... roughly 41.66
	Loop = 1;
	LoopsCompleted = 0;
	Name = OPstringCopy(name);

	OPint totalSize = sizeof(OPmat4)* frameCount * boneCount;
	JointFrames = frames;

	CurrentFrame = (OPmat4*)OPalloc(sizeof(OPmat4) * boneCount);
	BoneCount = boneCount;
}

OPskeletonAnimation* OPskeletonAnimation::Create(OPint boneCount, OPanimationFrame* frames, OPuint count, OPchar* name) {
	OPskeletonAnimation* result = (OPskeletonAnimation*)OPalloc(sizeof(OPskeletonAnimation));
	result->Init(boneCount, frames, count, name);
	return result;
}

inline OPanimationFrame GetInterpolatedFrame(OPanimationFrame frame1, OPanimationFrame frame2, OPfloat percentage) {
	OPanimationFrame inbetweenFrame;
	inbetweenFrame.Position = OPvec3Tween(frame1.Position, frame2.Position, percentage);
	inbetweenFrame.Rotation = frame1.Rotation.Slerp(frame2.Rotation, percentage);
	inbetweenFrame.Scale = OPvec3Tween(frame1.Scale, frame2.Scale, percentage);
	return inbetweenFrame;
}

OPmat4 CalculateFrame(OPanimationFrame frame1) {
	OPmat4 result = OPMAT4_IDENTITY;
	result.Translate(frame1.Position);
	result.Rot(frame1.Rotation);
	result.Scl(frame1.Scale);
	return result;
}

OPmat4 InterpolateFrames(OPanimationFrame frame1, OPanimationFrame frame2, OPfloat percentage) {
	return CalculateFrame(GetInterpolatedFrame(frame1, frame2, percentage));
}

void OPskeletonAnimation::Update(OPtimer* timer, OPfloat timeScale) {
	ASSERT(FramesPer != 0, "Must have at least 1 frame per second");

	Elapsed += timer->Elapsed * timeScale;
	LastFrame = Frame;

	// Move into the next frame(s)
	while (Elapsed >= FramesPer) {
		Elapsed -= FramesPer;
		Frame++;
		if (Frame >= FrameCount) {
			if(Loop) {
				Frame = 0;
				LoopsCompleted++;
			} else {
				// If it doesn't loop, set it to the last frame
				// and set LoopsCompleted to 1 so we know not
				// to keep playing the animation
				Frame--;
				LoopsCompleted = 1;
			}
		}
	}

	OPint ind1, ind2;
	OPfloat percent = Elapsed / FramesPer;

	for (OPint i = 0; i < BoneCount; i++) {

		ind1 = FrameCount * i + Frame;
		ind2 = FrameCount * i + (Frame + 1);

		// TODO(garrett): Fix - interpolate into the first frame if looping
		if (Frame <= (FrameCount - 2)) {
			CurrentFrame[i] = InterpolateFrames(JointFrames[ind1], JointFrames[ind2], percent);
		}
		else {
			CurrentFrame[i] = CalculateFrame(JointFrames[ind1]);
		}

	}
}

void OPskeletonAnimation::UpdateEvents(void* data) {
	if(LastFrame != Frame) {
		for(OPuint i = 0; i < EventCount; i++) {
			// Start Frame is no longer the LastFrame
			// And Event Frame is between Start & End
			if( (Events[i].Frame <= Frame &&
					 Events[i].End >= Frame )) {
				Events[i].Event(this, Events[i].Frame, data);
			}
		}
	} else {
		for(OPuint i = 0; i < EventCount; i++) {
			if( !Events[i].OnFrameChange &&
				(Events[i].Frame <= Frame &&
				 Events[i].End >= Frame )
				) {
				Events[i].Event(this, Events[i].Frame, data);
			}
		}
	}
}

void OPskeletonAnimation::Apply(OPskeleton* skeleton) {
	ASSERT(skeleton->hierarchyCount == BoneCount, "This animation is not meant for this skeleton, Bone Count mismatch.");

	for (OPint i = 0; i < skeleton->hierarchyCount; i++) {
		skeleton->activePose[i] = CurrentFrame[i];
	}
}

void OPskeletonAnimation::Apply(OPmat4* animationFrame, OPskeleton* skeleton) {
	for (OPint i = 0; i < skeleton->hierarchyCount; i++) {
		skeleton->activePose[i] = animationFrame[i];
	}
}

void OPskeletonAnimation::Apply(OPskeleton* skeleton, i16 fromJoint) {
	ASSERT(skeleton->hierarchyCount == BoneCount, "This animation is not meant for this skeleton, Bone Count mismatch.");

	for (OPint i = fromJoint; i < skeleton->hierarchyCount; i++) {
		if (i != fromJoint && skeleton->hierarchy[i] <= skeleton->hierarchy[fromJoint]) {
			break;
		}
		skeleton->activePose[i] = CurrentFrame[i];
	}
}

void OPskeletonAnimation::Apply(OPmat4* animationFrame, OPskeleton* skeleton, i16 fromJoint) {
	for (OPint i = fromJoint; i < skeleton->hierarchyCount; i++) {
		if (i != fromJoint && skeleton->hierarchy[i] <= skeleton->hierarchy[fromJoint]) {
			break;
		}
		skeleton->activePose[i] = animationFrame[i];
	}
}

void OPskeletonAnimation::Combine(OPskeletonAnimation* skelAnim2, OPskeleton* skeleton, i16 fromJoint) {
	ASSERT(skelAnim2->BoneCount == BoneCount, "Bone Count must match");

	for (OPint i = fromJoint; i < skeleton->hierarchyCount; i++) {
		if (i != fromJoint && skeleton->hierarchy[i] <= skeleton->hierarchy[fromJoint]) {
			break;
		}
		CurrentFrame[i] = skelAnim2->CurrentFrame[i];
	}
}

void OPskeletonAnimation::Merge(OPskeletonAnimation* skelAnim2, OPfloat merge, OPanimationMask* mask) {
	ASSERT(BoneCount == skelAnim2->BoneCount, "Bone Count must match");
	ASSERT(merge <= 1.0, "Can't merge passed the animation");
	ASSERT(merge >= 0.0, "Can't merge before the animation");

	OPint ind1, ind2, ind3, ind4;
	OPfloat percent1 = Elapsed / (OPfloat)FramesPer;
	OPfloat percent2 = skelAnim2->Elapsed / (OPfloat)skelAnim2->FramesPer;

	for (OPint i = 0; i < BoneCount; i++) {
		if (mask != NULL && !(*mask)[i]) continue;
		
		ind1 = FrameCount * i + Frame;
		if (Frame == FrameCount - 1) {
			ind2 = FrameCount * i;
			if (!Loop) {
				ind2 += Frame;
			}
		}
		else {
			ind2 = FrameCount * i + (Frame + 1);
		}


		ind3 = skelAnim2->FrameCount * i + skelAnim2->Frame;
		if (skelAnim2->Frame == skelAnim2->FrameCount - 1) {
			ind4 = skelAnim2->FrameCount * i;
		}
		else {
			ind4 = skelAnim2->FrameCount * i + (skelAnim2->Frame + 1);
		}


		// TODO(garrett): Fix - interpolate into the first frame if looping
				
		OPanimationFrame inbetweenFrame1 = GetInterpolatedFrame(JointFrames[ind1], JointFrames[ind2], percent1);
		OPanimationFrame inbetweenFrame2 = GetInterpolatedFrame(skelAnim2->JointFrames[ind3], skelAnim2->JointFrames[ind4], percent2);

		CurrentFrame[i] = InterpolateFrames(inbetweenFrame1, inbetweenFrame2, merge);
		skelAnim2->CurrentFrame[i] = CurrentFrame[i];

	}
}

void OPskeletonAnimation::SetEvents(OPuint frames, OPskeletonAnimationEvent* events) {
	Events = events;
	EventCount = frames;
}


void OPanimationMask::Init(OPskeleton* skeleton, bool val) {
	mask = (bool*)OPalloc(sizeof(bool) * skeleton->hierarchyCount);
	for (ui32 i = 0; i < skeleton->hierarchyCount; i++) {
		mask[i] = val;
	}
}

void OPanimationMask::Destroy() {
	ASSERT(mask != NULL, "Animation Mask was never initialized");
	OPfree(mask);
}

void OPanimationMask::Set(i16 ind, bool val) {
	mask[ind] = val;
}


void OPanimationMask::SetTree(OPskeleton* skeleton, i16 joint, bool val) {
	i16 startingJoint = joint;
	i16 parent = skeleton->hierarchy[joint];
	mask[startingJoint] = val;

	for (ui32 i = startingJoint + 1; i < skeleton->hierarchyCount && skeleton->hierarchy[i] >= startingJoint; i++) {
		mask[i] = val;
	}
}