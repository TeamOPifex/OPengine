#pragma once
#ifndef OPEngine_Core_Timer
#define OPEngine_Core_Timer

#include "Target.h" // defines the current build target
#include "Types.h"
#include "DynamicMemory.h"

#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
#include <sys/types.h>
#include <sys/time.h> // link with -lrt
#include <unistd.h>
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
// Windows specific lib
#include <Windows.h>
#endif

/* Struct definitions */
typedef struct{
	ui64 TotalGametime;
	ui64 TimeLastTick;
	ui64 Elapsed;
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
	struct timeval _lastTime;
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
// Windows specific values for time
	LARGE_INTEGER frequency;
	LARGE_INTEGER _lastTime;
#endif
} OPtimer;

/* function definitions */

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
OPtimer* OPcreateTimer();
//----------------------------------------------------------------------------
void OPdestroyTimer(OPtimer* timer);
//----------------------------------------------------------------------------
void OPtimerTick(OPtimer* timer);
//----------------------------------------------------------------------------
OPfloat  OPtimerDelta(OPtimer* timer);
//----------------------------------------------------------------------------
ui64 OPtimerTotal(OPtimer* timer);
#ifdef __cplusplus
}
#endif
#endif
