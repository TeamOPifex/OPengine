#ifndef OP_PERFORMANCE_PHYSICS
#define OP_PERFORMANCE_PHYSICS

#include "./Math/include/OPmat4.h"

#ifdef OPIFEX_OPTION_PHYSICS

#include <PxPhysicsAPI.h>
#include <extensions/PxExtensionsAPI.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <extensions/PxDefaultCpuDispatcher.h>
#include <extensions/PxShapeExt.h>
#include <extensions/PxSimpleFactory.h>

#include <foundation/PxFoundation.h>
#include <foundation/PxMat33.h>


//#ifdef NDEBUG
//#pragma comment(lib, "PhysX3_x86.lib")
//#pragma comment(lib, "PhysX3Common_x86.lib")
//#pragma comment(lib, "PhysX3Extensions.lib")
//#pragma comment(lib, "PxTask.lib")
//#else
//#pragma comment(lib, "PhysX3DEBUG_x86.lib")
//#pragma comment(lib, "PhysX3CommonDEBUG_x86.lib")
//#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")
//#pragma comment(lib, "PxTaskDEBUG.lib")
//#endif

using namespace physx;
using namespace std;

extern PxPhysics* gPhysicsSDK;

#endif

typedef struct {
#ifdef OPIFEX_OPTION_PHYSICS
	PxTriggerPair* triggerPairs;
	ui32 count;
#endif
} OPphysicsTrigger;

#ifdef OPIFEX_OPTION_PHYSICS
class OPphysicsSimulationEventCallbackHandler : public PxSimulationEventCallback 
{
public:
	OPphysicsSimulationEventCallbackHandler(void(*onTrigger)(OPphysicsTrigger)) {
		_onTrigger = onTrigger;
	}
private:
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
		OPlog("Contact");
	}
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) {
		OPphysicsTrigger trigger = {
			pairs,
			count
		};
		_onTrigger(trigger);
	}
	virtual void onConstraintBreak(PxConstraintInfo*, PxU32) {}
	virtual void onWake(PxActor** , PxU32 ) {}
	virtual void onSleep(PxActor** , PxU32 ){}

	void(*_onTrigger)(OPphysicsTrigger);
};

#endif

typedef struct {
	void* scene;
	ui64 elapsed;
#ifdef OPIFEX_OPTION_PHYSICS
	OPphysicsSimulationEventCallbackHandler* onTrigger;
#endif
} OPphysicsScene;

typedef struct {
#ifdef OPIFEX_OPTION_PHYSICS
	PxRigidDynamic* actor;
#endif
} OPphysicsDynamic;

typedef struct {
#ifdef OPIFEX_OPTION_PHYSICS
	PxRigidStatic* actor;
#endif
} OPphysicsStatic;

typedef struct {
#ifdef OPIFEX_OPTION_PHYSICS
	PxRigidActor* actor;
#endif
} OPphysicsActor;

// typedef struct {
// #ifdef OPIFEX_OPTION_PHYSICS
// 	PxControllerManager* manager;
// #endif
// } OPphysicsCharacterManager;

// typedef struct {
// #ifdef OPIFEX_OPTION_PHYSICS
// 	ControlledActor* actor;
// #endif
// } OPphysicsControlledActor;

void OPphysicsInit();
OPphysicsScene* OPphysicsCreateScene(void(*onTrigger)(OPphysicsTrigger));
void OPphysicsRemoveDynamic(OPphysicsScene* scene, OPphysicsDynamic* actor);
void OPphysicsRemoveStatic(OPphysicsScene* scene, OPphysicsStatic* actor);
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
OPphysicsStatic* OPphysicsCreatePlane(OPphysicsScene* scene);

#endif