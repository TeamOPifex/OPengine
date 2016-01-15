#include "./Human/include/Rendering/Skinning/OPskeletonAnimation.h"
#include "./Core/include/Assert.h"

void OPskeletonAnimationInit(OPskeletonAnimation* skelAnim, OPint boneCount, OPmat4* frames, i32 frameCount) {
	OPbzero(skelAnim, sizeof(OPskeletonAnimation));
	skelAnim->FrameCount = frameCount;
	skelAnim->Frame = 0;
	skelAnim->Elapsed = 0;
	skelAnim->FramesPer = 1000 / 24;
	skelAnim->Loop = 1;
	skelAnim->LoopsCompleted = 0;

	OPint totalSize = sizeof(OPmat4)* frameCount * boneCount;
	skelAnim->JointFrames = (OPmat4*)OPalloc(totalSize);
	OPmemcpy(skelAnim->JointFrames, frames, totalSize);
	for (OPuint i = 0; i < frameCount * boneCount; i++) {
		OPmat4SetTranslate(skelAnim->JointFrames[i], 0, 0, 0);
	}

	skelAnim->CurrentFrame = (OPmat4*)OPalloc(sizeof(OPmat4) * boneCount);
	skelAnim->BoneCount = boneCount;
}

OPskeletonAnimation* OPskeletonAnimationCreate(OPint boneCount, OPmat4* frames, i32 count) {
	OPskeletonAnimation* result = (OPskeletonAnimation*)OPalloc(sizeof(OPskeletonAnimation));
	OPskeletonAnimationInit(result, boneCount, frames, count);
	return result;
}


void OPskeletonAnimationUpdate(OPskeletonAnimation* skelAnim, OPtimer* timer, OPfloat timeScale) {
	ASSERT(skelAnim->FramesPer != 0, "Must have at least 1 frame per second");

	skelAnim->Elapsed += timer->Elapsed * timeScale;
	skelAnim->LastFrame = skelAnim->Frame;

	while (skelAnim->Elapsed >= skelAnim->FramesPer) {
		// OPlog("Elapsed %d, FP %d, Frame %d",
		// 	skelAnim->Elapsed,
		// 	skelAnim->FramesPer,
		// 	skelAnim->Frame);
		skelAnim->Elapsed -= skelAnim->FramesPer;
		skelAnim->Frame++;
		if (skelAnim->Frame >= skelAnim->FrameCount) {
			if(skelAnim->Loop) {
				skelAnim->Frame = 0;
				skelAnim->LoopsCompleted++;
			} else {
				skelAnim->Frame--;
				skelAnim->LoopsCompleted = 1;
			}
		}
	}

	OPint ind1, ind2;
	OPfloat percent = skelAnim->Elapsed / (OPfloat)skelAnim->FramesPer;

	for (OPint i = 0; i < skelAnim->BoneCount; i++) {

		ind1 = skelAnim->BoneCount * skelAnim->Frame + i;
		ind2 = skelAnim->BoneCount * (skelAnim->Frame + 1) + i;

		// Innefficient and poor interpolation scheme
		// TODO(garrett): Fix - interpolate into the first frame if looping
		if (skelAnim->Frame <= (skelAnim->FrameCount - 2)) {
			skelAnim->CurrentFrame[i] = OPmat4Interpolate(skelAnim->JointFrames[ind1], skelAnim->JointFrames[ind2], percent);
		}
		else {
			skelAnim->CurrentFrame[i] = skelAnim->JointFrames[ind1];
		}

	}
}

void OPskeletonAnimationUpdateEvents(OPskeletonAnimation* skelAnim, void* data) {
	if(skelAnim->LastFrame != skelAnim->Frame) {
		for(OPint i = 0; i < skelAnim->EventCount; i++) {
			// Start Frame is no longer the LastFrame
			// And Event Frame is between Start & End
			if( (skelAnim->Events[i].Frame <= skelAnim->Frame &&
					 skelAnim->Events[i].End >= skelAnim->Frame )) {
				skelAnim->Events[i].Event(skelAnim, skelAnim->Events[i].Frame, data);
			}
		}
	} else {
		for(OPint i = 0; i < skelAnim->EventCount; i++) {
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
		skeleton->localPoses[i] = skelAnim->CurrentFrame[i];
	}
}

void OPskeletonAnimationApply(OPmat4* animationFrame, OPskeleton* skeleton) {
	for (OPint i = 0; i < skeleton->hierarchyCount; i++) {
		skeleton->localPoses[i] = animationFrame[i];
	}
}

void OPskeletonAnimationApply(OPskeletonAnimation* skelAnim, OPskeleton* skeleton, i16 fromJoint) {
	ASSERT(skeleton->hierarchyCount == skelAnim->BoneCount, "This animation is not meant for this skeleton, Bone Count mismatch.");

	for (OPint i = fromJoint; i < skeleton->hierarchyCount; i++) {
		if (i != fromJoint && skeleton->hierarchy[i] <= skeleton->hierarchy[fromJoint]) {
			break;
		}
		skeleton->localPoses[i] = skelAnim->CurrentFrame[i];
	}
}

void OPskeletonAnimationApply(OPmat4* animationFrame, OPskeleton* skeleton, i16 fromJoint) {
	for (OPint i = fromJoint; i < skeleton->hierarchyCount; i++) {
		if (i != fromJoint && skeleton->hierarchy[i] <= skeleton->hierarchy[fromJoint]) {
			break;
		}
		skeleton->localPoses[i] = animationFrame[i];
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

	OPint ind1, ind2, ind3, ind4;
	OPfloat percent1 = skelAnim1->Elapsed / (OPfloat)skelAnim1->FramesPer;
	OPfloat percent2 = skelAnim2->Elapsed / (OPfloat)skelAnim2->FramesPer;

	for (OPint i = 0; i < skelAnim1->BoneCount; i++) {

		ind1 = skelAnim1->BoneCount * skelAnim1->Frame + i;

		if (skelAnim1->Frame == skelAnim1->FrameCount - 1) {
			if(skelAnim1->Loop) {
				ind2 = skelAnim1->BoneCount * (0) + i;
			} else {
				ind2 = skelAnim1->BoneCount * skelAnim1->Frame + i;
			}
		}
		else {
			ind2 = skelAnim1->BoneCount * (skelAnim1->Frame + 1) + i;
		}

		ind3 = skelAnim1->BoneCount * skelAnim2->Frame + i;
		if (skelAnim2->Frame == skelAnim2->FrameCount - 1) {
			ind4 = skelAnim1->BoneCount * (0) + i;
		}
		else {
			ind4 = skelAnim1->BoneCount * (skelAnim2->Frame + 1) + i;
		}

		// Innefficient and poor interpolation scheme
		// TODO(garrett): Fix - interpolate into the first frame if looping

		OPmat4 anim1 = OPmat4Interpolate(skelAnim1->JointFrames[ind1], skelAnim1->JointFrames[ind2], percent1);
		OPmat4 anim2 = OPmat4Interpolate(skelAnim2->JointFrames[ind3], skelAnim2->JointFrames[ind4], percent2);

		if (merge == 1.0) {
			skelAnim1->CurrentFrame[i] = anim2;
			skelAnim2->CurrentFrame[i] = anim2;
		}
		else if (merge == 0.0) {
			skelAnim1->CurrentFrame[i] = anim1;
			skelAnim2->CurrentFrame[i] = anim1;
		}
		else {
			skelAnim1->CurrentFrame[i] = OPmat4Interpolate(anim1, anim2, merge);
			skelAnim2->CurrentFrame[i] = skelAnim1->CurrentFrame[i];
		}

	}
}

void OPskeletonAnimationSetEvents(OPskeletonAnimation* skelAnim, OPuint frames, OPskeletonAnimationEvent* events) {
	skelAnim->Events = events;
	skelAnim->EventCount = frames;
}
