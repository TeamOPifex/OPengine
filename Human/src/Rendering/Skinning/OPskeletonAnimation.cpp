#include "./Human/include/Rendering/Skinning/OPskeletonAnimation.h"

void OPskeletonAnimationInit(OPskeletonAnimation* skelAnim, OPint boneCount, OPmat4* frames, i32 frameCount) {
	skelAnim->FrameCount = frameCount;
	skelAnim->Frame = 0;
	skelAnim->Elapsed = 0;
	skelAnim->FramesPer = 1000 / 24;
	OPint totalSize = sizeof(OPmat4)* frameCount * boneCount;
	skelAnim->JointFrames = (OPmat4*)OPalloc(totalSize);
	OPmemcpy(skelAnim->JointFrames, frames, totalSize);
}

OPskeletonAnimation* OPskeletonAnimationCreate(OPint boneCount, OPmat4* frames, i32 count) {
	OPskeletonAnimation* result = (OPskeletonAnimation*)OPalloc(sizeof(OPskeletonAnimation));
	OPskeletonAnimationInit(result, boneCount, frames, count);
	return result;
}

void OPskeletonAnimationUpdate(OPskeletonAnimation* skelAnim, OPtimer* timer, OPskeleton* skeleton) {
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

	for (OPint i = 0; i < skeleton->hierarchyCount; i++) {

		ind1 = skeleton->hierarchyCount * skelAnim->Frame + i;
		ind2 = skeleton->hierarchyCount * (skelAnim->Frame + 1) + i;

		// Innefficient and poor interpolation scheme
		// TODO(garrett): Fix
		// TODO(garrett): interpolate into the first frame if looping
		if(skelAnim->Frame < (skelAnim->FrameCount - 2)) {

			for(OPint j = 0; j < 4; j++) {
				for(OPint k = 0; k < 4; k++) {

					skeleton->localPoses[i][j][k] = 
						skelAnim->JointFrames[ind1][j][k] + 
							(skelAnim->JointFrames[ind2][j][k] - skelAnim->JointFrames[ind1][j][k]) *
						percent;

				}
			}

		} else {
			skeleton->localPoses[i] = skelAnim->JointFrames[ind1];
		}

	}

}