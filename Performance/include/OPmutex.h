#pragma once

struct OPmutex;
typedef struct OPmutex OPmutex;

#if defined(OPIFEX_UNIX)
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#elif defined(OPIFEX_WINDOWS)

#endif

struct OPmutex {
#if defined(OPIFEX_UNIX)
	pthread_mutex_t Mutex;
#elif defined(OPIFEX_WINDOWS)
	HANDLE Mutex;
#endif
};

OPmutex OPmutexCreate();
OPint OPmutexLock(OPmutex* mutex);
OPint OPmutexUnlock(OPmutex* mutex);
