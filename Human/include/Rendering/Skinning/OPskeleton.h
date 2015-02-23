#ifndef OPIFEX_RENDERING_SKINNING_SKELETON
#define OPIFEX_RENDERING_SKINNING_SKELETON

#define NUM_JOINTS 4
#define OPSKELETON_MAX_BONE_COUNT 62

#include "./Math/include/OPmat4.h"
#include "./Math/include/OPvec4.h"

typedef struct {
	ui16 hierarchyCount;
	i16* hierarchy;
	OPmat4* globalPoses;
	OPmat4* globalInvPoses;
	OPmat4* localPoses;
	OPmat4* skinned;
} OPskeleton;

OPskeleton* OPskeletonCreate(i16* hierarchy, OPmat4* pose, i32 count);
void OPskeletonUpdate(OPskeleton* skeleton);
void OPskeletonDestroy(OPskeleton* skeleton);

inline OPskeleton* OPskeletonCopy(OPskeleton* source) {
	return OPskeletonCreate(source->hierarchy, source->localPoses, source->hierarchyCount);
}

#endif