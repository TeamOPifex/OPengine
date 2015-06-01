#include "./Performance/include/OPphysXScene.h"

PxReal _timestep = 1.0f / 60.0f;

void OPphysXSceneInit(OPphysXScene* scene, void(*onTrigger)(OPphysXTrigger), void(*onContact)(OPphysXContact)) {

	PxSceneDesc sceneDesc(OPphysXSDK->getTolerancesScale());

	if(onTrigger != NULL || onContact != NULL) {
		sceneDesc.simulationEventCallback = new OPphysXEventHandler(onTrigger, onContact);
	}

	sceneDesc.gravity = PxVec3(0.0f, -9.8f * 5.0f, 0.0f);

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

OPphysXScene* OPphysXSceneCreate() {
	OPphysXScene* scene = (OPphysXScene*)OPallocZero(sizeof(OPphysXScene));
	OPphysXSceneInit(scene, NULL, NULL);
	return scene;
}

OPphysXScene* OPphysXSceneCreate(void(*onTrigger)(OPphysXTrigger), void(*onContact)(OPphysXContact)) {
	OPphysXScene* scene = (OPphysXScene*)OPallocZero(sizeof(OPphysXScene));
	OPphysXSceneInit(scene, onTrigger, onContact);
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
