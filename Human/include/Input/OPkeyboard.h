#ifndef OPENGINE_HUMAN_INPUT_KEYBOARD
#define OPENGINE_HUMAN_INPUT_KEYBOARD

#include "./Human/include/Input/Enums/OPkeyboardKeys.h"
#include "./Core/include/OPtimer.h"

typedef struct {
	OPint	keys[OPKEYBOARD_MAX];
	OPint	prevKeys[OPKEYBOARD_MAX];
} OPkeyboardState;

extern OPkeyboardState Keyboard;

void OPkeyboardUpdate(OPtimer* timer);
void OPkeyboardUpdatePost(OPtimer* timer);
OPint OPkeyboardIsDown(OPkeyboardKeys key);
OPint OPkeyboardIsUp(OPkeyboardKeys key);
OPint OPkeyboardWasPressed(OPkeyboardKeys key);
OPint OPkeyboardWasReleased(OPkeyboardKeys key);
OPint OPkeyboardAnyInputIsDown();

#endif