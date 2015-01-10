#ifndef OPIFEX_RENDERING_SKINNING_SKELETON
#define OPIFEX_RENDERING_SKINNING_SKELETON

#define NUM_JOINTS 4

#include "./Math/include/OPmat4.h"
#include "./Math/include/OPvec4.h"

typedef struct {
	i16* hierarchy;
	ui16 hierarchyCount;
	OPmat4* globalPoses;
	OPmat4* globalInvPoses;
	OPmat4* localPoses;
	OPmat4* skinned;
} OPskeleton;

EXPORT_METHOD OPskeleton* OPskeletonCreate(i16* hierarchy, OPmat4* pose, i32 count);
EXPORT_METHOD void OPskeletonUpdate(OPskeleton* skeleton);

#endif