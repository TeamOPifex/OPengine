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
	OPmat4* bindPose;
	OPmat4* activePose;
	OPmat4* skinned;
	OPmat4* boneOffsets;

	OPmat4 globalInverse;

	i16 JointIndex(const OPchar* joint);
	OPmat4 JointMatrix(i32 joint);
	OPvec3 JointPosition(i32 joint);
	void Reset();


	static OPskeleton* Create(i16* hierarchy, OPmat4* bindPose, OPmat4* boneOffsets, OPmat4 invGlobalPose, i32 count, OPchar** names);
	void Update();
	i16 Get(const OPchar* name);
	void Destroy();

	inline OPskeleton* Copy() {
		return OPskeleton::Create(hierarchy, activePose, boneOffsets, globalInverse, hierarchyCount, jointNames);
	}

	inline OPmat4 Local(const OPchar* name) {
		i16 ind = Get(name);
		if (ind > -1) {
			return activePose[ind];
		}
		return OPMAT4_IDENTITY;
	}

	inline OPvec3 LocalTranslate(i16 ind) {
		OPvec3 result = OPVEC3_ZERO;
		while (ind > -1) {
			result += OPmat4GetTranslate(activePose[ind]);
			ind = hierarchy[ind];
		}
		return result;
	}

	inline OPvec3 LocalTranslate(const OPchar* name) {
		return LocalTranslate(Get(name));
	}
};
