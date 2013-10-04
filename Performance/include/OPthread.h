#ifndef OPPERF_THREAD
#define OPPERF_THREAD

#include "Core/include/Types.h"
#include "Core/include/Log.h"
#include "Core/include/DynamicMemory.h"

#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)

#endif

//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
typedef struct{
#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
	pthread_t Thread;
	void* Return;
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)

#endif
} OPthread;

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPthread OPthreadStart(void* (*function) (void*), void* params);
OPint    OPthreadStop(OPthread* thread);
OPint    OPthreadJoin(OPthread* thread);

#endif