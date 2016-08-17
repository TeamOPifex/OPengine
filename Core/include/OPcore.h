#pragma once

#include "./Core/include/OPtypes.h"
#include "./Core/include/Platform/Android.h"
#include "./Core/include/Platform/Desktop.h"
#include "./Core/include/Platform/IOS.h"

extern OPtimer OPTIMER;
extern OPchar* OPSTARTUP_PATH;
extern OPchar* OPEXECUTABLE_PATH;
extern bool OPENGINERUNNING;

//----------------------------------------------------------------------------
/**
 * Ends the game cycle.
 *	This function sets an internal flag which will cease the game loop.
 *	and result in the termination and clean up of all user code and
 *	data.
 */
void OPend();

#define OP_MAIN_END \
		OPend(); \
		return 0; \
		}