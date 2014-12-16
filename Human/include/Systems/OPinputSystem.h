#ifndef OPENGINE_HUMAN_SYSTEM_INPUT
#define OPENGINE_HUMAN_SYSTEM_INPUT

#include "./Human/include/Input/OPkeyboard.h"
#include "./Human/include/Input/OPmouse.h"
#include "./Human/include/Input/OPtouch.h"
#include "./Human/include/Input/OPgamePad.h"
#include "./Human/include/Input/OPmyo.h"
#ifdef OPIFEX_OCULUS
#include "./Human/include/Input/OPoculus.h"
#endif

inline void OPinputSystemUpdate() {
	OPkeyboardUpdate();
	OPmouseUpdate();
	OPgamePadSystemUpdate();
}

#endif