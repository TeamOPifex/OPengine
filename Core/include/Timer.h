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
/**
 *Time measurment object.
 *	The OPtimer struct is responsible for keeping track of
 *	time elapsed between ticks, and total time over all updates.
 */
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

//---- Function prototypes ---------------------------------------------------
// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
/**
 * OPcreateTimer - creates an OPtimer instance
 *	Allocates space and initializes an OPtimer struct, then returns a
 *	pointer to the allocated memory segment. If allocation fails, a NULL
 *	pointer is returned.
 *	@return Pointer to an OPtimer instance.
 */
OPtimer* OPcreateTimer();
//----------------------------------------------------------------------------
/**
 * OPdestroyTimer - creates an OPtimer instance
 *	Frees memory occupied by an OPtimer instance. Does nothing if a null
 *	pointer is passed.
 * @param timer Pointer to an OPtimer instance which will be destroyed.
 */
void OPdestroyTimer(OPtimer* timer);
//----------------------------------------------------------------------------
/**
 * OPtimerTick - updates a OPtimer
 *	Updates an OPtimer instance with updated internal values representing
 *	time at the moment of the call. This function will not check to see
 *	if the OPtimer pointer is null before operating on it.
 * @param timer Pointer to an OPtimer instance which will be updated.
 */
void OPtimerTick(OPtimer* timer);
//----------------------------------------------------------------------------
/**
 * OPtimerDelta - returns real time between ticks.
 *	Acts as a getter for the appropriate internal values of the provided
 *	OPtimer instance. The float value retuned is as a measurment in
 *	milliseconds since the last call to OPtimerTick(). This function will
 *	not check to see if the OPtimer pointer is null before operating on it.
 * @param timer Pointer to an OPtimer instance.
 * @return Fractional time in milliseconds since the last tick.
 */
OPfloat  OPtimerDelta(OPtimer* timer);
//----------------------------------------------------------------------------
/**
 * OPtimerTotal - milliseconds since the timer's creation
 *	Acts as a getter for the appropriate internal values of the provided
 *	OPtimer instance. The unsigned integer value retuned is as a truncated
 *	measurment of total milliseconds from the first call to OPtimerTick()
 *	to the most recent call. This function will not check to see if the
 *	OPtimer pointer is null before operating on it.
 * @param timer Pointer to an OPtimer instance.
 * @return Whole milliseconds from the first tick, to the most recent.
 */
ui64 OPtimerTotal(OPtimer* timer);
#ifdef __cplusplus
}
#endif
#endif
