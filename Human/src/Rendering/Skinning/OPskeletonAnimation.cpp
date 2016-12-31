#include "./Human/include/Rendering/Skinning/OPskeletonAnimation.h"
#include "./Data/include/OPstring.h"
#include "./Core/include/Assert.h"

void OPskeletonAnimationInit(OPskeletonAnimation* skelAnim, OPint boneCount, OPanimationFrame* frames, OPuint frameCount, OPchar* name) {
	OPbzero(skelAnim, sizeof(OPskeletonAnimation));
	skelAnim->FrameCount = frameCount;
	skelAnim->Frame = 0;
	skelAnim->Elapsed = 0;
	skelAnim->FramesPer = 1000.0 / 60.0;//  24.0; // 24 fps... roughly 41.66
	skelAnim->Loop = 1;
	skelAnim->LoopsCompleted = 0;
	skelAnim->Name = OPstringCopy(name);

	OPint totalSize = sizeof(OPmat4)* frameCount * boneCount;
	skelAnim->JointFrames = frames;

	skelAnim->CurrentFrame = (OPmat4*)OPalloc(sizeof(OPmat4) * boneCount);
	skelAnim->BoneCount = boneCount;
}

OPskeletonAnimation* OPskeletonAnimationCreate(OPint boneCount, OPanimationFrame* frames, OPuint count, OPchar* name) {
	OPskeletonAnimation* result = (OPskeletonAnimation*)OPalloc(sizeof(OPskeletonAnimation));
	OPskeletonAnimationInit(result, boneCount, frames, count, name);
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

void OPskeletonAnimationUpdate(OPskeletonAnimation* skelAnim, OPtimer* timer, OPfloat timeScale) {
	ASSERT(skelAnim->FramesPer != 0, "Must have at least 1 frame per second");

	skelAnim->Elapsed += timer->Elapsed * timeScale;
	skelAnim->LastFrame = skelAnim->Frame;

	// Move into the next frame(s)
	while (skelAnim->Elapsed >= skelAnim->FramesPer) {
		skelAnim->Elapsed -= skelAnim->FramesPer;
		skelAnim->Frame++;
		if (skelAnim->Frame >= skelAnim->FrameCount) {
			if(skelAnim->Loop) {
				skelAnim->Frame = 0;
				skelAnim->LoopsCompleted++;
			} else {
				// If it doesn't loop, set it to the last frame
				// and set LoopsCompleted to 1 so we know not
				// to keep playing the animation
				skelAnim->Frame--;
				skelAnim->LoopsCompleted = 1;
			}
		}
	}

	OPint ind1, ind2;
	OPfloat percent = skelAnim->Elapsed / skelAnim->FramesPer;

	for (OPint i = 0; i < skelAnim->BoneCount; i++) {

		ind1 = skelAnim->FrameCount * i + skelAnim->Frame;
		ind2 = skelAnim->FrameCount * i + (skelAnim->Frame + 1);

		// TODO(garrett): Fix - interpolate into the first frame if looping
		if (skelAnim->Frame <= (skelAnim->FrameCount - 2)) {
			skelAnim->CurrentFrame[i] = InterpolateFrames(skelAnim->JointFrames[ind1], skelAnim->JointFrames[ind2], percent);
		}
		else {
			skelAnim->CurrentFrame[i] = CalculateFrame(skelAnim->JointFrames[ind1]);
		}

	}
}

void OPskeletonAnimationUpdateEvents(OPskeletonAnimation* skelAnim, void* data) {
	if(skelAnim->LastFrame != skelAnim->Frame) {
		for(OPuint i = 0; i < skelAnim->EventCount; i++) {
			// Start Frame is no longer the LastFrame
			// And Event Frame is between Start & End
			if( (skelAnim->Events[i].Frame <= skelAnim->Frame &&
					 skelAnim->Events[i].End >= skelAnim->Frame )) {
				skelAnim->Events[i].Event(skelAnim, skelAnim->Events[i].Frame, data);
			}
		}
	} else {
		for(OPuint i = 0; i < skelAnim->EventCount; i++) {
			if( !skelAnim->Events[i].OnFrameChange &&
				(skelAnim->Events[i].Frame <= skelAnim->Frame &&
				 skelAnim->Events[i].End >= skelAnim->Frame )
				) {
				skelAnim->Events[i].Event(skelAnim, skelAnim->Events[i].Frame, data);
			}
		}
	}
}

void OPskeletonAnimationApply(OPskeletonAnimation* skelAnim, OPskeleton* skeleton) {
	ASSERT(skeleton->hierarchyCount == skelAnim->BoneCount, "This animation is not meant for this skeleton, Bone Count mismatch.");

	for (OPint i = 0; i < skeleton->hierarchyCount; i++) {
		skeleton->activePose[i] = skelAnim->CurrentFrame[i];
	}
}

void OPskeletonAnimationApply(OPmat4* animationFrame, OPskeleton* skeleton) {
	for (OPint i = 0; i < skeleton->hierarchyCount; i++) {
		skeleton->activePose[i] = animationFrame[i];
	}
}

void OPskeletonAnimationApply(OPskeletonAnimation* skelAnim, OPskeleton* skeleton, i16 fromJoint) {
	ASSERT(skeleton->hierarchyCount == skelAnim->BoneCount, "This animation is not meant for this skeleton, Bone Count mismatch.");

	for (OPint i = fromJoint; i < skeleton->hierarchyCount; i++) {
		if (i != fromJoint && skeleton->hierarchy[i] <= skeleton->hierarchy[fromJoint]) {
			break;
		}
		skeleton->activePose[i] = skelAnim->CurrentFrame[i];
	}
}

void OPskeletonAnimationApply(OPmat4* animationFrame, OPskeleton* skeleton, i16 fromJoint) {
	for (OPint i = fromJoint; i < skeleton->hierarchyCount; i++) {
		if (i != fromJoint && skeleton->hierarchy[i] <= skeleton->hierarchy[fromJoint]) {
			break;
		}
		skeleton->activePose[i] = animationFrame[i];
	}
}

void OPskeletonAnimationCombine(OPskeletonAnimation* skelAnim, OPskeletonAnimation* skelAnim2, OPskeleton* skeleton, i16 fromJoint) {
	ASSERT(skelAnim2->BoneCount == skelAnim->BoneCount, "Bone Count must match");

	for (OPint i = fromJoint; i < skeleton->hierarchyCount; i++) {
		if (i != fromJoint && skeleton->hierarchy[i] <= skeleton->hierarchy[fromJoint]) {
			break;
		}
		skelAnim->CurrentFrame[i] = skelAnim2->CurrentFrame[i];
	}
}

void OPskeletonAnimationMerge(OPskeletonAnimation* skelAnim1, OPskeletonAnimation* skelAnim2, OPfloat merge) {
	ASSERT(skelAnim1->BoneCount == skelAnim2->BoneCount, "Bone Count must match");
	ASSERT(merge <= 1.0, "Can't merge passed the animation");
	ASSERT(merge >= 0.0, "Can't merge before the animation");

	OPint ind1, ind2, ind3, ind4;
	OPfloat percent1 = skelAnim1->Elapsed / (OPfloat)skelAnim1->FramesPer;
	OPfloat percent2 = skelAnim2->Elapsed / (OPfloat)skelAnim2->FramesPer;

	for (OPint i = 0; i < skelAnim1->BoneCount; i++) {


		//ind1 = skelAnim->FrameCount * i + skelAnim->Frame;
		//ind2 = skelAnim->FrameCount * i + (skelAnim->Frame + 1);


		ind1 = skelAnim1->FrameCount * i + skelAnim1->Frame;
		if (skelAnim1->Frame == skelAnim1->FrameCount - 1) {
			if(skelAnim1->Loop) {
				ind2 = skelAnim1->FrameCount * i;
			} else {
				ind2 = skelAnim1->FrameCount * i + skelAnim1->Frame;
			}
		}
		else {
			ind2 = skelAnim1->FrameCount * i + (skelAnim1->Frame + 1);
		}


		ind3 = skelAnim2->FrameCount * i + skelAnim2->Frame;
		if (skelAnim2->Frame == skelAnim2->FrameCount - 1) {
			ind4 = skelAnim2->FrameCount * i;
		}
		else {
			ind4 = skelAnim2->FrameCount * i + (skelAnim2->Frame + 1);
		}

		// Innefficient and poor interpolation scheme
		// TODO(garrett): Fix - interpolate into the first frame if looping



		//OPmat4 anim1 = InterpolateFrames(skelAnim1->JointFrames[ind1], skelAnim1->JointFrames[ind2], percent1);
		//OPmat4 anim2 = InterpolateFrames(skelAnim2->JointFrames[ind3], skelAnim2->JointFrames[ind4], percent2);

		OPanimationFrame inbetweenFrame1 = GetInterpolatedFrame(skelAnim1->JointFrames[ind1], skelAnim1->JointFrames[ind2], percent1);
		OPanimationFrame inbetweenFrame2 = GetInterpolatedFrame(skelAnim2->JointFrames[ind3], skelAnim2->JointFrames[ind4], percent2);

		//if (merge >= 1.0) {
		//	skelAnim1->CurrentFrame[i] = CalculateFrame(inbetweenFrame2);
		//	skelAnim2->CurrentFrame[i] = skelAnim1->CurrentFrame[i];
		//}
		//else if (merge <= 0.0) {
		//	skelAnim1->CurrentFrame[i] = CalculateFrame(inbetweenFrame1);
		//	skelAnim2->CurrentFrame[i] = skelAnim1->CurrentFrame[i];
		//}
		//else {
			skelAnim1->CurrentFrame[i] = InterpolateFrames(inbetweenFrame1, inbetweenFrame2, merge); //OPmat4Interpolate(anim1, anim2, merge);
			skelAnim2->CurrentFrame[i] = skelAnim1->CurrentFrame[i];
		//}

	}
}

void OPskeletonAnimationSetEvents(OPskeletonAnimation* skelAnim, OPuint frames, OPskeletonAnimationEvent* events) {
	skelAnim->Events = events;
	skelAnim->EventCount = frames;
}
