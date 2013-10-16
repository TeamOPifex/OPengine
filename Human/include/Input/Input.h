#pragma once

#include "./Core/include/Core.h"
#include "InputEnums.h"
#include "./Math/include/Vector2.h"


typedef struct {
	bool	keys[OPKEYBOARD_MAX];
	bool	prevKeys[OPKEYBOARD_MAX];
} KeyboardState;

typedef struct {
	i32	wheel;
	i32	prevWheel;
	i32	positionX;
	i32	positionY;
	i32	prevPositionX;
	i32	prevPositionY;
	bool	keys[OPMOUSE_MAX];
	bool	prevKeys[OPMOUSE_MAX];
} MouseState;

typedef struct {
	bool tapping;
	bool prevTapping;
	bool updated;
	f32	positionX;
	f32	positionY;
	f32	prevPositionX;
	f32	prevPositionY;
} TouchState;

extern KeyboardState Keyboard;
extern MouseState Mouse;
extern TouchState Touch;

void OPkeyboardUpdate();
bool OPkeyboardIsDown(OPkeyboardKey key);
bool OPkeyboardIsUp(OPkeyboardKey key);
bool OPkeyboardWasPressed(OPkeyboardKey key);
bool OPkeyboardWasReleased(OPkeyboardKey key);

void OPmouseUpdate();
i32 OPmousePositionX();
i32 OPmousePositionY();
i32 OPmousePositionMovedX();
i32 OPmousePositionMovedY();
i32 OPmouseWheel();
i32 OPmouseWheelMoved();
bool OPmouseIsDown(OPmouseKey key);
bool OPmouseIsUp(OPmouseKey key);
bool OPmouseWasPressed(OPmouseKey key);
bool OPmouseWasReleased(OPmouseKey key);
void OPmouseSetPosition(i32 x, i32 y);
void OPmouseSetPositionScreenCenter();

void OPtouchUpdate();
f32 OPtouchPositionX();
f32 OPtouchPositionY();
f32 OPtouchPositionMovedX();
f32 OPtouchPositionMovedY();
bool OPtouchIsDown();
bool OPtouchIsUp();
bool OPtouchWasPressed();
bool OPtouchWasReleased();