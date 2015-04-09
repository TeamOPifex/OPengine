#ifndef OPENGINE_HUMAN_RENDERER_CAMERA_FREE
#define OPENGINE_HUMAN_RENDERER_CAMERA_FREE

#include "./Human/include/Rendering/OPcam.h"
#include "./Core/include/OPtimer.h"

// Free Flight Camera
struct OPcamFreeFlight {
	OPcam Camera;
	OPvec3 Rotation, Movement;
	OPfloat RotationSpeed, MoveSpeed;
};

void OPcamFreeFlightInit(OPcamFreeFlight* camFree, OPfloat moveSpeed, OPfloat rotateSpeed, OPvec3 position);
OPcamFreeFlight* OPcamFreeCreate(OPfloat moveSpeed, OPfloat rotateSpeed, OPvec3 position);
void OPcamFreeFlightUpdate(OPcamFreeFlight* camFree, OPtimer* timer);
void OPcamFreeFlightUpdate(OPcamFreeFlight* camFree);
void OPcamFreeFlightDestroy();

#endif