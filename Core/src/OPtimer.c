#include "./Core/include/OPtimer.h"
#include "./Core/include/OPmemory.h"

//----------------------------------------------------------------------------
OPint OPtimerCreate(struct OPtimer* timer){
	OPbzero(timer, sizeof(struct OPtimer));
	if(!timer) return -1;
#if defined(OPIFEX_UNIX)
	gettimeofday(&(timer->TimeLastTick), NULL);
	timer->TotalGametime = 0;
	timer->Elapsed = 0;
#elif defined(OPIFEX_WINDOWS)
// Windows specific values for time
	timer->TotalGametime = 0;;
	QueryPerformanceCounter(&timer->TimeLastTick);
	timer->Elapsed = 0;
#endif

	return 0;
}
//----------------------------------------------------------------------------
void OPtimerTick(struct OPtimer* timer){
#if defined(OPIFEX_UNIX)
	struct timeval time;
	ui64 elapsed;

	gettimeofday(&time, NULL);
	elapsed = ((time.tv_sec - timer->TimeLastTick.tv_sec) * 1000000 +
		(time.tv_usec - timer->TimeLastTick.tv_usec)) / 1000;

	timer->TotalGametime += elapsed;
	timer->TimeLastTick = time;
	timer->Elapsed = elapsed;

#elif defined(OPIFEX_WINDOWS)
	// Windows specific values for time
	LARGE_INTEGER StartingTime, Frequency;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	timer->Elapsed = StartingTime.QuadPart - timer->TimeLastTick.QuadPart;
	timer->Elapsed *= 1000; // to milliseconds
	timer->Elapsed /= Frequency.QuadPart;

	timer->TimeLastTick = StartingTime;
	timer->TotalGametime += timer->Elapsed;

#endif
}

//----------------------------------------------------------------------------

OPfloat  OPtimerDelta(struct OPtimer* timer){
#if defined(OPIFEX_UNIX)
	return (OPfloat)(timer->Elapsed / 1000.0);
#elif defined(OPIFEX_WINDOWS)
	return (OPfloat)(timer->Elapsed / 1000.0);
#endif
}

//-----------------------------------------------------------------------------

ui64 OPtimerTotal(struct OPtimer* timer){
	return timer->TotalGametime;// / 1000000.0;
}
