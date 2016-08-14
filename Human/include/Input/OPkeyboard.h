#pragma once

struct OPkeyboardState;
typedef struct OPkeyboardState OPkeyboardState;

#include "./Human/include/Input/Enums/OPkeyboardKeys.h"
#include "./Core/include/OPtimer.h"

struct OPkeyboardState {
	bool	keys[(ui32)OPkeyboardKey::_MAX];
	bool	prevKeys[(ui32)OPkeyboardKey::_MAX];
};

extern OPkeyboardState Keyboard;
extern void (*OPKEYBOARD_STREAM)(OPchar);

void OPkeyboardUpdate(OPtimer* timer);
void OPkeyboardUpdatePost(OPtimer* timer);
bool OPkeyboardIsDown(OPkeyboardKey key);
bool OPkeyboardIsUp(OPkeyboardKey key);
bool OPkeyboardWasPressed(OPkeyboardKey key);
bool OPkeyboardWasReleased(OPkeyboardKey key);
bool OPkeyboardAnyInputIsDown();
