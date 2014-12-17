#ifndef OPENGINE_HUMAN_INPUT_TOUCH
#define OPENGINE_HUMAN_INPUT_TOUCH

#include "./Core/include/OPtypes.h"

typedef struct {
	OPint tapping;
	OPint prevTapping;
	OPint updated;
	f32	positionX;
	f32	positionY;
	f32	prevPositionX;
	f32	prevPositionY;
} OPtouchState;

extern OPtouchState Touch;

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