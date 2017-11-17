#include "./Pipeline/include/Loaders/OPloaderOPskeleton.h"
#include "./Data/include/OPcman.h"
#include "./Core/include/Assert.h"

void OPskeletonAddLoader() {
	OPassetLoader loader = {
		".skel",
		"Models/",
		sizeof(OPskeleton),
		(OPint(*)(OPstream*, void**))OPloaderOPskeletonLoad,
		(OPint(*)(void*))OPloaderOPskeletonUnload,
		NULL
	};
	OPCMAN.AddLoader(&loader);
}

OPint OPloaderOPskeletonLoad(OPstream* str, OPskeleton** skeleton) {
	//OPstream* str = OPreadFile(filename);

	i32 boneCount = str->I16();

	ASSERT(boneCount < OPSKELETON_MAX_BONE_COUNT, "Skeleton cannot contain more than OPSKELETON_MAX_BONE_COUNT");



	i16 hierarchy[OPSKELETON_MAX_BONE_COUNT];
	OPchar* jointNames[OPSKELETON_MAX_BONE_COUNT];
	OPmat4 bindPose[OPSKELETON_MAX_BONE_COUNT];
	OPmat4 offsets[OPSKELETON_MAX_BONE_COUNT];


	OPmat4 globalInverseBindPose = OPMAT4_IDENTITY;
	for (ui32 j = 0; j < 4; j++) {
		for (ui32 k = 0; k < 4; k++) {
			globalInverseBindPose[k][j] = str->F32();
		}
	}
	//globalInverseBindPose.SetIdentity();

	for (i32 i = 0; i < boneCount; i++) {
		hierarchy[i] = str->I16();
		//OPlogErr("hier: %d", hierarchy[i]);
		jointNames[i] = str->String();
		//OPlog(jointNames[i]);

		//OPlogErr("Pose");
		for (ui32 j = 0; j < 4; j++) {
			for (ui32 k = 0; k < 4; k++) {
				bindPose[i][k][j] = str->F32();
				//OPlogErr("%f", pose[i][k][j]);
			}
		}
		//OPmat4Log("Pose", bindPose[i]);
		//pose[i].SetIdentity();
		// pose[i] = OPmat4Transpose(pose[i]);

		//OPlogErr("Offset");
		for (ui32 j = 0; j < 4; j++) {
			for (ui32 k = 0; k < 4; k++) {
				offsets[i][k][j] = str->F32();
				//OPlogErr("%f", offsets[i][k][j]);
			}
		}
		//offsets[i].SetIdentity();
		//OPmat4Log("Offset", offsets[i]);
	}

	*skeleton = OPskeleton::Create(hierarchy, bindPose, offsets, globalInverseBindPose, boneCount, jointNames);
	for (i32 i = 0; i < boneCount; i++) {
		OPfree(jointNames[i]);
	}

	//OPstreamDestroy(str);

	return 1;
}

OPint OPloaderOPskeletonUnload(OPskeleton* skeleton) {
	skeleton->Destroy();
	OPfree(skeleton);
	return 1;
}