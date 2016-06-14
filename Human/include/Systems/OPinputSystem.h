#ifndef OPENGINE_HUMAN_SYSTEM_INPUT
#define OPENGINE_HUMAN_SYSTEM_INPUT

#include "./Human/include/Input/OPkeyboard.h"
#include "./Human/include/Input/OPmouse.h"
#include "./Human/include/Input/OPtouch.h"
#include "./Human/include/Input/OPgamePad.h"
#include "./Human/include/Input/OPmyo.h"
#include "./Human/include/Input/OPoculus.h"

inline void OPinputSystemUpdate(OPtimer* timer) {
	OPkeyboardUpdate(timer);
	OPmouseUpdate();
	OPGAMEPADSYSTEM.Update();
}

#endif