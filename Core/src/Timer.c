#include "./../include/Timer.h"
//#include <stdio.h>

//----------------------------------------------------------------------------
OPtimer* OPcreateTimer(){
	OPtimer* timer = (OPtimer*)OPalloc((OPuint)sizeof(OPtimer));
	
	// if allocation failed then return a null pointer
	if(!timer) return NULL;
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
	gettimeofday(&(timer->_lastTime), NULL);
	timer->TotalGametime = 0;
	timer->TimeLastTick = 0;
	timer->Elapsed = 0;
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
// Windows specific values for time
	
	QueryPerformanceFrequency(&(timer->frequency));
	QueryPerformanceCounter(&(timer->_lastTime));
#endif

	return timer;
}
//----------------------------------------------------------------------------
void OPdestroyTimer(OPtimer* timer){
	if(timer) OPfree(timer);
}
//----------------------------------------------------------------------------
void OPtimerTick(OPtimer* timer){
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
	struct timeval time;
	ui64 elapsed;

	gettimeofday(&time, NULL);
	elapsed = (time.tv_sec - timer->_lastTime.tv_sec) * 1000000 + 
		  (time.tv_usec - timer->_lastTime.tv_usec);

	timer->TotalGametime += elapsed;
	timer->TimeLastTick = (time.tv_sec * 1000000 + time.tv_usec);
	timer->Elapsed = elapsed;
	
	timer->_lastTime = time;
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
// Windows specific values for time
	LARGE_INTEGER t2;
	double elapsed;
	QueryPerformanceCounter(&t2);
	elapsed = (t2.QuadPart - timer->_lastTime.QuadPart) * 1000.0 / timer->frequency.QuadPart;

	timer->TotalGametime += elapsed;	
	timer->TimeLastTick = timer->_lastTime.QuadPart * 1000.0 / timer->frequency.QuadPart;
	timer->Elapsed = elapsed;
#endif	
}
//----------------------------------------------------------------------------
OPfloat  OPtimerDelta(OPtimer* timer){
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
	return (OPfloat)(timer->Elapsed / 1000.0);
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	return (OPfloat)(timer->Elapsed * 1000.0 / timer->frequency.QuadPart);
#endif	
}
//-----------------------------------------------------------------------------
ui64 OPtimerTotal(OPtimer* timer){
	return timer->TotalGametime / 1000000.0;
}

