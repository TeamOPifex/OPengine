#ifndef OPENGINE_HUMAN_INPUT_MOUSEKEY
#define OPENGINE_HUMAN_INPUT_MOUSEKEY

#include "./Core/include/Types.h"

typedef enum {
	OPKEY_LBUTTON = 0,
	OPKEY_RBUTTON,
	OPKEY_CANCEL,
	OPKEY_MBUTTON,
	OPKEY_XBUTTON1,
	OPKEY_XBUTTON2,
	OPKEY_XBUTTON3,
	OPKEY_XBUTTON4,
	OPMOUSE_MAX = 8
} OPmouseKeys;

extern ui32 OPmouseMapping[OPMOUSE_MAX];

#endif