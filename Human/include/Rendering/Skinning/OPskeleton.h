#pragma once

struct OPskeleton;
typedef struct OPskeleton OPskeleton;

#define NUM_JOINTS 4
#define OPSKELETON_MAX_BONE_COUNT 100

#include "./Math/include/OPmat4.h"
#include "./Math/include/OPvec4.h"

struct OPskeleton {
	ui16 hierarchyCount;
	i16* hierarchy;
	OPchar** jointNames;
	OPmat4* globalPoses;
	OPmat4* globalInvPoses;
	OPmat4* localPoses;
	OPmat4* skinned;
	OPmat4* bindPoses;

	OPmat4 globalInverse;

	i16 JointIndex(const OPchar* joint);
	OPmat4 JointMatrix(i32 joint);
	OPvec3 JointPosition(i32 joint);
};

OPskeleton* OPskeletonCreate(i16* hierarchy, OPmat4* bindPose, OPmat4* boneOffsets, OPmat4 invGlobalPose, i32 count, OPchar** names);
void OPskeletonUpdate(OPskeleton* skeleton);
i16 OPskeletonGet(OPskeleton* skeleton, const OPchar* name);
void OPskeletonDestroy(OPskeleton* skeleton);

inline OPskeleton* OPskeletonCopy(OPskeleton* source) {
	return OPskeletonCreate(source->hierarchy, source->localPoses, source->bindPoses, source->globalInverse, source->hierarchyCount, source->jointNames);
}

inline OPmat4 OPskeletonLocal(OPskeleton* skeleton, const OPchar* name) {
	i16 ind = OPskeletonGet(skeleton, name);
	if (ind > -1)  {
		return skeleton->localPoses[ind];
	}
	return OPMAT4_IDENTITY;
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