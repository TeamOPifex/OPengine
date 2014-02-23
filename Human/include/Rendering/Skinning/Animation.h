#ifndef OPIFEX_RENDERING_SKINNING_ANIMATION
#define OPIFEX_RENDERING_SKINNING_ANIMATION

#define NUM_JOINTS 4

#include "./Math/include/Matrix4.h"
#include "./Math/include/Vector4.h"

typedef struct {
	ui16* hierarchy;
	OPmat4* localPoses;
	OPmat4* globalPoses;
} OPskeleton;

void OPskeletonUpdate(OPskeleton* skeleton) {
	skeleton->globalPoses[0] = skeleton->localPoses[0];

	for( ui32 i = 1; i < NUM_JOINTS; ++i) {
		const ui16 parentJoint = skeleton->hierarchy[i];
		skeleton->globalPoses[i] = skeleton->globalPoses[parentJoint] * skeleton->localPoses[i];
	}
}

void OPskeletonAnimate(OPskeleton* skeleton) {
	ui16 verticesCount = 0;
	
	for(ui16 i = 0; i < verticesCount; i++) {
		ui16 index0 = jointIndices[i];
		OPvec4 position = srcVertexData[i];
		OPvec4 skinnedPosition = matrixPallete[index0] * position;
		dstVertexData[i] = skinnedPosition;
	}

	for(ui16 i = 0; i < verticesCount; i++) {
		ui16 index0 = jointIndices[i * 2 + 0];
		ui16 index1 = jointIndices[i * 2 + 1];
		f32 weight0 = jointWeights[i * 2 + 0];
		f32 weight1 = jointWeights[i * 2 + 1];
		OPmat4 mat0 = matrixPallete[index0] * weight0;
		OPmat4 mat1 = matrixPallete[index1] * weight1;

		OPmat4 weightedMatrix = mat0 + mat1;

		OPvec4 position = srcVertexData[i];
		OPvec4 skinnedPosition = weightedMatrix * position;
		dstVertexData[i] = skinnedPosition;
	}
}

#endif