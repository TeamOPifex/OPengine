#ifndef OP_PERFORMANCE_AABB2
#define OP_PERFORMANCE_AABB2

#include "./Math/include/Matrix4.h"

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

#endif

typedef struct {
	void* scene;
	ui64 elapsed;
} OPphysicsScene;

typedef struct {
#ifdef OPIFEX_PHYSICS
	PxRigidDynamic* actor;
#endif
} OPphysicsDynamic;

typedef struct {
#ifdef OPIFEX_PHYSICS
	PxRigidStatic* actor;
#endif
} OPphysicsStatic;

typedef struct {
#ifdef OPIFEX_PHYSICS
	PxRigidActor* actor;
#endif
} OPphysicsActor;

void OPphysicsInit();
OPphysicsScene* OPphysicsCreateScene();
void OPphysicsStep(OPphysicsScene* scene, ui64 elapsed);
void OPphysicsDestroy(OPphysicsScene* scene);
void OPphysicsShutdown();

void OPphysicsAddForce(OPphysicsDynamic* dynamic, f32 x, f32 y, f32 z);
void OPphysicsAddTorque(OPphysicsDynamic* dynamic, f32 x, f32 y, f32 z);
void OPphysicsSetLinearVelocity(OPphysicsDynamic* dynamic, f32 x, f32 y, f32 z);
void OPphysicsSetAngularVelocity(OPphysicsDynamic* dynamic, f32 x, f32 y, f32 z);

void OPphysicsGetTransform(OPphysicsActor* actor, OPmat4* mat);
OPphysicsDynamic* OPphysicsCreateBoxDynamic(OPphysicsScene* scene, f32 x, f32 y, f32 z, f32 sx, f32 sy, f32 sz);
OPphysicsDynamic* OPphysicsCreateSphereDynamic(OPphysicsScene* scene, f32 x, f32 y, f32 z, f32 s);
OPphysicsStatic* OPphysicsCreateBoxStatic(OPphysicsScene* scene, f32 x, f32 y, f32 z, f32 sx, f32 sy, f32 sz);
OPphysicsStatic* OPphysicsCreateSphereStatic(OPphysicsScene* scene, f32 x, f32 y, f32 z, f32 s);
void* OPphysicsCreatePlane(OPphysicsScene* scene);


#endif