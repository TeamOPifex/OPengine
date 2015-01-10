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

EXPORT_METHOD void OPtouchUpdate();
EXPORT_METHOD f32 OPtouchPositionX();
EXPORT_METHOD f32 OPtouchPositionY();
EXPORT_METHOD f32 OPtouchPositionMovedX();
EXPORT_METHOD f32 OPtouchPositionMovedY();
EXPORT_METHOD OPint OPtouchIsDown();
EXPORT_METHOD OPint OPtouchIsUp();
EXPORT_METHOD OPint OPtouchWasPressed();
EXPORT_METHOD OPint OPtouchWasReleased();
EXPORT_METHOD OPint OPtouchAnyInputIsDown();

#endif