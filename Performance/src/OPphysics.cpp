#include "./Performance/include/OPphysics.h"


#ifdef OPIFEX_PHYSICS

	static PxPhysics* gPhysicsSDK = NULL;
	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;
	static PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;
	PxReal myTimestep = 1.0f / 60.0f;

#endif

void OPphysicsInit() {
#ifdef OPIFEX_PHYSICS
	PxFoundation* foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	gPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale());
	// Might not need these yet
	PxInitExtensions(*gPhysicsSDK);
#endif
}

OPphysicsScene* OPphysicsCreateScene() {

#ifdef OPIFEX_PHYSICS
	OPphysicsScene* scene = (OPphysicsScene*)OPalloc(sizeof(OPphysicsScene));

	PxSceneDesc sceneDesc(gPhysicsSDK->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f * 5.0f, 0.0f);

	if (!sceneDesc.cpuDispatcher) {
		PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = mCpuDispatcher;
	}

	if (!sceneDesc.filterShader) {
		sceneDesc.filterShader = gDefaultFilterShader;
		scene->scene = gPhysicsSDK->createScene(sceneDesc);
	}

	scene->elapsed = 0;
	
	return scene;
#else
	return NULL;
#endif
}


void OPphysicsGetTransform(OPphysicsActor* actor, OPmat4* mat)
{
#ifdef OPIFEX_PHYSICS
	PxU32 n = actor->actor->getNbShapes();
	PxShape** shapes = new PxShape*[n];

	actor->actor->getShapes(shapes, n);

	PxTransform pT = PxShapeExt::getGlobalPose(*shapes[0], *actor->actor);

	PxMat33 m = PxMat33(pT.q);

	mat->cols[0].x = m.column0[0];
	mat->cols[0].y = m.column0[1];
	mat->cols[0].z = m.column0[2];
	mat->cols[0].w = 0;

	mat->cols[1].x = m.column1[0];
	mat->cols[1].y = m.column1[1];
	mat->cols[1].z = m.column1[2];
	mat->cols[1].w = 0;

	mat->cols[2].x = m.column2[0];
	mat->cols[2].y = m.column2[1];
	mat->cols[2].z = m.column2[2];
	mat->cols[2].w = 0;

	mat->cols[3].x = pT.p[0];
	mat->cols[3].y = pT.p[1];
	mat->cols[3].z = pT.p[2];
	mat->cols[3].w = 1;
#endif
}


void OPphysicsAddForce(OPphysicsDynamic* dynamic, f32 x, f32 y, f32 z){
#ifdef OPIFEX_PHYSICS
	dynamic->actor->addForce(PxVec3(x, y, z));
#endif
}
void OPphysicsAddTorque(OPphysicsDynamic* dynamic, f32 x, f32 y, f32 z){
#ifdef OPIFEX_PHYSICS
	dynamic->actor->addTorque(PxVec3(x, y, z));
#endif
}

void OPphysicsSetLinearVelocity(OPphysicsDynamic* dynamic, f32 x, f32 y, f32 z){
#ifdef OPIFEX_PHYSICS
	dynamic->actor->setLinearVelocity(PxVec3(x, y, z));
#endif
}

void OPphysicsSetAngularVelocity(OPphysicsDynamic* dynamic, f32 x, f32 y, f32 z){
#ifdef OPIFEX_PHYSICS
	dynamic->actor->setAngularVelocity(PxVec3(x, y, z));
#endif
}


OPphysicsDynamic* OPphysicsCreateBoxDynamic(OPphysicsScene* scene, f32 x, f32 y, f32 z, f32 sx, f32 sy, f32 sz) {
#ifdef OPIFEX_PHYSICS
	PxTransform cubeTransform(PxVec3(x, y, z));

	PxMaterial* boxMaterial = gPhysicsSDK->createMaterial(0.6f, 0.8f, 0.5f);

	PxRigidDynamic* aSphereActor = gPhysicsSDK->createRigidDynamic(cubeTransform);

	PxShape* aSphereShape = aSphereActor->createShape(PxBoxGeometry(sx, sy, sz), *boxMaterial);
	aSphereActor->setMass(sx);

	((PxScene*)scene->scene)->addActor(*aSphereActor);

	OPphysicsDynamic* result = (OPphysicsDynamic*)OPalloc(sizeof(OPphysicsDynamic));
	result->actor = aSphereActor;
	return result;
#else
	return NULL;
#endif
}

OPphysicsDynamic* OPphysicsCreateSphereDynamic(OPphysicsScene* scene, f32 x, f32 y, f32 z, f32 s){
#ifdef OPIFEX_PHYSICS
	PxTransform cubeTransform(PxVec3(x, y, z));

	PxMaterial* boxMaterial = gPhysicsSDK->createMaterial(0.8f, 0.8f, 0.6f);

	PxRigidDynamic* aSphereActor = gPhysicsSDK->createRigidDynamic(cubeTransform);

	PxShape* aSphereShape = aSphereActor->createShape(PxSphereGeometry(s), *boxMaterial);
	aSphereActor->setMass(s);

	((PxScene*)scene->scene)->addActor(*aSphereActor);

	OPphysicsDynamic* result = (OPphysicsDynamic*)OPalloc(sizeof(OPphysicsDynamic));
	result->actor = aSphereActor;
	return result;
#else
	return NULL;
#endif
}

OPphysicsStatic* OPphysicsCreateBoxStatic(OPphysicsScene* scene, f32 x, f32 y, f32 z, f32 sx, f32 sy, f32 sz) {
#ifdef OPIFEX_PHYSICS
	PxTransform cubeTransform(PxVec3(x, y, z));

	PxMaterial* boxMaterial = gPhysicsSDK->createMaterial(0.6f, 0.8f, 0.5f);

	PxRigidStatic* aSphereActor = gPhysicsSDK->createRigidStatic(cubeTransform);

	PxShape* aSphereShape = aSphereActor->createShape(PxBoxGeometry(sx, sy, sz), *boxMaterial);

	((PxScene*)scene->scene)->addActor(*aSphereActor);

	OPphysicsStatic* result = (OPphysicsStatic*)OPalloc(sizeof(OPphysicsStatic));
	result->actor = aSphereActor;
	return result;
#else
	return NULL;
#endif
}

OPphysicsStatic* OPphysicsCreateSphereStatic(OPphysicsScene* scene, f32 x, f32 y, f32 z, f32 s){
#ifdef OPIFEX_PHYSICS
	PxTransform cubeTransform(PxVec3(x, y, z));

	PxMaterial* boxMaterial = gPhysicsSDK->createMaterial(0.8f, 0.8f, 0.6f);

	PxRigidStatic* aSphereActor = gPhysicsSDK->createRigidStatic(cubeTransform);

	PxShape* aSphereShape = aSphereActor->createShape(PxSphereGeometry(s), *boxMaterial);

	((PxScene*)scene->scene)->addActor(*aSphereActor);

	OPphysicsStatic* result = (OPphysicsStatic*)OPalloc(sizeof(OPphysicsStatic));
	result->actor = aSphereActor;
	return result;
#else
	return NULL;
#endif
}

void* OPphysicsCreatePlane(OPphysicsScene* scene){
#ifdef OPIFEX_PHYSICS

	PxTransform cubeTransform(PxVec3(0.0f, 4.0, 0.0f));
	PxMaterial* planeMaterial = gPhysicsSDK->createMaterial(0.9f, 0.1f, 0.2f);
	PxTransform pose = PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* aPlaneActor = gPhysicsSDK->createRigidStatic(pose);
	PxShape* aPlaneShape = aPlaneActor->createShape(PxPlaneGeometry(), *planeMaterial);
	((PxScene*)scene->scene)->addActor(*aPlaneActor);
	return aPlaneActor;
#else
	return NULL;
#endif
}

void OPphysicsStep(OPphysicsScene* scene, ui64 elapsed) {
#ifdef OPIFEX_PHYSICS
	scene->elapsed += elapsed;
	if (scene->elapsed > (1000 * myTimestep)) {
		scene->elapsed -= 1000 * myTimestep;
		((PxScene*)scene->scene)->simulate(myTimestep);
		((PxScene*)scene->scene)->fetchResults(true);
	}

#endif
}

void OPphysicsDestroy(OPphysicsScene* scene)
{
#ifdef OPIFEX_PHYSICS
	((PxScene*)scene->scene)->release();
#endif
}

void OPphysicsShutdown() {
#ifdef OPIFEX_PHYSICS
	gPhysicsSDK->release();
#endif
}