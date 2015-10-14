#ifndef OP_PERFORMANCE_PHYSX
#define OP_PERFORMANCE_PHYSX

#include "./Core/include/OPtypes.h"

#ifdef OPIFEX_OPTION_PHYSX

// INCLUDES

#include "./Math/include/OPmat4.h"
#include "./Math/include/OPvec3.h"
#include "./Core/include/OPtimer.h"
#include <PxPhysicsAPI.h>
#include <cooking/PxCooking.h>
#include <extensions/PxExtensionsAPI.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <extensions/PxDefaultCpuDispatcher.h>
#include <extensions/PxShapeExt.h>
#include <extensions/PxSimpleFactory.h>

#include <foundation/PxFoundation.h>
#include <foundation/PxMat33.h>

using namespace physx;
using namespace std;

// EXTERNS

extern PxPhysics* OPphysXSDK;
extern PxSimulationFilterShader OPphysXDefaultFilterShader;

// TYPES

typedef PxRigidActor OPphysXRigidActor;
typedef PxRigidStatic OPphysXRigidStatic;
typedef PxRigidDynamic OPphysXRigidDynamic;
typedef PxMaterial OPphysXMaterial;
typedef PxShape OPphysXShape;

// FUNCTIONS

void OPphysXInit();
void OPphysXDebugger();
PxTriangleMesh* OPphysXCreateTriangleMesh(ui32 vertCount, PxVec3* verts, ui32 triCount, PxU32* indices);
PxTriangleMesh* OPphysXCreateTriangleMesh(ui32 vertCount, PxVec3* verts, ui32 vertStride, ui32 triCount, PxU32* indices, ui32 indStride);
void OPphysXSetFilter(OPphysXRigidActor* actor, ui32 filterGroup, ui32 filterMask);
OPphysXMaterial* OPphysXCreateMaterial(f32 staticFriction, f32 dynamicFriction, f32 restitution);
OPphysXShape* OPphysXAddSphereShape(OPphysXRigidActor* actor, OPphysXMaterial* material, OPfloat size);
OPphysXShape* OPphysXAddBoxShape(OPphysXRigidActor* actor, OPphysXMaterial* material, OPvec3 size);
OPphysXShape* OPphysXAddPlaneShape(OPphysXRigidActor* actor, OPphysXMaterial* material);
OPphysXShape* OPphysXAddTriangleMeshShape(OPphysXRigidActor* actor, OPphysXMaterial* material, PxTriangleMesh* mesh);
void OPphysXGetTransform(OPphysXRigidActor* actor, OPmat4* mat);
i8 OPphysXOverlapping(OPphysXRigidActor* actor, OPphysXRigidActor* other);
i8 OPphysXOverlapping(OPphysXRigidActor* actor, PxGeometry* otherGeometry, PxTransform otherTransform);
i8 OPphysXOverlapping(OPphysXRigidActor* actor, PxShape* actorShape, OPphysXRigidActor* other, PxShape* otherShape);
void OPphysXShutdown();
PxTransform OPphysXMat4ToPx(OPmat4* mat);
void OPphysXShapeSetPose(OPphysXShape* shape, OPmat4 transform);

i8 OPphysXBoxColliding(PxRigidDynamic* actor, OPvec3 size, OPvec3 pos);
i8 OPphysXShapeBoxColliding(PxRigidDynamic* actor, OPphysXShape* shape, OPvec3 size, OPvec3 pos);
i8 OPphysXShapeBoxColliding(OPmat4 transform, OPphysXShape* shape, OPvec3 size, OPvec3 pos);

// INLINES

inline void OPphysXSetMass(OPphysXRigidDynamic* actor, OPfloat amount) {
	actor->setMass(amount);
}

inline void OPphysXAddForce(OPphysXRigidDynamic* dynamic, OPvec3 force) {
	dynamic->addForce(PxVec3(force.x, force.y, force.z));
}

inline void OPphysXAddTorque(OPphysXRigidDynamic* dynamic, OPvec3 torque) {
	dynamic->addTorque(PxVec3(torque.x, torque.y, torque.z));
}

inline void OPphysXSetLinearVelocity(OPphysXRigidDynamic* dynamic, OPvec3 velocity) {
	dynamic->setLinearVelocity(PxVec3(velocity.x, velocity.y, velocity.z));
}

inline void OPphysXSetAngularVelocity(OPphysXRigidDynamic* dynamic, OPvec3 velocity) {
	dynamic->setAngularVelocity(PxVec3(velocity.x, velocity.y, velocity.z));
}

inline void OPphysXSetGravity(OPphysXRigidActor* actor, i8 state) {
	actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !state);
}

inline void OPphysXSetSimulation(OPphysXShape* shape, i8 state) {
	shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, state);
}

inline void OPphysXSetTrigger(OPphysXShape* shape, i8 state) {
	shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, state);
}

inline void OPphysXSetSceneQuery(OPphysXShape* shape, i8 state) {
    shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, state);
}
inline OPphysXShape* OPphysXGetShape(OPphysXRigidActor* actor, OPuint index) {
	OPphysXShape* result;
	actor->getShapes(&result, 1, index);
	return result;
}

#endif


#endif
