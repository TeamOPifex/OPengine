#pragma once
#ifndef OPEngine_Core_Timer
#define OPEngine_Core_Timer

#include "Target.h" // defines the current build target
#include "Types.h"
#include "DynamicMemory.h"

#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
#include <sys/types.h>
#include <sys/time.h> // link with -lrt
#include <unistd.h>
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
// Windows specific lib
#elif defined(OPIFEX_ANDROID)
// android specific lib
#endif

/* Struct definitions */
typedef struct{
	ui64 TotalGametime;
	ui64 TimeLastTick;
	ui64 Elapsed;
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	struct timeval _lastTime;
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
// Windows specific values for time
#elif defined(OPIFEX_ANDROID)
// android specific values for time
#endif
} OPtimer;

/* function definitions */
OPtimer* CreateTimer();
//----------------------------------------------------------------------------
void DestroyTimer(OPtimer* timer);
//----------------------------------------------------------------------------
void OPtimerTick(OPtimer* timer);
//----------------------------------------------------------------------------
OPfloat  OPtimerDelta(OPtimer* timer);
//----------------------------------------------------------------------------
ui64 OPtimerTotal(OPtimer* timer);

#endif
