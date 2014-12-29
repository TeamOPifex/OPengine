#include "./Human/include/Rendering/Skinning/OPskeleton.h"

inline void OPskeletonUpdateGlobalPoses(OPskeleton* skeleton) {
	skeleton->globalPoses[0] = skeleton->localPoses[0];

	for (ui32 i = 1; i < skeleton->hierarchyCount; ++i) {
		// skeleton->globalPoses[i] = 
		// 	skeleton->localPoses[i] * 
		// 	skeleton->globalPoses[skeleton->hierarchy[i]];
		skeleton->globalPoses[i] = 
			skeleton->globalPoses[skeleton->hierarchy[i]] * 
			skeleton->localPoses[i];
		// OPmat4Mul(
		// 	&skeleton->globalPoses[i],
		// 	&skeleton->localPoses[i],
		// 	&skeleton->globalPoses[skeleton->hierarchy[i]]
		// 	);
		// OPmat4Log("Into", &skeleton->globalPoses[i]);
		// OPmat4Log("Parent", &skeleton->globalPoses[skeleton->hierarchy[i]]);
		// OPmat4Log("Local", &skeleton->localPoses[i]);
	}
}

OPskeleton* OPskeletonCreate(i16* hierarchy, OPmat4* pose, i32 count) {
	OPint lenOfMatrices = sizeof(OPmat4)* count;
	OPint sizeOfSkeleton = sizeof(OPskeleton);
	void* memoryBlock = OPalloc(sizeOfSkeleton + lenOfMatrices * 4);

	OPskeleton* skeleton = (OPskeleton*)memoryBlock;
	skeleton->hierarchy = hierarchy;
	skeleton->hierarchyCount = count;

	skeleton->localPoses = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton);
	skeleton->globalPoses = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + lenOfMatrices);
	skeleton->localInvPoses = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + lenOfMatrices * 2);
	skeleton->skinned = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + lenOfMatrices * 3);

	// for (i32 i = 0; i < skeleton->hierarchyCount; i++) {
	// 	OPmat4Identity(&skeleton->localPoses[i]);
	// }
	OPmemcpy(skeleton->localPoses, pose, sizeof(OPmat4)* count);

	// OPmemcpy(skeleton->globalPoses, pose, sizeof(OPmat4)* count);
	// OPmemcpy(skeleton->localInvPoses, pose, sizeof(OPmat4)* count);
	// OPmemcpy(skeleton->skinned, pose, sizeof(OPmat4)* count);

	OPskeletonUpdateGlobalPoses(skeleton);

	for (i32 i = 0; i < skeleton->hierarchyCount; i++) {
		//OPmat4Identity(&skeleton->localInvPoses[i]);
		//OPmat4Identity(&skeleton->skinned[i]);
		//
		//OPmat4Identity(&skeleton->localInvPoses[i]);
		//OPlog("H: %d", skeleton->hierarchy[i]);
		OPmat4Inverse(&skeleton->localInvPoses[i], skeleton->globalPoses[i]);
		OPmat4 inv = skeleton->localInvPoses[i] * skeleton->globalPoses[i];
		OPmat4Log("Ident: ", &inv);
	}
	
	return skeleton;
}

void OPskeletonUpdate(OPskeleton* skeleton) {
	OPskeletonUpdateGlobalPoses(skeleton);

	for (i32 i = 0; i < skeleton->hierarchyCount; i++) {
		//OPmat4Identity(&skeleton->localInvPoses[i]);
		//OPmat4Identity(&skeleton->skinned[i]);

		// skeleton->skinned[i] = 
		// 	skeleton->localInvPoses[i] * 
		// 	skeleton->globalPoses[i];

		skeleton->skinned[i] = 
			skeleton->globalPoses[i] * 
			skeleton->localInvPoses[i];
		// OPmat4Mul(
		// 	&skeleton->skinned[i], 
		// 	&skeleton->globalPoses[i], 
		// 	&skeleton->localInvPoses[i] );
	}
}