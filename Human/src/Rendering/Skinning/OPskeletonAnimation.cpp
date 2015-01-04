#include "./Human/include/Rendering/Skinning/OPskeletonAnimation.h"

void OPskeletonAnimationInit(OPskeletonAnimation* skelAnim, OPskeleton* skeleton, OPmat4* frames, i32 count) {
	skelAnim->FrameCount = count;
	skelAnim->Skeleton = skeleton;
	skelAnim->Frame = 0;
	skelAnim->Elapsed = 0;
	skelAnim->FramesPer = 1000 / 24;
	OPint totalSize = sizeof(OPmat4)* count * skeleton->hierarchyCount;
	skelAnim->JointFrames = (OPmat4*)OPalloc(totalSize);
	OPmemcpy(skelAnim->JointFrames, frames, totalSize);
}

OPskeletonAnimation* OPskeletonAnimationCreate(OPskeleton* skeleton, OPmat4* frames, i32 count) {
	OPskeletonAnimation* result = (OPskeletonAnimation*)OPalloc(sizeof(OPskeletonAnimation));
	OPskeletonAnimationInit(result, skeleton, frames, count);
	return result;
}

void OPskeletonAnimationUpdate(OPskeletonAnimation* skelAnim, OPtimer* timer) {
	OPint ind;
	skelAnim->Elapsed += timer->Elapsed;
	if (skelAnim->Elapsed > skelAnim->FramesPer) {
		skelAnim->Elapsed -= skelAnim->FramesPer;
		skelAnim->Frame++;
		if (skelAnim->Frame >= skelAnim->FrameCount) {
			skelAnim->Frame = 0;
		}
	}

	for (OPint i = 0; i < skelAnim->Skeleton->hierarchyCount; i++) {
		ind = skelAnim->Skeleton->hierarchyCount * skelAnim->Frame + i;
		skelAnim->Skeleton->localPoses[i] = skelAnim->JointFrames[ind];
	}
}