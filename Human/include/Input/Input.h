#pragma once

#include "./Core/include/Core.h"
#include "InputEnums.h"
#include "./Math/include/Vector2.h"


typedef struct {
	bool	keys[MaxKeyboardKeys];
	bool	prevKeys[MaxKeyboardKeys];
} KeyboardState;

typedef struct {
	i32	wheel;
	i32	prevWheel;
	i32	positionX;
	i32	positionY;
	i32	prevPositionX;
	i32	prevPositionY;
	bool	keys[MaxMouseKeys];
	bool	prevKeys[MaxMouseKeys];
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
bool OPkeyboardIsDown(KeyboardKey key);
bool OPkeyboardIsUp(KeyboardKey key);
bool OPkeyboardWasPressed(KeyboardKey key);
bool OPkeyboardWasReleased(KeyboardKey key);

void OPmouseUpdate();
i32 OPmousePositionX();
i32 OPmousePositionY();
i32 OPmousePositionMovedX();
i32 OPmousePositionMovedY();
i32 OPmouseWheel();
i32 OPmouseWheelMoved();
bool OPmouseIsDown(MouseKey key);
bool OPmouseIsUp(MouseKey key);
bool OPmouseWasPressed(MouseKey key);
bool OPmouseWasReleased(MouseKey key);
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