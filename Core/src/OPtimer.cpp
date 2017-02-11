#include "./Core/include/OPtimer.h"
#include "./Core/include/OPmemory.h"

//----------------------------------------------------------------------------
OPint OPtimer::Init(){

#if defined(OPIFEX_UNIX)
	gettimeofday(&(TimeLastTick), NULL);
	TotalGametime = 0;
	Elapsed = 0;
#elif defined(OPIFEX_WINDOWS)
	// Windows specific values for time
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&TimeLastTick);
	TotalGametime = 0;
	Elapsed = 0;
#ifdef _DEBUG
	ElapsedHighRes = 0;
#endif
#endif

	return 0;
}
//----------------------------------------------------------------------------
void OPtimer::Tick() {
#if defined(OPIFEX_UNIX)
	struct timeval time;
	ui64 elapsed;

	gettimeofday(&time, NULL);
	elapsed = ((time.tv_sec - TimeLastTick.tv_sec) * 1000000 +
		(time.tv_usec - TimeLastTick.tv_usec)) / 1000;

	TotalGametime += elapsed;
	Elapsed = elapsed;
	if (Elapsed != 0) {
		// We want to keep adding up until at least 1 ms has elapsed
		TimeLastTick = time;
	}

#elif defined(OPIFEX_WINDOWS)
	// Windows specific values for time
	LARGE_INTEGER StartingTime;

	QueryPerformanceCounter(&StartingTime);

	Elapsed = StartingTime.QuadPart - TimeLastTick.QuadPart;
	ElapsedHighRes = Elapsed * 1000;
	ElapsedHighRes /= (OPfloat)Frequency.QuadPart; 
	Elapsed *= 1000; // to milliseconds
	Elapsed /= Frequency.QuadPart;

	TotalGametime += Elapsed;
	if (Elapsed != 0) {
		// We want to keep adding up until at least 1 ms has elapsed
		TimeLastTick = StartingTime;
	}

#endif
}
