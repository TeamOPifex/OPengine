#ifndef OPENGINE_HUMAN_RENDERER_MESHSKINNED
#define OPENGINE_HUMAN_RENDERER_MESHSKINNED

#include "./Human/include/Rendering/OPmesh.h"
#include "./Math/include/OPmat4.h"
#include "./Math/include/OPvec3.h"
#include "./Math/include/OPvec4.h"


	struct OPmeshSkinnedKeyframe {
		OPint Bone;
		OPfloat Time;

		//OPint Features;
		//OPvec3 Position;
		OPvec4 Rotation;
	};
	typedef struct OPmeshSkinnedKeyframe OPmeshSkinnedKeyframe;

	struct OPmeshSkinnedAnimation {
		OPint KeyframeCount;
		OPmeshSkinnedKeyframe* Frames;
	};
	typedef struct OPmeshSkinnedAnimation OPmeshSkinnedAnimation;

	struct OPmeshSkinnedSkeleton {
		ui16 JointCount;
		ui16* Heirarchy;
		OPmat4* LocalPoses;
		OPmat4* GlobalPoses;
	};
	typedef struct OPmeshSkinnedSkeleton OPmeshSkinnedSkeleton;

	struct OPmeshSkinned {
		OPmesh Mesh;
		OPmeshSkinnedSkeleton* Skeleton;
		OPint AnimationCount;
		OPmeshSkinnedAnimation* Animations;
	};
	typedef struct OPmeshSkinned OPmeshSkinned;

	void OPmeshSkinnedLocalToGlobalPose(OPmeshSkinned* skinned)
	{
		// the root has no parent
		skinned->Skeleton->GlobalPoses[0] = skinned->Skeleton->LocalPoses[0];

		for (ui16 i = 1; i < skinned->Skeleton->JointCount; ++i)
		{
			OPmat4Mul(&skinned->Skeleton->GlobalPoses[i],
				skinned->Skeleton->GlobalPoses[skinned->Skeleton->Heirarchy[i]],
				skinned->Skeleton->LocalPoses[i]);
		}
	}


#endif
