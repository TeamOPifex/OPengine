#include "./Human/include/Rendering/Camera/OPcamFreeFlight.h"
#include "./Human/include/Input/OPkeyboard.h"
#include "./Human/include/Input/OPgamePad.h"
#include "./Human/include/Input/OPmouse.h"
#include "./Human/include/Rendering/OPrender.h"

void OPcamFreeFlight::Init(OPfloat moveSpeed, OPfloat rotateSpeed, OPvec3 position, OPfloat camNear, OPfloat camFar) {
	RotationSpeed = rotateSpeed;
	MoveSpeed = moveSpeed;
	Rotation = OPVEC3_ZERO;
	Movement = OPVEC3_ZERO;

	OPfloat angle = OPvec3Angle(OPVEC3_FORWARD, OPvec3Norm(position));
	Rotation.x = -angle;

	Camera.SetPerspective(
		position,
		OPVEC3_ZERO,
		OPVEC3_UP,
		camNear, camFar,
		45.0f,
		OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (f32)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height
		);


	Update();
}

void OPcamFreeFlight::Update() {
	OPmat4 rotation = OPmat4RotY(Rotation.y) * OPmat4RotX(Rotation.x);
	OPvec3 target = OPmat4Transform(OPVEC3_BACKWARD, rotation);

	Camera.pos += OPmat4Transform(Movement, rotation);
	Camera.target = Camera.pos + target;
	Camera.Update();
}

void OPcamFreeFlight::Update(OPtimer* timer) {
	OPfloat dt = timer->Elapsed / 1000.0f;
	OPvec3 rot = { 0, 0, 0 };

	OPgamePad* gamePad = OPgamePadGet(OPgamePadIndex::ONE);
	OPfloat triggerDifference = 1.0f + gamePad->RightTrigger() - (gamePad->LeftTrigger() * 0.9f);

	OPfloat moveSpeed = dt * MoveSpeed * triggerDifference * 10.0f;
	OPfloat rotSpeed = dt * RotationSpeed * triggerDifference;
	
	Movement.z = OPkeyboardIsDown(OPkeyboardKey::S) - OPkeyboardIsDown(OPkeyboardKey::W) - gamePad->LeftThumbY();
	Movement.x = OPkeyboardIsDown(OPkeyboardKey::D) - OPkeyboardIsDown(OPkeyboardKey::A) + gamePad->LeftThumbX();
	Movement *= moveSpeed;

	rot.y = OPkeyboardIsDown(OPkeyboardKey::Q) - OPkeyboardIsDown(OPkeyboardKey::E) - gamePad->RightThumbX();
	rot.x = OPkeyboardIsDown(OPkeyboardKey::Z) - OPkeyboardIsDown(OPkeyboardKey::C) + gamePad->RightThumbY();
	if (OPmouseIsDown(OPmouseButton::RBUTTON)) {
		rot.x += OPmousePositionMovedY() / 10.0f;
		rot.y += OPmousePositionMovedX() / 10.0f;
	}
	rot *= rotSpeed;
	Rotation += rot;
	OPmouseButton
	Update();
}

void OPcamFreeFlight::Destroy() {

}
