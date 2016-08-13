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
	OPcmanAddLoader(&loader);
}

OPint OPloaderOPskeletonLoad(OPstream* str, OPskeleton** skeleton) {
	//OPstream* str = OPreadFile(filename);

	i32 boneCount = str->I16();

	ASSERT(boneCount < OPSKELETON_MAX_BONE_COUNT, "Skeleton cannot contain more than OPSKELETON_MAX_BONE_COUNT");

	i16 hierarchy[OPSKELETON_MAX_BONE_COUNT];
	OPchar* jointNames[OPSKELETON_MAX_BONE_COUNT];
	OPmat4 pose[OPSKELETON_MAX_BONE_COUNT];

	for (i32 i = 0; i < boneCount; i++) {
		hierarchy[i] = str->I16();
		jointNames[i] = str->String();
		OPlog(jointNames[i]);

		for (i32 c = 0; c < 4; c++) {
			pose[i][c][0] = str->F32();
			pose[i][c][1] = str->F32();
			pose[i][c][2] = str->F32();
			pose[i][c][3] = str->F32();
		}

		pose[i] = OPmat4Transpose(pose[i]);
	}

	*skeleton = OPskeletonCreate(hierarchy, pose, boneCount, jointNames);

	//OPstreamDestroy(str);

	return 1;
}

OPint OPloaderOPskeletonUnload(OPskeleton* skeleton) {
	OPskeletonDestroy(skeleton);
	return 1;
}