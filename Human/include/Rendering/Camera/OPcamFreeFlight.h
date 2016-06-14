#ifndef OPENGINE_HUMAN_RENDERER_CAMERA_FREE
#define OPENGINE_HUMAN_RENDERER_CAMERA_FREE

#include "./Human/include/Rendering/OPcam.h"
#include "./Core/include/OPtimer.h"

struct OPcamFreeFlight;

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

// Free Flight Camera
struct OPcamFreeFlight {
	OPcam Camera;
	OPvec3 Rotation, Movement;
	OPfloat RotationSpeed, MoveSpeed;

	void Init() {
		OPcamFreeFlightInit(this, 1.0, 1.0, OPvec3Create(0, 5, 5));
	}

	void Update(OPtimer* timer) {
		OPcamFreeFlightUpdate(this, timer);
	}

	OPmat4* View() { return &Camera.view; }
	OPmat4* Proj() { return &Camera.proj; }
};
#endif