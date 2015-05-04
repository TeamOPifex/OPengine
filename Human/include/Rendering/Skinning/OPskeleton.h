#ifndef OPIFEX_RENDERING_SKINNING_SKELETON
#define OPIFEX_RENDERING_SKINNING_SKELETON

#define NUM_JOINTS 4
#define OPSKELETON_MAX_BONE_COUNT 62

#include "./Math/include/OPmat4.h"
#include "./Math/include/OPvec4.h"

typedef struct {
	ui16 hierarchyCount;
	i16* hierarchy;
	OPchar** jointNames;
	OPmat4* globalPoses;
	OPmat4* globalInvPoses;
	OPmat4* localPoses;
	OPmat4* skinned;
} OPskeleton;

OPskeleton* OPskeletonCreate(i16* hierarchy, OPmat4* pose, i32 count, OPchar** names);
void OPskeletonUpdate(OPskeleton* skeleton);
i16 OPskeletonGet(OPskeleton* skeleton, const OPchar* name);
void OPskeletonDestroy(OPskeleton* skeleton);

inline OPskeleton* OPskeletonCopy(OPskeleton* source) {
	return OPskeletonCreate(source->hierarchy, source->localPoses, source->hierarchyCount, source->jointNames);
}
inline OPmat4 OPskeletonLocal(OPskeleton* skeleton, const OPchar* name) {
	i16 ind = OPskeletonGet(skeleton, name);
	if (ind > -1)  {
		return skeleton->localPoses[ind];
	}
	return OPMAT4IDENTITY;
}
inline OPvec3 OPskeletonLocalTranslate(OPskeleton* skeleton, i16 ind) {
	OPvec3 result = OPVEC3_ZERO;
	while (ind > -1) {
		result += OPmat4GetTranslate(skeleton->localPoses[ind]);
		ind = skeleton->hierarchy[ind];
	}
	return result;
}
inline OPvec3 OPskeletonLocalTranslate(OPskeleton* skeleton, const OPchar* name) {
	i16 ind = OPskeletonGet(skeleton, name);
	return OPskeletonLocalTranslate(skeleton, ind);
}

#endif