#include "./Human/include/Rendering/Skinning/Animation.h"

OPskeleton* OPskeletonCreate(i16* hierarchy, OPmat4* pose, i32 count) {
	OPskeleton* skeleton = (OPskeleton*)OPalloc(sizeof(OPskeleton));
	skeleton->hierarchy = hierarchy;
	skeleton->hierarchyCount = count;
	skeleton->localPoses = (OPmat4*)OPalloc(sizeof(OPmat4)* count);
	skeleton->globalPoses = (OPmat4*)OPalloc(sizeof(OPmat4)* count);

	for (i32 i = 0; i < count; i++) {
		skeleton->localPoses[i] = pose[i];
		OPmat4identity(&skeleton->globalPoses[i]);
	}

	return skeleton;
}

void OPskeletonUpdate(OPskeleton* skeleton) {
	skeleton->globalPoses[0] = skeleton->localPoses[0];

	for (ui32 i = 1; i < skeleton->hierarchyCount; ++i) {
		skeleton->globalPoses[i] = 
			skeleton->globalPoses[skeleton->hierarchy[i]] * 
			skeleton->localPoses[i];
	}
}