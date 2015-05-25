#include "./Human/include/Rendering/Skinning/OPskeleton.h"
#include "./Data/include/OPstring.h"

inline void OPskeletonUpdateGlobalPoses(OPskeleton* skeleton) {
	skeleton->globalPoses[0] = skeleton->localPoses[0];
	for (ui32 i = 1; i < skeleton->hierarchyCount; ++i) {
		skeleton->globalPoses[i] = skeleton->globalPoses[skeleton->hierarchy[i]] * skeleton->localPoses[i];
	}
}

OPskeleton* OPskeletonCreate(i16* hierarchy, OPmat4* pose, i32 count, OPchar** names) {
	OPint sizeOfMatricesArray = sizeof(OPmat4)* count;
	OPint sizeOfHierarchy = sizeof(i16)* count;
	OPint sizeOfSkeleton = sizeof(OPskeleton);
	void* memoryBlock = OPalloc(sizeOfSkeleton + sizeOfHierarchy + sizeOfMatricesArray * 4);

	OPskeleton* skeleton = (OPskeleton*)memoryBlock;
	skeleton->hierarchyCount = count;
	skeleton->hierarchy = (i16*)((OPint)memoryBlock + sizeOfSkeleton);
	OPmemcpy(skeleton->hierarchy, hierarchy, sizeOfHierarchy);

	skeleton->globalPoses = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + sizeOfHierarchy + sizeOfMatricesArray * 0);
	skeleton->localPoses = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + sizeOfHierarchy + sizeOfMatricesArray * 1);
	skeleton->skinned = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + sizeOfHierarchy + sizeOfMatricesArray * 2);
	skeleton->globalInvPoses = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + sizeOfHierarchy + sizeOfMatricesArray * 3);

	OPmemcpy(skeleton->localPoses, pose, sizeOfMatricesArray);

	OPskeletonUpdateGlobalPoses(skeleton);
	for (OPint i = 0; i < skeleton->hierarchyCount; i++) {
		OPmat4Inverse(&skeleton->globalInvPoses[i], skeleton->globalPoses[i]);
	}


	skeleton->jointNames = (OPchar**)OPalloc(sizeof(OPchar*) * count);
	for (OPint i = 0; i < count; i++) {
		skeleton->jointNames[i] = OPstringCopy(names[i]);
	}
	return skeleton;
}

i16 OPskeletonGet(OPskeleton* skeleton, const OPchar* name) {
	OPlog("Skeleton Get %d bones", skeleton->hierarchyCount);
	for (ui16 i = 0; i < skeleton->hierarchyCount; i++) {
		if (OPstringEquals(name, skeleton->jointNames[i])) return i;
	}
	return -1;
}

void OPskeletonUpdate(OPskeleton* skeleton) {
	OPskeletonUpdateGlobalPoses(skeleton);

	for (i32 i = 0; i < skeleton->hierarchyCount; i++) {
		skeleton->skinned[i] = skeleton->globalPoses[i] * skeleton->globalInvPoses[i];
	}
}

void OPskeletonDestroy(OPskeleton* skeleton) {
	OPfree(skeleton);
}