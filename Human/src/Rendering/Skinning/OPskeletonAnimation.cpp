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
	OPint ind1, ind2;

	skelAnim->Elapsed += timer->Elapsed;

	while (skelAnim->Elapsed > skelAnim->FramesPer) {
		skelAnim->Elapsed -= skelAnim->FramesPer;
		skelAnim->Frame++;
		if (skelAnim->Frame >= skelAnim->FrameCount) {
			skelAnim->Frame = 0;
		}
	}

	OPfloat percent = skelAnim->Elapsed / (OPfloat)skelAnim->FramesPer;

	for (OPint i = 0; i < skelAnim->Skeleton->hierarchyCount; i++) {

		ind1 = skelAnim->Skeleton->hierarchyCount * skelAnim->Frame + i;
		ind2 = skelAnim->Skeleton->hierarchyCount * (skelAnim->Frame + 1) + i;

		// Innefficient and poor interpolation scheme
		// TODO(garrett): Fix
		// TODO(garrett): interpolate into the first frame if looping
		if(skelAnim->Frame < (skelAnim->FrameCount - 2)) {

			for(OPint j = 0; j < 4; j++) {
				for(OPint k = 0; k < 4; k++) {

					skelAnim->Skeleton->localPoses[i][j][k] = 
						skelAnim->JointFrames[ind1][j][k] + 
							(skelAnim->JointFrames[ind2][j][k] - skelAnim->JointFrames[ind1][j][k]) *
						percent;

				}
			}

		} else {
			skelAnim->Skeleton->localPoses[i] = skelAnim->JointFrames[ind1];
		}

	}

}