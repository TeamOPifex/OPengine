#ifndef OPENGINE_HUMAN_INPUT_KEYBOARD
#define OPENGINE_HUMAN_INPUT_KEYBOARD

#include "./Human/include/Input/Enums/OPkeyboardKeys.h"
#include "./Core/include/OPtimer.h"

struct OPkeyboardState {
	OPint	keys[_OPKEYBOARD_MAX];
	OPint	prevKeys[_OPKEYBOARD_MAX];
};
typedef struct OPkeyboardState OPkeyboardState;

extern OPkeyboardState Keyboard;
extern void (*OPKEYBOARD_STREAM)(OPchar);

void OPkeyboardUpdate(OPtimer* timer);
void OPkeyboardUpdatePost(OPtimer* timer);
OPint OPkeyboardIsDown(OPkeyboardKey key);
OPint OPkeyboardIsUp(OPkeyboardKey key);
OPint OPkeyboardWasPressed(OPkeyboardKey key);
OPint OPkeyboardWasReleased(OPkeyboardKey key);
OPint OPkeyboardAnyInputIsDown();

void OPkeyboardKey(OPuint codepoint);

#endif
