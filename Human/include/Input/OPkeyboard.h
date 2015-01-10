#ifndef OPENGINE_HUMAN_INPUT_KEYBOARD
#define OPENGINE_HUMAN_INPUT_KEYBOARD

#include "./Human/include/Input/Enums/OPkeyboardKeys.h"
#include "./Core/include/OPtimer.h"

typedef struct {
	OPint	keys[_OPKEYBOARD_MAX];
	OPint	prevKeys[_OPKEYBOARD_MAX];
} OPkeyboardState;

extern IMPORT_METHOD OPkeyboardState Keyboard;

EXPORT_METHOD void OPkeyboardUpdate(OPtimer* timer);
EXPORT_METHOD void OPkeyboardUpdatePost(OPtimer* timer);
EXPORT_METHOD OPint OPkeyboardIsDown(OPkeyboardKey key);
EXPORT_METHOD OPint OPkeyboardIsUp(OPkeyboardKey key);
EXPORT_METHOD OPint OPkeyboardWasPressed(OPkeyboardKey key);
EXPORT_METHOD OPint OPkeyboardWasReleased(OPkeyboardKey key);
EXPORT_METHOD OPint OPkeyboardAnyInputIsDown();

#endif