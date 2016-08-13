#include "./Pipeline/include/Loaders/OPloaderOPanimation.h"
#include "./Data/include/OPcman.h"
#include "./Core/include/Assert.h"

void OPskeletonAnimationAddLoader() {
	OPassetLoader loader = {
		".anim",
		"Models/",
		sizeof(OPskeletonAnimation),
		(OPint(*)(OPstream*, void**))OPloaderOPanimationLoad,
		(OPint(*)(void*))OPloaderOPanimationUnload,
		NULL
	};
	OPcmanAddLoader(&loader);
}

OPint OPloaderOPanimationLoad(OPstream* str, OPskeletonAnimation** animation) {
	//OPstream* str = OPreadFile(filename);

	i32 boneCount = str->I16();

	ASSERT(boneCount < OPSKELETON_MAX_BONE_COUNT, "Joints cannot contain more than OPSKELETON_MAX_BONE_COUNT");

	OPchar* name = str->String();
	OPint frameCount = str->UI32();
	OPint totalFrames = boneCount * frameCount;

	OPlog("Name: %s, Frames: %d", name, frameCount);

	OPmat4* frames = (OPmat4*)OPalloc(sizeof(OPmat4)* totalFrames);

	for (OPint j = 0; j < totalFrames; j++) {
		for (i32 c = 0; c < 4; c++) {
			frames[j][c][0] = str->F32();
			frames[j][c][1] = str->F32();
			frames[j][c][2] = str->F32();
			frames[j][c][3] = str->F32();
		}
		frames[j] = OPmat4Transpose(frames[j]);
	}

	*animation = OPskeletonAnimationCreate(boneCount, frames, frameCount);

	OPfree(frames);

	OPfree(name);

	//OPstreamDestroy(str);

	return 1;
}

OPint OPloaderOPanimationUnload(OPskeletonAnimation* animation) {
	return 1;
}