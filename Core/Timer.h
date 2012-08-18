#ifndef OPEngine_Core_Timer
#define OPEngine_Core_Timer
#include "Types.h"

/* Struct definitions */
typedef struct{
	ui64 TotalGametime;
	ui64 TimeLastTick;
	ui64 Elapsed;
} OPTimer;

/* function definitions */
void TimerTick(OPTimer* timer);
f32  TimerDelta(OPTimer* timer);
ui64 TimerTotal(OPTimer* timer);

#endif
