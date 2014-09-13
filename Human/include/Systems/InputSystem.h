#ifndef OPENGINE_HUMAN_SYSTEM_INPUT
#define OPENGINE_HUMAN_SYSTEM_INPUT

#include "./Human/include/Input/Input.h"
#include "./Human/include/Input/GamePadSystem.h"
#include "./Human/include/Input/Myo.h"
#ifdef OPIFEX_OCULUS
#include "./Human/include/Input/Oculus.h"
#endif

inline void OPinputSystemUpdate() {
	OPkeyboardUpdate();
	OPmouseUpdate();
	OPgamePadSystemUpdate();
}

#endif