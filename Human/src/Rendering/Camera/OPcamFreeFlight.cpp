#include "./Human/include/Rendering/Camera/OPcamFreeFlight.h"
#include "./Human/include/Input/OPkeyboard.h"
#include "./Human/include/Input/OPgamePad.h"
#include "./Human/include/Input/OPmouse.h"
#include "./Human/include/Rendering/OPrender.h"

void OPcamFreeFlightInit(OPcamFreeFlight* camFree, OPfloat moveSpeed, OPfloat rotateSpeed, OPvec3 position, OPfloat camNear, OPfloat camFar) {
	camFree->RotationSpeed = rotateSpeed;
	camFree->MoveSpeed = moveSpeed;
	camFree->Rotation = OPVEC3_ZERO;
	camFree->Movement = OPVEC3_ZERO;

	OPfloat angle = OPvec3Angle(OPVEC3_FORWARD, OPvec3Norm(position));
	camFree->Rotation.x = -angle;

	camFree->Camera = OPcamPersp(
		position,
		OPVEC3_ZERO,
		OPVEC3_UP,
		camNear, camFar,
		45.0f,
		OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
		);


	OPcamFreeFlightUpdate(camFree);
}

OPcamFreeFlight* OPcamFreeCreate(OPfloat moveSpeed, OPfloat rotateSpeed, OPvec3 position, OPfloat camNear, OPfloat camFar) {
	OPcamFreeFlight* result = (OPcamFreeFlight*)OPalloc(sizeof(OPcamFreeFlight));
	OPcamFreeFlightInit(result, moveSpeed, rotateSpeed, position, camNear, camFar);
	return result;
}

void OPcamFreeFlightUpdate(OPcamFreeFlight* camFree) {
	OPmat4 rotation = OPmat4RotY(camFree->Rotation.y) * OPmat4RotX(camFree->Rotation.x);
	OPvec3 target = OPmat4Transform(OPVEC3_BACKWARD, rotation);

	camFree->Camera.pos += OPmat4Transform(camFree->Movement, rotation);
	camFree->Camera.target = camFree->Camera.pos + target;
	camFree->Camera.Update();
}

void OPcamFreeFlightUpdate(OPcamFreeFlight* camFree, OPtimer* timer) {
	OPfloat dt = timer->Elapsed / 1000.0f;
	OPvec3 rot = { 0, 0, 0 };

	OPgamePad* gamePad = OPgamePadGet(OPGAMEPAD_ONE);
	OPfloat triggerDifference = 1.0f + gamePad->RightTrigger() - (gamePad->LeftTrigger() * 0.9f);

	OPfloat moveSpeed = dt * camFree->MoveSpeed * triggerDifference * 10.0f;
	OPfloat rotSpeed = dt * camFree->RotationSpeed * triggerDifference;

	camFree->Movement.z = OPkeyboardIsDown(OPKEY_S) - OPkeyboardIsDown(OPKEY_W) - gamePad->LeftThumbY();
	camFree->Movement.x = OPkeyboardIsDown(OPKEY_D) - OPkeyboardIsDown(OPKEY_A) + gamePad->LeftThumbX();
	camFree->Movement *= moveSpeed;

	rot.y = OPkeyboardIsDown(OPKEY_Q) - OPkeyboardIsDown(OPKEY_E) - gamePad->RightThumbX();
	rot.x = OPkeyboardIsDown(OPKEY_Z) - OPkeyboardIsDown(OPKEY_C) + gamePad->RightThumbY();
	if (OPmouseIsDown(OPMOUSE_RBUTTON)) {
		rot.x += OPmousePositionMovedY() / 10.0f;
		rot.y += OPmousePositionMovedX() / 10.0f;
	}
	rot *= rotSpeed;
	camFree->Rotation += rot;

	OPcamFreeFlightUpdate(camFree);
}

void OPcamFreeFlightDestroy() {

}
