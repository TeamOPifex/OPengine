#include "./Human/include/Rendering/Skinning/OPskeleton.h"

inline void OPskeletonUpdateGlobalPoses(OPskeleton* skeleton) {
	skeleton->globalPoses[0] = skeleton->localPoses[0];
	for (ui32 i = 1; i < skeleton->hierarchyCount; ++i) {
		skeleton->globalPoses[i] = skeleton->globalPoses[skeleton->hierarchy[i]] * skeleton->localPoses[i];
	}
}

OPskeleton* OPskeletonCreate(i16* hierarchy, OPmat4* pose, i32 count) {
	OPint lenOfMatrices = sizeof(OPmat4)* count;
	OPint sizeOfSkeleton = sizeof(OPskeleton);
	void* memoryBlock = OPalloc(sizeOfSkeleton + lenOfMatrices * 6);

	OPskeleton* skeleton = (OPskeleton*)memoryBlock;
	skeleton->hierarchy = hierarchy;
	skeleton->hierarchyCount = count;

	skeleton->invBindPoses = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton);
	skeleton->globalPoses = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + lenOfMatrices);
	skeleton->localPoses = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + lenOfMatrices * 2);
	skeleton->skinned = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + lenOfMatrices * 3);
	skeleton->bindPoses = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + lenOfMatrices * 4);
	skeleton->globalInvPoses = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + lenOfMatrices * 5);

	OPmemcpy(skeleton->bindPoses, pose, sizeof(OPmat4)* count);
	OPmemcpy(skeleton->localPoses, pose, sizeof(OPmat4)* count);

	skeleton->globalPoses[0] = skeleton->bindPoses[0];
	for (ui32 i = 1; i < skeleton->hierarchyCount; ++i) {
		skeleton->globalPoses[i] = skeleton->globalPoses[skeleton->hierarchy[i]] * skeleton->bindPoses[i];
	}

	for (OPint i = 0; i < skeleton->hierarchyCount; i++) {
		OPmat4Inverse(&skeleton->globalInvPoses[i], skeleton->globalPoses[i]);

		OPmat4 result = skeleton->globalInvPoses[i] * skeleton->globalPoses[i];
		OPmat4Log("Inv:", skeleton->globalInvPoses[i]);
		OPmat4Log("result:", result);
	}
	
	//OPskeletonUpdateGlobalPoses(skeleton);
	
	return skeleton;
}

void OPskeletonUpdate(OPskeleton* skeleton) {
	OPskeletonUpdateGlobalPoses(skeleton);

	for (i32 i = 0; i < skeleton->hierarchyCount; i++) {
		skeleton->skinned[i] = skeleton->globalPoses[i] * skeleton->globalInvPoses[i];
	}
}