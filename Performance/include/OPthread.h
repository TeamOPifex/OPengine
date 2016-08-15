#pragma once

struct OPthread;
typedef struct OPthread OPthread;

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"

#if defined(OPIFEX_UNIX)
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#elif defined(OPIFEX_WINDOWS)

#endif

#define OPTHREAD_STOPPED 0
#define OPTHREAD_RUNNING 1

struct OPthread {
	OPint Status;
#if defined(OPIFEX_UNIX)
	pthread_t Thread;
	void* Return;
#elif defined(OPIFEX_WINDOWS)
	HANDLE Thread;
	DWORD ThreadId;
	void* Return;
#endif
};

OPthread OPthreadStart(void* (*function) (void*), void* params);
OPint OPthreadStop(void* retval);
OPint OPthreadJoin(OPthread* thread);