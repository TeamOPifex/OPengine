#include "./Performance/include/Physics.h"

#ifdef OPIFEX_PHYSICS

	#include <PxPhysicsAPI.h>
	#include <extensions\PxExtensionsAPI.h>
	#include <extensions\PxDefaultErrorCallback.h>
	#include <extensions\PxDefaultAllocator.h>
	#include <extensions\PxDefaultSimulationFilterShader.h>
	#include <extensions\PxDefaultCpuDispatcher.h>
	#include <extensions\PxShapeExt.h>
	#include <extensions\PxSimpleFactory.h>

	#include <foundation\PxFoundation.h>
	#include <foundation\PxMat33.h>

	#pragma comment(lib, "PhysX3CHECKED_x86.lib")
	#pragma comment(lib, "PhysX3CommonCHECKED_x86.lib")
	#pragma comment(lib, "PhysX3ExtensionsCHECKED.lib")
	#pragma comment(lib, "PxTaskCHECKED.lib")

	using namespace physx;
	using namespace std;

	static PxPhysics* gPhysicsSDK = NULL;
	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;
	static PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;
	PxScene* gScene = NULL;
	PxReal myTimestep = 1.0f / 60.0f;

#endif

void OPphysicsInitialize() {
#ifdef OPIFEX_PHYSICS
	PxFoundation* foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	gPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale());
	// Might not need these yet
	PxInitExtensions(*gPhysicsSDK);
	PxSceneDesc sceneDesc(gPhysicsSDK->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);

	if (!sceneDesc.cpuDispatcher) {
		PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		sceneDesc.cpuDispatcher = mCpuDispatcher;
	}

	if (!sceneDesc.filterShader) {
		sceneDesc.filterShader = gDefaultFilterShader;
		gScene = gPhysicsSDK->createScene(sceneDesc);
	}
#endif
}



void OPphysicsGetTransform(void* actor, OPmat4* mat)
{
	PxRigidActor* pActor = (PxRigidActor*)actor;
	PxU32 n = pActor->getNbShapes();

	PxShape** shapes = new PxShape*[n];

	pActor->getShapes(shapes, n);


	


	PxTransform pT = PxShapeExt::getGlobalPose(*shapes[0], *pActor);
	
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
}

void* OPphysicsBoxCreate(){
	PxTransform cubeTransform(PxVec3(0.0f, 4.0, 0.0f));
	PxMaterial* boxMaterial = gPhysicsSDK->createMaterial(0.6f, 0.1f, 0.6f);
	PxRigidDynamic* aSphereActor = gPhysicsSDK->createRigidDynamic(cubeTransform);
	PxShape* aSphereShape = aSphereActor->createShape(PxSphereGeometry(1), *boxMaterial);
	gScene->addActor(*aSphereActor);
	return aSphereActor;
}
void* OPphysicsPlaneCreate(){

	PxTransform cubeTransform(PxVec3(0.0f, 4.0, 0.0f));
	PxMaterial* planeMaterial = gPhysicsSDK->createMaterial(0.9f, 0.1f, 1.0f);
	PxTransform pose = PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* aPlaneActor = gPhysicsSDK->createRigidStatic(pose);
	PxShape* aPlaneShape = aPlaneActor->createShape(PxPlaneGeometry(), *planeMaterial);
	gScene->addActor(*aPlaneActor);
	return aPlaneActor;
}

void OPphysicsStep() {
#ifdef OPIFEX_PHYSICS
	gScene->simulate(myTimestep);

	gScene->fetchResults(true);

#endif
}

void OPphysicsDestroy()
{
#ifdef OPIFEX_PHYSICS
	gScene->release();
	gPhysicsSDK->release();
#endif
}
