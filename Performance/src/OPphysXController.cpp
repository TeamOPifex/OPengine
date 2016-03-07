#include "./Performance/include/OPphysXController.h"
#include "./Performance/include/OPphysXControllerHitReport.h"

#ifdef OPIFEX_OPTION_PHYSX

OPphysXControllerManager* OPphysXControllerCreateManager(OPphysXScene* scene) {
	return PxCreateControllerManager(*scene->scene);
}

OPphysXController* OPphysXControllerCreate(OPphysXControllerManager* manager, OPphysXMaterial* material, OPfloat height, OPfloat radius,
		void(*onShapeHit)(PxControllerShapeHit),
		void(*onControllerHit)(PxControllersHit),
		void(*onObstacleHit)(PxControllerObstacleHit)) {
	PxCapsuleControllerDesc desc = PxCapsuleControllerDesc();
	desc.height = height;
	desc.radius = radius;
	desc.material = material;
	desc.reportCallback = new OPphysXControllerHitReport(onShapeHit, onControllerHit, onObstacleHit);

	OPphysXController* result = manager->createController(desc);
	result->setContactOffset(0.02f);
	OPuint shapesCount = result->getActor()->getNbShapes();
	OPlog("# of shapes: %d", shapesCount);
	PxShape* shapes = NULL;
	result->getActor()->getShapes(&shapes, shapesCount);
	shapes->setName("Generic Controller");
	shapes->setContactOffset(0.02f);

	result->getActor()->setGlobalPose(PxTransform(PxVec3(0,0,OPpi / 2.0f)));
	return result;
}
OPphysXController* OPphysXControllerCreate(OPphysXControllerManager* manager, OPphysXMaterial* material, OPfloat height, OPfloat radius) {
	return OPphysXControllerCreate(manager, material, height, radius, NULL, NULL, NULL);
}

void OPphysXControllerMove(OPphysXController* controller, OPvec3 disp, OPtimer* timer) {
	PxControllerFilters filters = PxControllerFilters(0);
	controller->move(PxVec3(disp.x, disp.y, disp.z), 0.01f, timer->Elapsed, filters);
}

void OPphysXControllerSetFootPos(OPphysXController* controller, OPvec3 disp) {
	controller->setFootPosition(PxExtendedVec3(disp.x, disp.y, disp.z));
}

void OPphysXControllerSetPos(OPphysXController* controller, OPvec3 disp) {
	controller->setPosition(PxExtendedVec3(disp.x, disp.y, disp.z));
}

OPvec3 OPphysXControllerGetPos(OPphysXController* controller) {
	PxExtendedVec3 pos = controller->getPosition();
	return OPvec3Create(pos.x, pos.y, pos.z);
}

OPvec3 OPphysXControllerGetFootPos(OPphysXController* controller) {
	PxExtendedVec3 pos = controller->getFootPosition();
	return OPvec3Create(pos.x, pos.y, pos.z);
}

void OPphysXControllerRelease(OPphysXController* controller) {
	controller->release();
}
#endif
