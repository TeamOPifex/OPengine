#ifndef OP_PERFORMANCE_PHYSXSCENE
#define OP_PERFORMANCE_PHYSXSCENE

#include "./Core/include/OPtypes.h"

#ifdef OPIFEX_OPTION_PHYSX

#include "./Performance/include/OPphysX.h"
#include "./Performance/include/OPphysXEventHandler.h"

typedef struct {
	PxScene* scene;
	ui64 elapsed;
} OPphysXScene;

// FUNCTIONS

void OPphysXSceneInit(OPphysXScene* scene, void(*onTrigger)(OPphysXTrigger), void(*onContact)(OPphysXContact));
OPphysXScene* OPphysXSceneCreate();
OPphysXScene* OPphysXSceneCreate(void(*onTrigger)(OPphysXTrigger), void(*onContact)(OPphysXContact));
OPphysXRigidDynamic* OPphysXSceneCreateDynamic(OPphysXScene* scene, OPvec3 position);
OPphysXRigidDynamic* OPphysXSceneCreateDynamic(OPphysXScene* scene, PxTransform transform);
OPphysXRigidStatic* OPphysXSceneCreateStatic(OPphysXScene* scene, OPvec3 position);
OPphysXRigidStatic* OPphysXSceneCreateStatic(OPphysXScene* scene, PxTransform transform);
void OPphysXSceneUpdate(OPphysXScene* scene, OPtimer* timer);

// INLINES

inline void OPphysXSceneAddActor(OPphysXScene* scene, PxRigidActor* actor) {
	scene->scene->addActor(*actor);
}

inline void OPphysXSceneRemove(OPphysXScene* scene, OPphysXRigidActor* actor) {
	scene->scene->removeActor(*actor);
}

inline void OPphysXSceneDestroy(OPphysXScene* scene) {
	scene->scene->release();
}

#endif

#endif