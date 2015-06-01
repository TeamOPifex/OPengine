#include "./Performance/include/OPphysX.h"
#include "./Performance/include/OPphysXBasicFilterShader.h"
#include "./Core/include/OPmemory.h"

#ifdef OPIFEX_OPTION_PHYSX

PxPhysics* OPphysXSDK = NULL;
static PxDefaultErrorCallback _defaultErrorCallback;
static PxDefaultAllocator _defaultAllocatorCallback;
PxSimulationFilterShader OPphysXDefaultFilterShader = OPphysXBasicFilterShader;

void OPphysXInit() {
	PxFoundation* foundation = PxCreateFoundation(PX_PHYSICS_VERSION, _defaultAllocatorCallback, _defaultErrorCallback);
	OPphysXSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale());
	PxInitExtensions(*OPphysXSDK);
}

void OPphysXSetFilter(OPphysXRigidActor* actor, ui32 filterGroup, ui32 filterMask) {
	PxFilterData filterData;
	filterData.word0 = filterGroup; // word0 = own ID
	filterData.word1 = filterMask;	// word1 = ID mask to filter pairs that trigger a contact callback;
	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)OPalloc(sizeof(PxShape*)*numShapes);
	actor->getShapes(shapes, numShapes);
	for(PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}
	OPfree(shapes);
}

OPphysXMaterial* OPphysXCreateMaterial(f32 staticFriction, f32 dynamicFriction, f32 restitution) {
	OPphysXMaterial* material = OPphysXSDK->createMaterial(staticFriction, dynamicFriction, restitution);
	return material;
}

OPphysXShape* OPphysXAddSphereShape(OPphysXRigidActor* actor, OPphysXMaterial* material, OPfloat size) {
	PxShape* sphereShape = actor->createShape(PxSphereGeometry(size), *material);
	return sphereShape;
}

OPphysXShape* OPphysXAddBoxShape(OPphysXRigidActor* actor, OPphysXMaterial* material, OPvec3 size) {
	PxShape* boxShape = actor->createShape(PxBoxGeometry(size.x, size.y, size.z), *material);
	return boxShape;
}

OPphysXShape* OPphysXAddPlaneShape(OPphysXRigidActor* actor, OPphysXMaterial* material) {
// 	PxTransform pose = PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	PxShape* planeShape = actor->createShape(PxPlaneGeometry(), *material);
	return planeShape;
}

void OPphysXGetTransform(OPphysXRigidActor* actor, OPmat4* mat) {
	ui32 n = actor->getNbShapes();
	PxShape** shapes = new PxShape*[n];

	actor->getShapes(shapes, n);

	PxTransform pT = PxShapeExt::getGlobalPose(*shapes[0], *actor);

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

	OPfree(shapes);
}

void OPphysXShutdown() {
	OPphysXSDK->release();
}
#endif