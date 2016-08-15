#pragma once

#include "./Core/include/OPtypes.h"

#ifdef OPIFEX_OPTION_PHYSX

#include "./Performance/include/OPphysXScene.h"

typedef PxControllerManager OPphysXControllerManager;
typedef PxController OPphysXController;

// FUNCTIONS

OPphysXControllerManager* OPphysXControllerCreateManager(OPphysXScene* scene);
OPphysXController* OPphysXControllerCreate(OPphysXControllerManager* manager, OPphysXMaterial* material, OPfloat height, OPfloat radius, void(*onShapeHit)(PxControllerShapeHit), void(*onControllerHit)(PxControllersHit), void(*onObstacleHit)(PxControllerObstacleHit));
OPphysXController* OPphysXControllerCreate(OPphysXControllerManager* manager, OPphysXMaterial* material, OPfloat height, OPfloat radius);
void OPphysXControllerMove(OPphysXController* controller, OPvec3 displacement, OPtimer* timer);
OPvec3 OPphysXControllerGetPos(OPphysXController* controller);
void OPphysXControllerSetPos(OPphysXController* controller, OPvec3 disp);
OPvec3 OPphysXControllerGetFootPos(OPphysXController* controller);
void OPphysXControllerSetFootPos(OPphysXController* controller, OPvec3 disp);
void OPphysXControllerRelease(OPphysXController* controller);
// INLINES


#endif