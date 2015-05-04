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

void OPcamFreeFlightInit(OPcamFreeFlight* camFree, OPfloat moveSpeed, OPfloat rotateSpeed, OPvec3 position, OPfloat camNear, OPfloat camFar);
OPcamFreeFlight* OPcamFreeCreate(OPfloat moveSpeed, OPfloat rotateSpeed, OPvec3 position, OPfloat camNear, OPfloat camFar);
void OPcamFreeFlightUpdate(OPcamFreeFlight* camFree, OPtimer* timer);
void OPcamFreeFlightUpdate(OPcamFreeFlight* camFree);
void OPcamFreeFlightDestroy();

inline void OPcamFreeFlightInit(OPcamFreeFlight* camFree, OPfloat moveSpeed, OPfloat rotateSpeed, OPvec3 position) {
	OPcamFreeFlightInit(camFree, moveSpeed, rotateSpeed, position, 1.0f, 1000.0f);
}
inline OPcamFreeFlight* OPcamFreeCreate(OPfloat moveSpeed, OPfloat rotateSpeed, OPvec3 position) {
	return OPcamFreeCreate(moveSpeed, rotateSpeed, position, 1.0f, 1000.0f);
}
#endif