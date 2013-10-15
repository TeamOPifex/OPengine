#pragma once

#include "InputEnums.h"
#include "./Math/include/Vector2.h"


typedef struct {
	bool	keys[MaxKeyboardKeys];
	bool	prevKeys[MaxKeyboardKeys];
} KeyboardState;

typedef struct {
	bool	keys[MaxMouseKeys];
	bool	prevKeys[MaxMouseKeys];
	i32	wheel;
	i32	prevWheel;
	i32	positionX;
	i32	positionY;
	i32	prevPositionX;
	i32	prevPositionY;
} MouseState;

extern KeyboardState Keyboard;
extern MouseState Mouse;

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