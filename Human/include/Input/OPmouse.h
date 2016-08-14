#pragma once

struct OPmouseState;
typedef struct OPmouseState OPmouseState;

#include "./Human/include/Input/Enums/OPmouseButtons.h"
#include "./Core/include/OPtypes.h"

struct OPmouseState {
	i32	updatedWheel;
	i32	wheel;
	i32	prevWheel;
	d64	positionX;
	d64	positionY;
	d64	prevPositionX;
	d64	prevPositionY;
	bool	keys[(ui32)OPmouseButton::_MAX];
	bool	prevKeys[(ui32)OPmouseButton::_MAX];
};

extern OPmouseState Mouse;

void OPmouseUpdate();
i32 OPmousePositionX();
i32 OPmousePositionY();
i32 OPmousePositionMovedX();
i32 OPmousePositionMovedY();
i32 OPmouseWheel();
i32 OPmouseWheelMoved();
bool OPmouseIsDown(OPmouseButton key);
bool OPmouseIsUp(OPmouseButton key);
bool OPmouseWasPressed(OPmouseButton key);
bool OPmouseWasReleased(OPmouseButton key);
void OPmouseSetPosition(i32 x, i32 y);
void OPmouseSetPositionScreenCenter();
bool OPmouseAnyInputIsDown();