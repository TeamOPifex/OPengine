#ifndef OPENGINE_HUMAN_INPUT_MOUSE
#define OPENGINE_HUMAN_INPUT_MOUSE

#include "./Human/include/Input/Enums/OPmouseKeys.h"

typedef struct {
	i32	wheel;
	i32	prevWheel;
	i32	positionX;
	i32	positionY;
	i32	prevPositionX;
	i32	prevPositionY;
	OPint	keys[_OPMOUSE_MAX];
	OPint	prevKeys[_OPMOUSE_MAX];
} OPmouseState;

extern OPmouseState Mouse;

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

#endif