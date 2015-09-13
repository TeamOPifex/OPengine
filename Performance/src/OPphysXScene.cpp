#include "./Performance/include/OPphysXScene.h"

#ifdef OPIFEX_OPTION_PHYSX

PxReal _timestep = 1.0f / 60.0f;

void OPphysXSceneInit(OPphysXScene* scene, OPvec3 gravity, void(*onTrigger)(OPphysXTrigger), void(*onContact)(OPphysXContact)) {

	PxSceneDesc sceneDesc(OPphysXSDK->getTolerancesScale());

	if(onTrigger != NULL || onContact != NULL) {
		sceneDesc.simulationEventCallback = new OPphysXEventHandler(onTrigger, onContact);
	}

	sceneDesc.gravity = PxVec3(gravity.x, gravity.y, gravity.z);

	if (!sceneDesc.cpuDispatcher) {
		PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = mCpuDispatcher;
	}

	if (!sceneDesc.filterShader) {
		sceneDesc.filterShader = OPphysXDefaultFilterShader;
	}

	scene->scene = OPphysXSDK->createScene(sceneDesc);
	scene->elapsed = 0;
}

OPphysXScene* OPphysXSceneCreate(OPvec3 gravity) {
	OPphysXScene* scene = (OPphysXScene*)OPallocZero(sizeof(OPphysXScene));
	OPphysXSceneInit(scene, gravity, NULL, NULL);
	return scene;
}

OPphysXScene* OPphysXSceneCreate(OPvec3 gravity, void(*onTrigger)(OPphysXTrigger), void(*onContact)(OPphysXContact)) {
	OPphysXScene* scene = (OPphysXScene*)OPallocZero(sizeof(OPphysXScene));
	OPphysXSceneInit(scene, gravity, onTrigger, onContact);
	return scene;
}

OPphysXRigidDynamic* OPphysXSceneCreateDynamic(OPphysXScene* scene, OPvec3 position) {
 	PxTransform transform(PxVec3(position.x, position.y, position.z));
	OPphysXRigidDynamic* actor = OPphysXSDK->createRigidDynamic(transform);
	return actor;
}

OPphysXRigidDynamic* OPphysXSceneCreateDynamic(OPphysXScene* scene, PxTransform transform) {
	OPphysXRigidDynamic* actor = OPphysXSDK->createRigidDynamic(transform);
	return actor;
}

OPphysXRigidStatic* OPphysXSceneCreateStatic(OPphysXScene* scene, OPvec3 position, OPfloat rotate, OPvec3 around) {
	PxTransform transform = PxTransform(PxVec3(position.x, position.y, position.z), PxQuat(rotate, PxVec3(around.x, around.y, around.z)));
	OPphysXRigidStatic* actor = OPphysXSDK->createRigidStatic(transform);
	return actor;
}


OPphysXRigidStatic* OPphysXSceneCreateStatic(OPphysXScene* scene, OPvec3 position) {
 	PxTransform transform(PxVec3(position.x, position.y, position.z));
	OPphysXRigidStatic* actor = OPphysXSDK->createRigidStatic(transform);
	return actor;
}

OPphysXRigidStatic* OPphysXSceneCreateStatic(OPphysXScene* scene, PxTransform transform) {
	OPphysXRigidStatic* actor = OPphysXSDK->createRigidStatic(transform);
	return actor;
}



void OPphysXSceneUpdate(OPphysXScene* scene, OPtimer* timer) {
	scene->elapsed += timer->Elapsed;
	if (scene->elapsed > (1000 * _timestep)) {
		scene->elapsed -= 1000 * _timestep;
		scene->scene->simulate(_timestep);
		scene->scene->fetchResults(true);
	}
}

#endif
