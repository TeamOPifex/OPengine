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

	return manager->createController(desc);
}
OPphysXController* OPphysXControllerCreate(OPphysXControllerManager* manager, OPphysXMaterial* material, OPfloat height, OPfloat radius) {
	return OPphysXControllerCreate(manager, material, height, radius, NULL, NULL, NULL);
}

void OPphysXControllerMove(OPphysXController* controller, OPvec3 disp, OPtimer* timer) {
	PxControllerFilters filters = PxControllerFilters(0);
	controller->move(PxVec3(disp.x, disp.y, disp.z), 0.01f, timer->Elapsed, filters);
}

OPvec3 OPphysXControllerGetPos(OPphysXController* controller) {
	PxExtendedVec3 pos = controller->getPosition();
	return OPvec3Create(pos.x, pos.y, pos.z);
}

#endif