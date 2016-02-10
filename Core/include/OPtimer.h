#pragma once
#ifndef OPEngine_Core_Timer
#define OPEngine_Core_Timer

#include "OPtypes.h"

#if defined(OPIFEX_UNIX)
#include <sys/types.h>
#include <sys/time.h> // link with -lrt
#include <unistd.h>
#include <time.h>
#elif defined(OPIFEX_WINDOWS)
// Windows specific lib
#include <Windows.h>
#include <Mmsystem.h>
#endif

/* Struct definitions */
/**
 * Time measurment object.
 *	The OPtimer struct is responsible for keeping track of
 *	time elapsed between ticks, and total time over all updates.
 */
struct OPtimer{
	ui64 TotalGametime;
	ui64 Elapsed;
#if defined(OPIFEX_WINDOWS)
	LARGE_INTEGER TimeLastTick;
	LARGE_INTEGER Frequency;
#elif defined(OPIFEX_UNIX)
	struct timeval TimeLastTick;
#else
	ui64 TimeLastTick;
#endif
} ;
typedef struct OPtimer OPtimer;

//---- Function prototypes ---------------------------------------------------
// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

/**
 * OPtimerInit - creates an OPtimer instance
 *	Allocates space and initializes an OPtimer struct, then returns a
 *	pointer to the allocated memory segment. If allocation fails, a NULL
 *	pointer is returned.
 *	@param timer Pointer to new instance of OPtimer
 *	@return Error code on failure
 */
OPint OPtimerInit(struct OPtimer *timer);

//----------------------------------------------------------------------------
/**
 * OPtimerTick - updates a OPtimer
 *	Updates an OPtimer instance with updated internal values representing
 *	time at the moment of the call. This function will not check to see
 *	if the OPtimer pointer is null before operating on it.
 * @param timer Pointer to an OPtimer instance which will be updated.
 */
void OPtimerTick(struct OPtimer* timer);

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
OPfloat  OPtimerDelta(struct OPtimer* timer);

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
ui64 OPtimerTotal(struct OPtimer* timer);

#ifdef __cplusplus
}
#endif

#endif
