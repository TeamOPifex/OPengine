#ifndef OPENGINE_HUMAN_INPUT_MOUSE
#define OPENGINE_HUMAN_INPUT_MOUSE

#include "./Human/include/Input/Enums/OPmouseKeys.h"

typedef struct {
	i32	wheel;
	i32	prevWheel;
	d64	positionX;
	d64	positionY;
	d64	prevPositionX;
	d64	prevPositionY;
	OPint	keys[_OPMOUSE_MAX];
	OPint	prevKeys[_OPMOUSE_MAX];
} OPmouseState;

extern OPmouseState Mouse;

void OPmouseUpdate();
d64 OPmousePositionX();
d64 OPmousePositionY();
d64 OPmousePositionMovedX();
d64 OPmousePositionMovedY();
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