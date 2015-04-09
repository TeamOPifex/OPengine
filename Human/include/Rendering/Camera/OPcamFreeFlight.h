#ifndef OPENGINE_HUMAN_RENDERER_CAMERA_FREE
#define OPENGINE_HUMAN_RENDERER_CAMERA_FREE

#include "./Human/include/Rendering/OPcam.h"
#include "./Core/include/OPtimer.h"

// Free Flight Camera
struct OPcamFreeFlight {
	OPcam Camera;
	OPvec3 Rotation, Movement;
	OPvec3 Position, Target;
	OPfloat RotationSpeed, MoveSpeed;
};

void OPcamFreeFlightInit(OPcamFreeFlight* camFree, OPfloat speed);
OPcamFreeFlight* OPcamFreeCreate(OPfloat speed);
void OPcamFreeFlightUpdate(OPcamFreeFlight* camFree, OPtimer* timer);
void OPcamFreeFlightUpdate(OPcamFreeFlight* camFree);
void OPcamFreeFlightDestroy();

#endif