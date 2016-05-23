#include "./Performance/include/OPphysXScene.h"

#ifdef OPIFEX_OPTION_PHYSX

PxReal _timestep = 1.0f / 60.0f;
// TODO: (garrett) this should be tied with the OPcore fixed timestep
ui64 _physXTimeStep = 16;

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

void OPphysXSceneUpdate(OPphysXScene* scene, ui64 timestep) {
	scene->scene->simulate(timestep / 1000.f);
	scene->scene->fetchResults(true);
}

void OPphysXSceneUpdate(OPphysXScene* scene, OPtimer* timer) {
	scene->elapsed += timer->Elapsed;
	if (scene->elapsed > _physXTimeStep) { //(1000 * _timestep)
		scene->elapsed -= _physXTimeStep;// 1000 * _timestep;
		scene->scene->simulate(_timestep);
		scene->scene->fetchResults(true);
	}
}

void OPphysXSceneAddWalls(OPphysXScene* scene, OPfloat left, OPfloat right, OPfloat forward, OPfloat backward, OPphysXMaterial* material, i8 addFloor) {
	PxTransform transform;
	OPphysXRigidStatic* plane;

	if(addFloor)
	{
		transform = PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
		plane = OPphysXSceneCreateStatic(scene, transform);
		OPphysXAddPlaneShape(plane, material);
		OPphysXSceneAddActor(scene, plane);
	}

	{
		transform = PxTransform(PxVec3(left, 0.0f, 0.0f), PxQuat(0, PxVec3(1.0f, 0.0f, 0.0f)));
		plane = OPphysXSceneCreateStatic(scene, transform);
		OPphysXAddPlaneShape(plane, material);
		OPphysXSceneAddActor(scene, plane);
	}

	{
		transform = PxTransform(PxVec3(right, 0.0f, 0.0f), PxQuat(PxPi, PxVec3(0.0f, 1.0f, 0.0f)));
		plane = OPphysXSceneCreateStatic(scene, transform);
		OPphysXAddPlaneShape(plane, material);
		OPphysXSceneAddActor(scene, plane);
	}

	{
		transform = PxTransform(PxVec3(0.0f, 0.0f, forward), PxQuat(PxHalfPi, PxVec3(0.0f, 1.0f, 0.0f)));
		plane = OPphysXSceneCreateStatic(scene, transform);
		OPphysXAddPlaneShape(plane, material);
		OPphysXSceneAddActor(scene, plane);
	}

	{
		transform = PxTransform(PxVec3(0.0f, 0.0f, backward), PxQuat(-PxHalfPi, PxVec3(0.0f, 1.0f, 0.0f)));
		plane = OPphysXSceneCreateStatic(scene, transform);
		OPphysXAddPlaneShape(plane, material);
		OPphysXSceneAddActor(scene, plane);
	}
}

#endif
