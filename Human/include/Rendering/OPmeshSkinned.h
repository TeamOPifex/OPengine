#ifndef OPENGINE_HUMAN_RENDERER_MESHSKINNED
#define OPENGINE_HUMAN_RENDERER_MESHSKINNED

#include "./Human/include/Rendering/OPmesh.h"
#include "./Math/include/OPmat4.h"
#include "./Math/include/OPvec3.h"
#include "./Math/include/OPvec4.h"


	typedef struct {
		OPint Bone;
		OPfloat Time;

		//OPint Features;
		//OPvec3 Position;
		OPvec4 Rotation;
	} OPmeshSkinnedKeyframe;

	typedef struct {
		OPint KeyframeCount;
		OPmeshSkinnedKeyframe* Frames;
	} OPmeshSkinnedAnimation;

	typedef struct {
		ui16 JointCount;
		ui16* Heirarchy;
		OPmat4* LocalPoses;
		OPmat4* GlobalPoses;
	} OPmeshSkinnedSkeleton;

	typedef struct {
		OPmesh Mesh;
		OPmeshSkinnedSkeleton* Skeleton;
		OPint AnimationCount;
		OPmeshSkinnedAnimation* Animations;
	} OPmeshSkinned;

	void OPmeshSkinnedLocalToGlobalPose(OPmeshSkinned* skinned)
	{
		// the root has no parent
		skinned->Skeleton->GlobalPoses[0] = skinned->Skeleton->LocalPoses[0];

		for (ui16 i = 1; i < skinned->Skeleton->JointCount; ++i)
		{
			OPmat4mul(&skinned->Skeleton->GlobalPoses[i], 
				&skinned->Skeleton->GlobalPoses[skinned->Skeleton->Heirarchy[i]],
				&skinned->Skeleton->LocalPoses[i]);
		}
	}


#endif