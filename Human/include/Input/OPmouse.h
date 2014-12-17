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
	OPint	keys[OPMOUSE_MAX];
	OPint	prevKeys[OPMOUSE_MAX];
} OPmouseState;

extern OPmouseState Mouse;

void OPmouseUpdate();
i32 OPmousePositionX();
i32 OPmousePositionY();
i32 OPmousePositionMovedX();
i32 OPmousePositionMovedY();
i32 OPmouseWheel();
i32 OPmouseWheelMoved();
OPint OPmouseIsDown(OPmouseKeys key);
OPint OPmouseIsUp(OPmouseKeys key);
OPint OPmouseWasPressed(OPmouseKeys key);
OPint OPmouseWasReleased(OPmouseKeys key);
void OPmouseSetPosition(i32 x, i32 y);
void OPmouseSetPositionScreenCenter();
OPint OPmouseAnyInputIsDown();

#endif