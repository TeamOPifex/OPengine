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
}

OPskeletonAnimation* OPskeletonAnimationCreate(OPint boneCount, OPmat4* frames, i32 count) {
	OPskeletonAnimation* result = (OPskeletonAnimation*)OPalloc(sizeof(OPskeletonAnimation));
	OPskeletonAnimationInit(result, boneCount, frames, count);
	return result;
}

void OPskeletonAnimationUpdate(OPskeletonAnimation* skelAnim, OPtimer* timer) {
	skelAnim->Elapsed += timer->Elapsed;
	skelAnim->LastFrame = skelAnim->Frame;

	while (skelAnim->Elapsed > skelAnim->FramesPer) {
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
}

void OPskeletonAnimationUpdateEvents(OPskeletonAnimation* skelAnim) {
	if(skelAnim->LastFrame != skelAnim->Frame) {
		for(OPint i = 0; i < skelAnim->EventCount; i++) {
			if(skelAnim->Events[i].Frame > skelAnim->LastFrame && skelAnim->Events[i].Frame <= skelAnim->Frame) {
				skelAnim->Events[i].Event(skelAnim, skelAnim->Events[i].Frame);
			}
		}
	}
}

void OPskeletonAnimationApply(OPskeletonAnimation* skelAnim, OPskeleton* skeleton) {
	OPint ind1, ind2;
	OPfloat percent = skelAnim->Elapsed / (OPfloat)skelAnim->FramesPer;

	for (OPint i = 0; i < skeleton->hierarchyCount; i++) {

		ind1 = skeleton->hierarchyCount * skelAnim->Frame + i;
		ind2 = skeleton->hierarchyCount * (skelAnim->Frame + 1) + i;

		// Innefficient and poor interpolation scheme
		// TODO(garrett): Fix - interpolate into the first frame if looping
		if(skelAnim->Frame < (skelAnim->FrameCount - 2)) {
			skeleton->localPoses[i] = OPmat4Interpolate(skelAnim->JointFrames[ind1], skelAnim->JointFrames[ind2], percent);
		} else {
			skeleton->localPoses[i] = skelAnim->JointFrames[ind1];
		}

	}

}

void OPskeletonAnimationMerge(OPskeletonAnimation* skelAnim1, OPskeletonAnimation* skelAnim2, OPfloat merge, OPskeleton* skeleton) {
	OPint ind1, ind2, ind3, ind4;
	OPfloat percent1 = skelAnim1->Elapsed / (OPfloat)skelAnim1->FramesPer;
	OPfloat percent2 = skelAnim2->Elapsed / (OPfloat)skelAnim2->FramesPer;

	for (OPint i = 0; i < skeleton->hierarchyCount; i++) {
		
		ind1 = skeleton->hierarchyCount * skelAnim1->Frame + i;
		if (skelAnim1->Frame == skelAnim1->FrameCount - 1) {
			ind2 = skeleton->hierarchyCount * (0) + i;
		}
		else {
			ind2 = skeleton->hierarchyCount * (skelAnim1->Frame + 1) + i;
		}

		ind3 = skeleton->hierarchyCount * skelAnim2->Frame + i;
		if (skelAnim2->Frame == skelAnim2->FrameCount - 1) {
			ind4 = skeleton->hierarchyCount * (0) + i;
		}
		else {
			ind4 = skeleton->hierarchyCount * (skelAnim2->Frame + 1) + i;
		}

		// Innefficient and poor interpolation scheme
		// TODO(garrett): Fix - interpolate into the first frame if looping

		OPmat4 anim1 = OPmat4Interpolate(skelAnim1->JointFrames[ind1], skelAnim1->JointFrames[ind2], percent1);
		OPmat4 anim2 = OPmat4Interpolate(skelAnim2->JointFrames[ind3], skelAnim2->JointFrames[ind4], percent2);

		if (merge == 1.0) {
			skeleton->localPoses[i] = anim2;
		}
		else if (merge == 0.0) {
			skeleton->localPoses[i] = anim1;
		}
		else {
			skeleton->localPoses[i] = OPmat4Interpolate(anim1, anim2, merge);
		}

	}
}

void OPskeletonAnimationSetEvents(OPskeletonAnimation* skelAnim, OPuint frames, OPskeletonAnimationEvent* events) {
	skelAnim->Events = events;
	skelAnim->EventCount = frames;
}