#ifndef OPENGINE_HUMAN_INPUT
#define OPENGINE_HUMAN_INPUT

#include "./Core/include/Core.h"
#include "InputEnums.h"
#include "./Math/include/Vector2.h"

typedef struct {
	OPint	keys[OPKEYBOARD_MAX];
	OPint	prevKeys[OPKEYBOARD_MAX];
} KeyboardState;

typedef struct {
	i32	wheel;
	i32	prevWheel;
	i32	positionX;
	i32	positionY;
	i32	prevPositionX;
	i32	prevPositionY;
	OPint	keys[OPMOUSE_MAX];
	OPint	prevKeys[OPMOUSE_MAX];
} MouseState;

typedef struct {
	OPint tapping;
	OPint prevTapping;
	OPint updated;
	f32	positionX;
	f32	positionY;
	f32	prevPositionX;
	f32	prevPositionY;
} TouchState;

extern KeyboardState Keyboard;
extern MouseState Mouse;
extern TouchState Touch;

void OPkeyboardUpdate();
OPint OPkeyboardIsDown(OPkeyboardKey key);
OPint OPkeyboardIsUp(OPkeyboardKey key);
OPint OPkeyboardWasPressed(OPkeyboardKey key);
OPint OPkeyboardWasReleased(OPkeyboardKey key);
OPint OPkeyboardAnyInputIsDown();

void OPmouseUpdate();
i32 OPmousePositionX();
i32 OPmousePositionY();
i32 OPmousePositionMovedX();
i32 OPmousePositionMovedY();
i32 OPmouseWheel();
i32 OPmouseWheelMoved();
OPint OPmouseIsDown(OPmouseKey key);
OPint OPmouseIsUp(OPmouseKey key);
OPint OPmouseWasPressed(OPmouseKey key);
OPint OPmouseWasReleased(OPmouseKey key);
void OPmouseSetPosition(i32 x, i32 y);
void OPmouseSetPositionScreenCenter();
OPint OPmouseAnyInputIsDown();

void OPtouchUpdate();
f32 OPtouchPositionX();
f32 OPtouchPositionY();
f32 OPtouchPositionMovedX();
f32 OPtouchPositionMovedY();
OPint OPtouchIsDown();
OPint OPtouchIsUp();
OPint OPtouchWasPressed();
OPint OPtouchWasReleased();
OPint OPtouchAnyInputIsDown();

#endif