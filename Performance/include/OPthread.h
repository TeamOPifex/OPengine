#ifndef OPPERF_THREAD
#define OPPERF_THREAD

#include "Core/include/Types.h"
#include "Core/include/Log.h"
#include "Core/include/DynamicMemory.h"

#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) || defined(OPIFEX_OSX32) || defined(OPIFEX_OSX64)
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
#include <windows.h>
#endif

#define OPTHREAD_STOPPED 0
#define OPTHREAD_RUNNING 1

//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
typedef struct{
	OPint Status;
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) || defined(OPIFEX_OSX32) || defined(OPIFEX_OSX64)
	pthread_t Thread;
	void* Return;
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	HANDLE Thread;
	DWORD ThreadId;
	void* Return;
#endif
} OPthread;

typedef struct{
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID) || defined(OPIFEX_OSX32) || defined(OPIFEX_OSX64)
	pthread_mutex_t Mutex;
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	HANDLE Mutex;
#endif
} OPmutex;

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPthread OPthreadStart(void* (*function) (void*), void* params);
OPint    OPthreadStop(void* retval);
OPint    OPthreadJoin(OPthread* thread);
//-----------------------------------------------------------------------------
OPmutex OPmutexCreate();
OPint   OPmutexLock(OPmutex* mutex);
OPint   OPmutexUnlock(OPmutex* mutex);
#endif