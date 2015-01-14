#include "./Pipeline/include/Loaders/OPloaderOPskeleton.h"
#include "./Data/include/OPcman.h"
#include "./Core/include/Assert.h"

void OPskeletonAddLoader() {
	OPassetLoader loader = {
		".skel",
		"Models/",
		sizeof(OPskeleton),
		(OPint(*)(const OPchar*, void**))OPloaderOPskeletonLoad,
		(OPint(*)(void*))OPloaderOPskeletonUnload,
		NULL
	};
	OPcmanAddLoader(&loader);
}

OPint OPloaderOPskeletonLoad(const OPchar* filename, OPskeleton** skeleton) {
	OPstream* str = OPreadFile(filename);

	i32 boneCount = OPreadi16(str);

	ASSERT(boneCount < OPSKELETON_MAX_BONE_COUNT, "Skeleton cannot contain more than OPSKELETON_MAX_BONE_COUNT");

	i16 hierarchy[OPSKELETON_MAX_BONE_COUNT];
	OPchar* jointNames[OPSKELETON_MAX_BONE_COUNT];
	OPmat4 pose[OPSKELETON_MAX_BONE_COUNT];

	for (i32 i = 0; i < boneCount; i++) {
		hierarchy[i] = OPreadi16(str);
		jointNames[i] = OPreadstring(str);

		for (i32 c = 0; c < 4; c++) {
			pose[i][c][0] = OPreadf32(str);
			pose[i][c][1] = OPreadf32(str);
			pose[i][c][2] = OPreadf32(str);
			pose[i][c][3] = OPreadf32(str);
		}

		pose[i] = OPmat4Transpose(pose[i]);
	}

	*skeleton = OPskeletonCreate(hierarchy, pose, boneCount);

	OPstreamDestroy(str);

	return 1;
}

OPint OPloaderOPskeletonUnload(OPskeleton* skeleton) {
	OPskeletonDestroy(skeleton);
	return 1;
}