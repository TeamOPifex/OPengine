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

extern IMPORT_METHOD OPmouseState Mouse;

EXPORT_METHOD void OPmouseUpdate();
EXPORT_METHOD i32 OPmousePositionX();
EXPORT_METHOD i32 OPmousePositionY();
EXPORT_METHOD i32 OPmousePositionMovedX();
EXPORT_METHOD i32 OPmousePositionMovedY();
EXPORT_METHOD i32 OPmouseWheel();
EXPORT_METHOD i32 OPmouseWheelMoved();
EXPORT_METHOD OPint OPmouseIsDown(OPmouseKey key);
EXPORT_METHOD OPint OPmouseIsUp(OPmouseKey key);
EXPORT_METHOD OPint OPmouseWasPressed(OPmouseKey key);
EXPORT_METHOD OPint OPmouseWasReleased(OPmouseKey key);
EXPORT_METHOD void OPmouseSetPosition(i32 x, i32 y);
EXPORT_METHOD void OPmouseSetPositionScreenCenter();
EXPORT_METHOD OPint OPmouseAnyInputIsDown();

#endif