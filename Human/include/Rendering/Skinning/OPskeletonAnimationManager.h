#ifndef OPIFEX_RENDERING_SKINNING_ANIMATION_MANAGER
#define OPIFEX_RENDERING_SKINNING_ANIMATION_MANAGER

#include "./Human/include/Rendering/Skinning/OPskeletonAnimation.h"
#include "./Human/include/Rendering/Skinning/OPskeletonAnimationTransition.h"
#include "./Core/include/Assert.h"

#define OPMAX_ANIMATION_MERGES 3

// STEPS
// Update Current Animation Mix
// If there's another animation mix
//	Update Next Animation Mix
//  Transition animation mixes together


// Mixes animations together
struct OPskeletonAnimationMix {
	OPskeletonAnimation* animations[OPMAX_ANIMATION_MERGES];
	i16 mergeJoints[OPMAX_ANIMATION_MERGES];
};

// A transition animation grouping
struct OPskeletonAnimationManagerTransition {
	OPskeletonAnimation* animation;
	OPfloat transitionTime;
};

// The manager itself, which will automatically merge and play animations appropriately
struct OPskeletonAnimationManager {
	OPskeleton* skeleton;
	i16 animationIndex;
	OPskeletonAnimation* animations[OPMAX_ANIMATION_MERGES];
	i16 animationJointMix[OPMAX_ANIMATION_MERGES];
	OPskeletonAnimationManagerTransition transition;
	OPskeletonAnimationManagerTransition buffer;
	OPfloat currentTime;
};
typedef struct OPskeletonAnimationManager OPskeletonAnimationManager;

inline void OPskeletonAnimationManagerInit(OPskeletonAnimationManager* manager, OPskeleton* skeleton) {
	manager->skeleton = skeleton;
	manager->transition.animation = NULL;
	manager->buffer.animation = NULL;
	manager->animations[0] = NULL;
	manager->animationJointMix[0] = 0;
	manager->animationIndex = 0;
}

inline OPskeletonAnimationManager* OPskeletonAnimationManagerCreate(OPskeleton* skeleton) {
	OPskeletonAnimationManager* result = (OPskeletonAnimationManager*)OPalloc(sizeof(OPskeletonAnimationManager));
	OPskeletonAnimationManagerInit(result, skeleton);
	return result;
}

inline void OPskeletonAnimationManagerSet(OPskeletonAnimationManager* manager, OPskeletonAnimation* animation) {
	manager->animations[0] = animation;
	manager->animationIndex = 1;
	OPskeletonAnimationReset(animation);
}

inline void OPskeletonAnimationManagerMix(OPskeletonAnimationManager* manager, OPskeletonAnimation* animation, i16 fromJoint) {
	manager->animations[1] = animation;
	manager->animationJointMix[1] = fromJoint;
	manager->animationIndex = 2;
	OPskeletonAnimationReset(animation);
}

inline void OPskeletonAnimationManagerTransition(OPskeletonAnimationManager* manager, OPskeletonAnimation* animation, OPfloat duration) {
	if(manager->animations[0] == animation) return;

	if(manager->transition.animation == NULL) {
		manager->transition.animation = animation;
		manager->transition.transitionTime = duration;
		manager->currentTime = 0.0f;
	} else {
		manager->buffer.animation = animation;
		manager->buffer.transitionTime = duration;
	}
}

inline void OPskeletonAnimationManagerUpdate(OPskeletonAnimationManager* manager, OPtimer* timer, OPfloat timeScale) {

	ASSERT(manager->animations[0] != NULL, "An animation must be set first.");

	// Update the current time stamp so that we can get the percentage
	// of the animations to merge together
	manager->currentTime += timer->Elapsed;
	OPfloat percentage = manager->currentTime / manager->transition.transitionTime;

	// If the animation isn't NULL then there's animation that we're
	// trying to merge in
	if(manager->transition.animation != NULL) {

		// If we're over 100% we need to swap from a transition
		if(percentage > 1.0f) {
			manager->animations[0] = manager->transition.animation;

			// Ensure that there wasn't an overflow buffer set already
			if(manager->buffer.animation != NULL) {
				manager->transition = manager->buffer;
				manager->buffer.animation = NULL;
			} else {
				manager->transition.animation = NULL;
			}
		}
	}

	for(ui16 i = 0; i < manager->animationIndex; i++) {
		OPskeletonAnimationUpdate(manager->animations[i], timer, timeScale);
	}

	if(manager->transition.animation != NULL) {
		OPskeletonAnimationUpdate(manager->transition.animation, timer, timeScale);
		OPskeletonAnimationMerge(manager->animations[0], manager->transition.animation, percentage, manager->skeleton);
	} else {
		for(ui16 i = 0; i < manager->animationIndex; i++) {
			OPskeletonAnimationApply(manager->animations[i], manager->skeleton, manager->animationJointMix[i]);
		}
	}

	OPskeletonUpdate(manager->skeleton);

}
inline void OPskeletonAnimationManagerUpdate(OPskeletonAnimationManager* manager, OPtimer* timer) {
	OPskeletonAnimationManagerUpdate(manager, timer, 1.0f);
}

inline void OPskeletonAnimationManagerDestroy(OPskeletonAnimationManager* manager) {

}

inline void OPskeletonAnimationManagerFree(OPskeletonAnimationManager* manager) {
	OPskeletonAnimationManagerDestroy(manager);
	OPfree(manager);
}

#endif
