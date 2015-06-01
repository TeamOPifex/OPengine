// #include "./Performance/include/OPphysics.h"


// #ifdef OPIFEX_OPTION_PHYSICS

// PxPhysics* gPhysicsSDK = NULL;
// static PxDefaultErrorCallback gDefaultErrorCallback;
// static PxDefaultAllocator gDefaultAllocatorCallback;
// static PxSimulationFilterShader gDefaultFilterShader = SampleSubmarineFilterShader;
// PxReal myTimestep = 1.0f / 60.0f;

// #endif

// void OPphysicsInit() {
// #ifdef OPIFEX_OPTION_PHYSICS
// 	PxFoundation* foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
// 	gPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale());
// 	// Might not need these yet
// 	PxInitExtensions(*gPhysicsSDK);
// #endif
// }





// #ifdef OPIFEX_OPTION_PHYSICS

// 	PxFilterFlags SampleSubmarineFilterShader(	
// 		PxFilterObjectAttributes attributes0, PxFilterData filterData0, 
// 		PxFilterObjectAttributes attributes1, PxFilterData filterData1,
// 		PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
// 	{
// 		// let triggers through
// 		if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
// 		{
// 			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
// 			return PxFilterFlag::eDEFAULT;
// 		}
// 		// generate contacts for all that were not filtered above
// 		pairFlags = PxPairFlag::eCONTACT_DEFAULT;

// 		// trigger the contact callback for pairs (A,B) where 
// 		// the filtermask of A contains the ID of B and vice versa.
// 		if((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
// 			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
		
// 		return PxFilterFlag::eDEFAULT;
// 	}

// 	PxPhysics* gPhysicsSDK = NULL;
// 	static PxDefaultErrorCallback gDefaultErrorCallback;
// 	static PxDefaultAllocator gDefaultAllocatorCallback;
// 	static PxSimulationFilterShader gDefaultFilterShader = SampleSubmarineFilterShader;
// 	PxReal myTimestep = 1.0f / 60.0f;


// #endif

// void OPphysicsInit() {
// #ifdef OPIFEX_OPTION_PHYSICS
// 	PxFoundation* foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
// 	gPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale());
// 	// Might not need these yet
// 	PxInitExtensions(*gPhysicsSDK);
// #endif
// }

// OPphysicsScene* OPphysicsCreateScene(void(*onTrigger)(OPphysicsTrigger)) {

// #ifdef OPIFEX_OPTION_PHYSICS
// 	OPphysicsScene* scene = (OPphysicsScene*)OPallocZero(sizeof(OPphysicsScene));

// 	PxSceneDesc sceneDesc(gPhysicsSDK->getTolerancesScale());
// 	if(onTrigger != NULL) {
// 		scene->onTrigger = new OPphysicsSimulationEventCallbackHandler(onTrigger);
// 		sceneDesc.simulationEventCallback = scene->onTrigger;
// 	}
// 	sceneDesc.gravity = PxVec3(0.0f, -9.8f * 5.0f, 0.0f);

// 	if (!sceneDesc.cpuDispatcher) {
// 		PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(2);
// 		sceneDesc.cpuDispatcher = mCpuDispatcher;
// 	}

// 	if (!sceneDesc.filterShader) {
// 		sceneDesc.filterShader = gDefaultFilterShader;
// 		scene->scene = gPhysicsSDK->createScene(sceneDesc);
// 	}

// 	scene->elapsed = 0;
	
// 	return scene;
// #else
// 	return NULL;
// #endif
// }

// void OPphysicsSetFiltering(OPphysicsActor* actor, PxU32 filterGroup, PxU32 filterMask)
// {
// 	PxFilterData filterData;
// 	filterData.word0 = filterGroup; // word0 = own ID
// 	filterData.word1 = filterMask;	// word1 = ID mask to filter pairs that trigger a contact callback;
// 	const PxU32 numShapes = actor->actor->getNbShapes();
// 	PxShape** shapes = (PxShape**)OPalloc(sizeof(PxShape*)*numShapes);
// 	actor->actor->getShapes(shapes, numShapes);
// 	for(PxU32 i = 0; i < numShapes; i++)
// 	{
// 		PxShape* shape = shapes[i];
// 		shape->setSimulationFilterData(filterData);
// 	}
// 	OPfree(shapes);
// }

// void OPphysicsGetTransform(OPphysicsActor* actor, OPmat4* mat)
// {
// #ifdef OPIFEX_OPTION_PHYSICS
// 	PxU32 n = actor->actor->getNbShapes();
// 	PxShape** shapes = new PxShape*[n];

// 	actor->actor->getShapes(shapes, n);

// 	PxTransform pT = PxShapeExt::getGlobalPose(*shapes[0], *actor->actor);

// 	PxMat33 m = PxMat33(pT.q);

// 	mat->cols[0].x = m.column0[0];
// 	mat->cols[0].y = m.column0[1];
// 	mat->cols[0].z = m.column0[2];
// 	mat->cols[0].w = 0;

// 	mat->cols[1].x = m.column1[0];
// 	mat->cols[1].y = m.column1[1];
// 	mat->cols[1].z = m.column1[2];
// 	mat->cols[1].w = 0;

// 	mat->cols[2].x = m.column2[0];
// 	mat->cols[2].y = m.column2[1];
// 	mat->cols[2].z = m.column2[2];
// 	mat->cols[2].w = 0;

// 	mat->cols[3].x = pT.p[0];
// 	mat->cols[3].y = pT.p[1];
// 	mat->cols[3].z = pT.p[2];
// 	mat->cols[3].w = 1;
// #endif
// }


// void OPphysicsAddForce(OPphysicsDynamic* dynamic, f32 x, f32 y, f32 z){
// #ifdef OPIFEX_OPTION_PHYSICS
// 	dynamic->actor->addForce(PxVec3(x, y, z));
// #endif
// }
// void OPphysicsAddTorque(OPphysicsDynamic* dynamic, f32 x, f32 y, f32 z){
// #ifdef OPIFEX_OPTION_PHYSICS
// 	dynamic->actor->addTorque(PxVec3(x, y, z));
// #endif
// }

// void OPphysicsSetLinearVelocity(OPphysicsDynamic* dynamic, f32 x, f32 y, f32 z){
// #ifdef OPIFEX_OPTION_PHYSICS
// 	dynamic->actor->setLinearVelocity(PxVec3(x, y, z));
// #endif
// }

// void OPphysicsSetAngularVelocity(OPphysicsDynamic* dynamic, f32 x, f32 y, f32 z){
// #ifdef OPIFEX_OPTION_PHYSICS
// 	dynamic->actor->setAngularVelocity(PxVec3(x, y, z));
// #endif
// }


// OPphysicsDynamic* OPphysicsCreateBoxDynamic(OPphysicsScene* scene, f32 x, f32 y, f32 z, f32 sx, f32 sy, f32 sz) {
// #ifdef OPIFEX_OPTION_PHYSICS
// 	PxTransform cubeTransform(PxVec3(x, y, z));

// 	PxMaterial* boxMaterial = gPhysicsSDK->createMaterial(0.6f, 0.8f, 0.5f);

// 	PxRigidDynamic* aSphereActor = gPhysicsSDK->createRigidDynamic(cubeTransform);

// 	PxShape* aSphereShape = aSphereActor->createShape(PxBoxGeometry(sx, sy, sz), *boxMaterial);
// 	aSphereActor->setMass(sx);

// 	//aSphereShape = aSphereActor->createShape(PxBoxGeometry(sx, sy, sz), *boxMaterial);
// 	// aSphereShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
// 	// aSphereShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

// 	((PxScene*)scene->scene)->addActor(*aSphereActor);

// 	OPphysicsDynamic* result = (OPphysicsDynamic*)OPalloc(sizeof(OPphysicsDynamic));
// 	result->actor = aSphereActor;
// 	return result;
// #else
// 	return NULL;
// #endif
// }

// OPphysicsDynamic* OPphysicsCreateSphereDynamic(OPphysicsScene* scene, f32 x, f32 y, f32 z, f32 s){
// #ifdef OPIFEX_OPTION_PHYSICS
// 	PxTransform cubeTransform(PxVec3(x, y, z));

// 	PxMaterial* boxMaterial = gPhysicsSDK->createMaterial(0.8f, 0.8f, 0.6f);

// 	PxRigidDynamic* aSphereActor = gPhysicsSDK->createRigidDynamic(cubeTransform);

// 	PxShape* aSphereShape = aSphereActor->createShape(PxSphereGeometry(s), *boxMaterial);
// 	aSphereActor->setMass(s);

// 	//aSphereShape = aSphereActor->createShape(PxSphereGeometry(s), *boxMaterial);
// 	// aSphereShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
// 	// aSphereShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

// 	((PxScene*)scene->scene)->addActor(*aSphereActor);


// 	OPphysicsDynamic* result = (OPphysicsDynamic*)OPalloc(sizeof(OPphysicsDynamic));
// 	result->actor = aSphereActor;
// 	return result;
// #else
// 	return NULL;
// #endif
// }

// OPphysicsStatic* OPphysicsCreateBoxStatic(OPphysicsScene* scene, f32 x, f32 y, f32 z, f32 sx, f32 sy, f32 sz) {
// #ifdef OPIFEX_OPTION_PHYSICS
// 	PxTransform cubeTransform(PxVec3(x, y, z));

// 	PxMaterial* boxMaterial = gPhysicsSDK->createMaterial(0.6f, 0.8f, 0.5f);

// 	PxRigidStatic* aSphereActor = gPhysicsSDK->createRigidStatic(cubeTransform);

// 	PxShape* aSphereShape = aSphereActor->createShape(PxBoxGeometry(sx, sy, sz), *boxMaterial);

// 	// aSphereShape = aSphereActor->createShape(PxBoxGeometry(sx, sy, sz), *boxMaterial);
// 	// aSphereShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
// 	// aSphereShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

// 	((PxScene*)scene->scene)->addActor(*aSphereActor);

// 	OPphysicsStatic* result = (OPphysicsStatic*)OPalloc(sizeof(OPphysicsStatic));
// 	result->actor = aSphereActor;
// 	return result;
// #else
// 	return NULL;
// #endif
// }

// OPphysicsStatic* OPphysicsCreateSphereStatic(OPphysicsScene* scene, f32 x, f32 y, f32 z, f32 s){
// #ifdef OPIFEX_OPTION_PHYSICS
// 	PxTransform cubeTransform(PxVec3(x, y, z));

// 	PxMaterial* boxMaterial = gPhysicsSDK->createMaterial(0.8f, 0.8f, 0.6f);

// 	PxRigidStatic* aSphereActor = gPhysicsSDK->createRigidStatic(cubeTransform);

// 	PxShape* aSphereShape = aSphereActor->createShape(PxSphereGeometry(s), *boxMaterial);

// 	//aSphereShape = aSphereActor->createShape(PxSphereGeometry(s), *boxMaterial);
// 	// aSphereShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
// 	// aSphereShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

// 	((PxScene*)scene->scene)->addActor(*aSphereActor);

// 	OPphysicsStatic* result = (OPphysicsStatic*)OPalloc(sizeof(OPphysicsStatic));
// 	result->actor = aSphereActor;
// 	return result;
// #else
// 	return NULL;
// #endif
// }

// OPphysicsStatic* OPphysicsCreatePlane(OPphysicsScene* scene) {
// #ifdef OPIFEX_OPTION_PHYSICS

// 	PxMaterial* planeMaterial = gPhysicsSDK->createMaterial(0.9f, 0.1f, 0.2f);
// 	PxTransform pose = PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
// 	PxRigidStatic* aPlaneActor = gPhysicsSDK->createRigidStatic(pose);
// 	PxShape* aPlaneShape = aPlaneActor->createShape(PxPlaneGeometry(), *planeMaterial);
// 	((PxScene*)scene->scene)->addActor(*aPlaneActor);

// 	OPphysicsStatic* result = (OPphysicsStatic*)OPalloc(sizeof(OPphysicsStatic));
// 	result->actor = aPlaneActor;
// 	return result;
// #else
// 	return NULL;
// #endif
// }

// void OPphysicsRemoveDynamic(OPphysicsScene* scene, OPphysicsDynamic* actor) {
// #ifdef OPIFEX_OPTION_PHYSICS
// 	((PxScene*)scene->scene)->removeActor(*actor->actor);
// #endif
// }

// void OPphysicsRemoveStatic(OPphysicsScene* scene, OPphysicsStatic* actor) {
// #ifdef OPIFEX_OPTION_PHYSICS
// 	((PxScene*)scene->scene)->removeActor(*actor->actor);
// #endif
// }

// void OPphysicsStep(OPphysicsScene* scene, ui64 elapsed) {
// #ifdef OPIFEX_OPTION_PHYSICS
// 	scene->elapsed += elapsed;
// 	if (scene->elapsed > (1000 * myTimestep)) {
// 		scene->elapsed -= 1000 * myTimestep;
// 		((PxScene*)scene->scene)->simulate(myTimestep);
// 		((PxScene*)scene->scene)->fetchResults(true);
// 	}

// #endif
// }

// void OPphysicsDestroy(OPphysicsScene* scene)
// {
// #ifdef OPIFEX_OPTION_PHYSICS
// 	((PxScene*)scene->scene)->release();
// #endif
// }

// void OPphysicsShutdown() {
// #ifdef OPIFEX_OPTION_PHYSICS
// 	gPhysicsSDK->release();
// #endif
// }