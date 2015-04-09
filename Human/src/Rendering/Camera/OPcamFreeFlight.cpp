#include "./Human/include/Rendering/Camera/OPcamFreeFlight.h"
#include "./Human/include/Input/OPkeyboard.h"
#include "./Human/include/Input/OPgamePad.h"
#include "./Human/include/Rendering/OPrender.h"

void OPcamFreeFlightInit(OPcamFreeFlight* camFree, OPfloat speed) {
	camFree->RotationSpeed = 3.0f;
	camFree->MoveSpeed = 3.0f;
	camFree->Position = OPVEC3_ONE;
	camFree->Target = OPVEC3_BACKWARD;
	camFree->Rotation = OPVEC3_ZERO;
	camFree->Movement = OPVEC3_ZERO;

	camFree->Camera = OPcamPersp(
		camFree->Position,
		camFree->Target,
		OPVEC3_UP,
		0.1f,
		1000.0f,
		45.0f,
		OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
		);
}

OPcamFreeFlight* OPcamFreeCreate(OPfloat speed) {
	OPcamFreeFlight* result = (OPcamFreeFlight*)OPalloc(sizeof(OPcamFreeFlight));
	OPcamFreeFlightInit(result, speed);
	return result;
}

void OPcamFreeFlightUpdate(OPcamFreeFlight* camFree) {
	OPmat4 rotation = OPmat4RotY(camFree->Rotation.y) * OPmat4RotX(camFree->Rotation.x);
	OPvec3 target = OPmat4Transform(OPVEC3_BACKWARD, rotation);
	camFree->Position += OPmat4Transform(camFree->Movement, rotation);

	camFree->Camera._pos = camFree->Position;
	camFree->Camera._targ = camFree->Position + target;
	camFree->Camera._viewStale = 1;
}

void OPcamFreeFlightUpdate(OPcamFreeFlight* camFree, OPtimer* timer) {
	OPfloat dt = timer->Elapsed / 1000.0f;
	OPvec3 rot = { 0, 0, 0 };

	OPfloat moveSpeed = dt * camFree->MoveSpeed * (1.0f + OPgamePadRightTrigger(OPgamePadGet(OPGAMEPAD_ONE)) * camFree->MoveSpeed);
	OPfloat rotSpeed = dt * camFree->RotationSpeed * (1.0f + OPgamePadRightTrigger(OPgamePadGet(OPGAMEPAD_ONE)));
	
	camFree->Movement.z = OPkeyboardIsDown(OPKEY_S) - OPkeyboardIsDown(OPKEY_W) - OPgamePadLeftThumbY(OPgamePadGet(OPGAMEPAD_ONE));
	camFree->Movement.x = OPkeyboardIsDown(OPKEY_E) - OPkeyboardIsDown(OPKEY_Q) + OPgamePadLeftThumbX(OPgamePadGet(OPGAMEPAD_ONE));
	camFree->Movement *= moveSpeed;

	rot.y = OPkeyboardIsDown(OPKEY_A) - OPkeyboardIsDown(OPKEY_D) - OPgamePadRightThumbX(OPgamePadGet(OPGAMEPAD_ONE));
	rot.x = OPkeyboardIsDown(OPKEY_Z) - OPkeyboardIsDown(OPKEY_C) + OPgamePadRightThumbY(OPgamePadGet(OPGAMEPAD_ONE));
	rot *= rotSpeed;
	camFree->Rotation += rot;

	OPcamFreeFlightUpdate(camFree);
}

void OPcamFreeFlightDestroy() {

}
