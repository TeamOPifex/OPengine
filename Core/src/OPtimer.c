#include "./Core/include/OPtimer.h"
#include "./Core/include/OPmemory.h"

//----------------------------------------------------------------------------
OPint OPtimerCreate(OPtimer* timer){
	OPbzero(timer, sizeof(OPtimer));
	if(!timer) return -1;
#if defined(OPIFEX_UNIX)
	gettimeofday(&(timer->_lastTime), NULL);
	timer->TotalGametime = 0;
	timer->TimeLastTick = 0;
	timer->Elapsed = 0;
#elif defined(OPIFEX_WINDOWS)
// Windows specific values for time
	timer->TotalGametime = 0;
	timer->TimeLastTick = timeGetTime();
	timer->Elapsed = 0;
#endif

	return 0;
}
//----------------------------------------------------------------------------
void OPtimerTick(OPtimer* timer){
#if defined(OPIFEX_UNIX)
	struct timeval time;
	ui64 elapsed;
	
	gettimeofday(&time, NULL);
	elapsed = ((time.tv_sec - timer->_lastTime.tv_sec) * 1000000 + 
		  (time.tv_usec - timer->_lastTime.tv_usec)) / 1000;

	timer->TotalGametime += elapsed;
	timer->TimeLastTick = (time.tv_sec * 1000000 + time.tv_usec);
	timer->Elapsed = elapsed;
	
	timer->_lastTime = time;
#elif defined(OPIFEX_WINDOWS)
	// Windows specific values for time
	DWORD time = timeGetTime();



	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);


	timer->Elapsed = StartingTime.QuadPart - timer->TimeLastTick;
	timer->Elapsed *= 1000;
	timer->Elapsed /= Frequency.QuadPart;
	timer->TimeLastTick = StartingTime.QuadPart;
	timer->TotalGametime += timer->Elapsed;

#endif
}

//----------------------------------------------------------------------------

OPfloat  OPtimerDelta(OPtimer* timer){
#if defined(OPIFEX_UNIX)
	return (OPfloat)(timer->Elapsed / 1000.0);
#elif defined(OPIFEX_WINDOWS)
	return (OPfloat)(timer->Elapsed / 1000.0);
#endif	
}

//-----------------------------------------------------------------------------

ui64 OPtimerTotal(OPtimer* timer){
	return timer->TotalGametime;// / 1000000.0;
}

