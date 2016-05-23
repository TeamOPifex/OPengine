#ifndef OP_PERFORMANCE_PHYSXSCENE
#define OP_PERFORMANCE_PHYSXSCENE

#include "./Core/include/OPtypes.h"

#ifdef OPIFEX_OPTION_PHYSX

#include "./Performance/include/OPphysX.h"
#include "./Performance/include/OPphysXEventHandler.h"

struct OPphysXScene {
	PxScene* scene;
	ui64 elapsed;
};

// FUNCTIONS

void OPphysXSceneInit(OPphysXScene* scene, OPvec3 gravity, void(*onTrigger)(OPphysXTrigger), void(*onContact)(OPphysXContact));
OPphysXScene* OPphysXSceneCreate(OPvec3 gravity);
OPphysXScene* OPphysXSceneCreate(OPvec3 gravity, void(*onTrigger)(OPphysXTrigger), void(*onContact)(OPphysXContact));
OPphysXRigidDynamic* OPphysXSceneCreateDynamic(OPphysXScene* scene, OPvec3 position);
OPphysXRigidDynamic* OPphysXSceneCreateDynamic(OPphysXScene* scene, PxTransform transform);
OPphysXRigidStatic* OPphysXSceneCreateStatic(OPphysXScene* scene, OPvec3 position);
OPphysXRigidStatic* OPphysXSceneCreateStatic(OPphysXScene* scene, PxTransform transform);
OPphysXRigidStatic* OPphysXSceneCreateStatic(OPphysXScene* scene, OPvec3 position, OPfloat rotate, OPvec3 around);
void OPphysXSceneUpdate(OPphysXScene* scene, ui64 timestep);
void OPphysXSceneUpdate(OPphysXScene* scene, OPtimer* timer);
i8 OPphysXShapeOverlapping(OPphysXRigidActor* actor, OPphysXShape* shape, PxGeometry* otherGeometry, PxTransform otherTransform);
void OPphysXSceneAddWalls(OPphysXScene* scene, OPfloat left, OPfloat right, OPfloat forward, OPfloat backward, OPphysXMaterial* material, i8 addFloor);

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
