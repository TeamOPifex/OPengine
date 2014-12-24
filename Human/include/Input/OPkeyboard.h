#ifndef OPENGINE_HUMAN_INPUT_KEYBOARD
#define OPENGINE_HUMAN_INPUT_KEYBOARD

#include "./Human/include/Input/Enums/OPkeyboardKeys.h"
#include "./Core/include/OPtimer.h"

typedef struct {
	OPint	keys[_OPKEYBOARD_MAX];
	OPint	prevKeys[_OPKEYBOARD_MAX];
} OPkeyboardState;

extern OPkeyboardState Keyboard;

void OPkeyboardUpdate(OPtimer* timer);
void OPkeyboardUpdatePost(OPtimer* timer);
OPint OPkeyboardIsDown(OPkeyboardKey key);
OPint OPkeyboardIsUp(OPkeyboardKey key);
OPint OPkeyboardWasPressed(OPkeyboardKey key);
OPint OPkeyboardWasReleased(OPkeyboardKey key);
OPint OPkeyboardAnyInputIsDown();

#endif