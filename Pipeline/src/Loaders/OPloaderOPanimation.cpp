#include "./Pipeline/include/Loaders/OPloaderOPanimation.h"
#include "./Data/include/OPcman.h"
#include "./Core/include/Assert.h"

void OPskeletonAnimationAddLoader() {
	OPassetLoader loader = {
		".anim",
		"Models/",
		sizeof(OPskeletonAnimation),
		(OPint(*)(const OPchar*, void**))OPloaderOPanimationLoad,
		(OPint(*)(void*))OPloaderOPanimationUnload,
		NULL
	};
	OPcmanAddLoader(&loader);
}

OPint OPloaderOPanimationLoad(const OPchar* filename, OPskeletonAnimation** animation) {
	OPstream* str = OPreadFile(filename);

	i32 boneCount = OPreadi16(str);

	ASSERT(boneCount < OPSKELETON_MAX_BONE_COUNT, "Joints cannot contain more than OPSKELETON_MAX_BONE_COUNT");

	OPchar* name = OPreadstring(str);
	OPint frameCount = OPreadui32(str);
	OPint totalFrames = boneCount * frameCount;

	OPmat4* frames = (OPmat4*)OPalloc(sizeof(OPmat4)* totalFrames);

	for (OPint j = 0; j < totalFrames; j++) {
		for (i32 c = 0; c < 4; c++) {
			frames[j][c][0] = OPreadf32(str);
			frames[j][c][1] = OPreadf32(str);
			frames[j][c][2] = OPreadf32(str);
			frames[j][c][3] = OPreadf32(str);
		}
		frames[j] = OPmat4Transpose(frames[j]);
	}

	*animation = OPskeletonAnimationCreate(boneCount, frames, frameCount);

	OPfree(frames);

	OPfree(name);

	OPstreamDestroy(str);

	return 1;
}

OPint OPloaderOPanimationUnload(OPskeletonAnimation* animation) {
	return 1;
}