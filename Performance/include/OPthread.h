#ifndef OPPERF_THREAD
#define OPPERF_THREAD

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"

#if defined(OPIFEX_UNIX)
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#elif defined(OPIFEX_WINDOWS)
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
#if defined(OPIFEX_UNIX)
	pthread_t Thread;
	void* Return;
#elif defined(OPIFEX_WINDOWS)
	HANDLE Thread;
	DWORD ThreadId;
	void* Return;
#endif
} OPthread;

typedef struct{
#if defined(OPIFEX_UNIX)
	pthread_mutex_t Mutex;
#elif defined(OPIFEX_WINDOWS)
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

EXPORT_METHOD OPthread OPthreadStart(void* (*function) (void*), void* params);
EXPORT_METHOD OPint    OPthreadStop(void* retval);
EXPORT_METHOD OPint    OPthreadJoin(OPthread* thread);

//-----------------------------------------------------------------------------

EXPORT_METHOD OPmutex OPmutexCreate();
EXPORT_METHOD OPint   OPmutexLock(OPmutex* mutex);
EXPORT_METHOD OPint   OPmutexUnlock(OPmutex* mutex);
#endif