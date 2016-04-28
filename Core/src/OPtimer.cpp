#include "./Core/include/OPtimer.h"
#include "./Core/include/OPmemory.h"

//----------------------------------------------------------------------------
OPint OPtimerInit(struct OPtimer *timer){
	OPbzero(timer, sizeof(struct OPtimer));
	if(!timer) return -1;

#if defined(OPIFEX_UNIX)
	gettimeofday(&(timer->TimeLastTick), NULL);
	timer->TotalGametime = 0;
	timer->Elapsed = 0;
#elif defined(OPIFEX_WINDOWS)
	// Windows specific values for time
	QueryPerformanceFrequency(&timer->Frequency);
	QueryPerformanceCounter(&timer->TimeLastTick);
	timer->TotalGametime = 0;
	timer->Elapsed = 0;
#endif

	return 0;
}
//----------------------------------------------------------------------------
void OPtimerTick(struct OPtimer* timer) {
#if defined(OPIFEX_UNIX)
	struct timeval time;
	ui64 elapsed;

	gettimeofday(&time, NULL);
	elapsed = ((time.tv_sec - timer->TimeLastTick.tv_sec) * 1000000 +
		(time.tv_usec - timer->TimeLastTick.tv_usec)) / 1000;

	timer->TotalGametime += elapsed;
	timer->Elapsed = elapsed;
	if (timer->Elapsed != 0) {
		// We want to keep adding up until at least 1 ms has elapsed
		timer->TimeLastTick = time;
	}

#elif defined(OPIFEX_WINDOWS)
	// Windows specific values for time
	LARGE_INTEGER StartingTime;

	QueryPerformanceCounter(&StartingTime);

	timer->Elapsed = StartingTime.QuadPart - timer->TimeLastTick.QuadPart;
	timer->Elapsed *= 1000; // to milliseconds
	timer->Elapsed /= timer->Frequency.QuadPart;

	timer->TotalGametime += timer->Elapsed;
	if (timer->Elapsed != 0) {
		// We want to keep adding up until at least 1 ms has elapsed
		timer->TimeLastTick = StartingTime;
	}

#endif
}

//----------------------------------------------------------------------------

OPfloat  OPtimerDelta(struct OPtimer* timer){
	return (OPfloat)(timer->Elapsed / 1000.0);
}

//-----------------------------------------------------------------------------

ui64 OPtimerTotal(struct OPtimer* timer){
	return timer->TotalGametime;// / 1000000.0;
}
