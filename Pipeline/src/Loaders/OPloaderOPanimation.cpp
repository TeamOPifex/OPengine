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
	OPCMAN.AddLoader(&loader);
}

OPint OPloaderOPanimationLoad(OPstream* str, OPskeletonAnimation** animation) {
	
	// Animation Name
	OPchar* name = str->String();

	// Number of bones to track
	i32 boneCount = str->I16(); 
	ASSERT(boneCount < OPSKELETON_MAX_BONE_COUNT, "Joints cannot contain more than OPSKELETON_MAX_BONE_COUNT");

	OPint frameCount = str->UI32(); // Number of Animation Keys
	OPint totalFrames = boneCount * frameCount; // Total Frames

	OPlog("Name: %s, Frames: %d", name, frameCount);

	OPuint totalSize = sizeof(OPanimationFrame) * totalFrames;

	OPanimationFrame* frames = (OPanimationFrame*)OPalloc(totalSize);

	OPmemcpy(frames, str->Read(totalSize), totalSize);

	//for (ui32 i = 0; i < boneCount; i++) {
	//	//str->String(); // bone name
	//	OPmemcpy(&frames[i], str->Read(sizeof(OPanimationFrame) * frameCount), sizeof(OPanimationFrame) * frameCount);
	//}

	// TODO: Change to Pos, Rot, Scale frames
	//OPmat4* frames = (OPmat4*)OPalloc(sizeof(OPmat4)* totalFrames);

	//for (OPint j = 0; j < totalFrames; j++) {
	//	for (i32 c = 0; c < 4; c++) {
	//		frames[j][c][0] = str->F32();
	//		frames[j][c][1] = str->F32();
	//		frames[j][c][2] = str->F32();
	//		frames[j][c][3] = str->F32();
	//	}
	//	frames[j] = OPmat4Transpose(frames[j]);
	//}

	*animation = OPskeletonAnimationCreate(boneCount, frames, frameCount, name);

	//OPfree(frames);

	OPfree(name);

	//OPstreamDestroy(str);

	return 1;
}

OPint OPloaderOPanimationUnload(OPskeletonAnimation* animation) {
	return 1;
}