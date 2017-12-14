#include "./Human/include/Rendering/Camera/OPcamFreeFlight.h"
#include "./Human/include/Input/OPkeyboard.h"
#include "./Human/include/Input/OPgamePadSystem.h"
#include "./Human/include/Input/OPmouse.h"
#include "./Human/include/Rendering/OPrender.h"


#ifdef ADDON_socketio
#include "OPsocketGamePadSystem.h"
#endif

void OPcamFreeFlight::Init(OPfloat moveSpeed, OPfloat rotateSpeed, OPvec3 position, OPfloat camNear, OPfloat camFar) {
	RotationSpeed = rotateSpeed;
	MoveSpeed = moveSpeed;
	Rotation = OPVEC3_ZERO;
	Movement = OPVEC3_ZERO;

	OPfloat angle = OPvec3Angle(OPVEC3_FORWARD, OPvec3Norm(position));
	Rotation.x = -angle;
	Rotation.y = OPvec3Angle(OPVEC3_LEFT, OPvec3Norm(position));
	Rotation.z = OPvec3Angle(OPVEC3_UP, OPvec3Norm(position));

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
	OPvec3 rot = OPvec3( 0, 0, 0 );

	OPgamePad* gamePad = OPGAMEPADS[0];
	OPfloat triggerDifference = 1.0f + gamePad->RightTrigger() - (gamePad->LeftTrigger() * 0.9f);

	OPfloat moveSpeed = dt * MoveSpeed * triggerDifference * 10.0f;
	OPfloat rotSpeed = dt * RotationSpeed * triggerDifference;
	
	Movement.z = OPKEYBOARD.IsDown(OPkeyboardKey::S) - OPKEYBOARD.IsDown(OPkeyboardKey::W) - gamePad->LeftThumbY();
	Movement.x = OPKEYBOARD.IsDown(OPkeyboardKey::D) - OPKEYBOARD.IsDown(OPkeyboardKey::A) + gamePad->LeftThumbX();
	Movement *= moveSpeed;

	rot.y = OPKEYBOARD.IsDown(OPkeyboardKey::Q) - OPKEYBOARD.IsDown(OPkeyboardKey::E) - gamePad->RightThumbX();
	rot.x = OPKEYBOARD.IsDown(OPkeyboardKey::Z) - OPKEYBOARD.IsDown(OPkeyboardKey::C) + gamePad->RightThumbY();

#ifdef ADDON_socketio
	Movement.z -= OPSOCKETGAMEPADS[0]->LeftThumbY();
	Movement.x += OPSOCKETGAMEPADS[0]->LeftThumbX();
	rot.y -= OPSOCKETGAMEPADS[0]->RightThumbX();
	rot.x += OPSOCKETGAMEPADS[0]->RightThumbY();
#endif
	if (OPMOUSE.IsDown(OPmouseButton::RBUTTON)) {
		rot.x += OPMOUSE.MovedY() / 10.0f;
		rot.y += OPMOUSE.MovedX() / 10.0f;
	}
	rot *= rotSpeed;
	Rotation += rot;
	//OPmouseButton
	Update();
}

void OPcamFreeFlight::Destroy() {

}
