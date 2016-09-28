#include "./Human/include/Rendering/Skinning/OPskeleton.h"
#include "./Data/include/OPstring.h"

inline void OPskeletonUpdateGlobalPoses(OPskeleton* skeleton) {
	skeleton->globalPoses[0] = skeleton->localPoses[0];
	for (ui32 i = 1; i < skeleton->hierarchyCount; ++i) {
		//OPlogInfo("Parent Joint: %s <-- Curr Joint: %s", skeleton->jointNames[skeleton->hierarchy[i]], skeleton->jointNames[i]);

		skeleton->globalPoses[i] = skeleton->globalPoses[skeleton->hierarchy[i]] * skeleton->localPoses[i];
		//skeleton->globalPoses[i] = skeleton->bindPoses[i] * skeleton->globalPoses[skeleton->hierarchy[i]]; // globalInvPoses is just Ident for now
		//skeleton->globalPoses[i] = skeleton->localPoses[i] * skeleton->globalPoses[skeleton->hierarchy[i]]; // globalInvPoses is just Ident for now
	}
}

OPskeleton* OPskeletonCreate(i16* hierarchy, OPmat4* bindPose, OPmat4* boneOffsets, OPmat4 invGlobalPose, i32 count, OPchar** names) {
	OPint sizeOfMatricesArray = sizeof(OPmat4)* count;
	OPint sizeOfHierarchy = sizeof(i16)* count;
	OPint sizeOfSkeleton = sizeof(OPskeleton);
	void* memoryBlock = OPalloc(sizeOfSkeleton + sizeOfHierarchy + sizeOfMatricesArray * 5);

	OPskeleton* skeleton = (OPskeleton*)memoryBlock;
	skeleton->hierarchyCount = count;
	skeleton->hierarchy = (i16*)((OPint)memoryBlock + sizeOfSkeleton);
	OPmemcpy(skeleton->hierarchy, hierarchy, sizeOfHierarchy);

	skeleton->globalPoses = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + sizeOfHierarchy + sizeOfMatricesArray * 0);

	skeleton->localPoses = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + sizeOfHierarchy + sizeOfMatricesArray * 1);
	skeleton->skinned = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + sizeOfHierarchy + sizeOfMatricesArray * 2);
	skeleton->globalInvPoses = (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + sizeOfHierarchy + sizeOfMatricesArray * 3);

	skeleton->bindPoses = OPALLOC(OPmat4, count);// (OPmat4*)((OPint)memoryBlock + sizeOfSkeleton + sizeOfHierarchy + sizeOfMatricesArray * 4);



	OPmemcpy(skeleton->bindPoses, boneOffsets, sizeOfMatricesArray);
	OPmemcpy(skeleton->localPoses, bindPose, sizeOfMatricesArray); // boneOffsets
	//OPmemcpy(skeleton->globalPoses, bindPose, sizeOfMatricesArray);

	skeleton->globalInverse = invGlobalPose;// boneOffsets[0];
	//skeleton->globalInverse = boneOffsets[0];
	//OPmat4Inverse(&skeleton->globalInverse, skeleton->globalInverse);

	//skeleton->globalInverse.SetIdentity();

	skeleton->jointNames = (OPchar**)OPalloc(sizeof(OPchar*) * count);
	for (OPint i = 0; i < count; i++) {
		skeleton->jointNames[i] = OPstringCopy(names[i]);
		OPlogInfo("Joint Name (%d): %s", i, skeleton->jointNames[i]);
	}

	OPskeletonUpdateGlobalPoses(skeleton);	
	//for (OPint i = 0; i < skeleton->hierarchyCount; i++) {
	//	OPmat4Inverse(&skeleton->globalInvPoses[i], skeleton->globalPoses[i]);
	//}

	//for (OPint i = 0; i < skeleton->hierarchyCount; i++) {
		//skeleton->localPoses[i] *= skeleton->globalInverse;// .Translate(0, 0, 0);
		//skeleton->localPoses[i].SetIdentity();
		//skeleton->bindPoses[i].SetIdentity();
		//skeleton->skinned[i].SetIdentity();
		//skeleton->globalInvPoses[i].SetIdentity();
		//skeleton->globalPoses[i].SetIdentity();
		//OPmat4Inverse(&skeleton->globalInvPoses[i], skeleton->globalPoses[i]);
	//}




	return skeleton;
}

i16 OPskeletonGet(OPskeleton* skeleton, const OPchar* name) {
	for (ui16 i = 0; i < skeleton->hierarchyCount; i++) {
		if (OPstringEquals(name, skeleton->jointNames[i])) return i;
	}
	return -1;
}

void OPskeletonUpdate(OPskeleton* skeleton) {
	OPskeletonUpdateGlobalPoses(skeleton);

	for (i32 i = 0; i < skeleton->hierarchyCount; i++) {
		skeleton->skinned[i] = skeleton->globalPoses[i] * skeleton->bindPoses[i];
		//skeleton->skinned[i] = skeleton->localPoses[i] * skeleton->globalPoses[i];
		//skeleton->skinned[i] = skeleton->localPoses[i] * skeleton->globalPoses[i];
		//skeleton->skinned[i] = skeleton->globalPoses[i] * skeleton->globalInvPoses[i] * skeleton->localPoses[i];
		//skeleton->skinned[i] = skeleton->localPoses[i] * skeleton->globalPoses[i] * skeleton->globalInverse;
	}
}

void OPskeletonDestroy(OPskeleton* skeleton) {
	OPfree(skeleton);
}
