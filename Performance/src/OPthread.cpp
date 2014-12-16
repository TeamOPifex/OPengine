#include "./Performance/include/OPthread.h"

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPthread OPthreadStart(void* (*function) (void*), void* params){
	OPthread out;
	OPbzero(&out, sizeof(OPthread));

#if defined(OPIFEX_WINDOWS)
	out.Thread = CreateThread( NULL,
		0,
		(LPTHREAD_START_ROUTINE)function,
		params,
		0,
		&out.ThreadId);
#else
	OPint err = -1;
	err = pthread_create(&out.Thread, NULL, function, params);
	if(err){
		const OPchar* f = "OPthread: Creation failed.";
		switch(err){
			case EAGAIN:
				OPlog("%s process limit of %d may have been reached.", f, RLIMIT_NPROC);
				break;
			case EINVAL:
				OPlog("%s Invalid attributes set.", f);
				break;
			case EPERM:
				OPlog("%s Insufficent permissions to spawn thread.", f);
				break;
		}
	}
	else{
		out.Status = OPTHREAD_RUNNING;
	}
#endif

	return out;
}
//-----------------------------------------------------------------------------
OPint OPthreadStop(void* retval){
#if defined(OPIFEX_WINDOWS)
	CloseHandle(NULL);
#else
	//thread->Status = OPTHREAD_STOPPED;
	pthread_exit(retval);
#endif
	return 1;
}
//-----------------------------------------------------------------------------
OPint OPthreadJoin(OPthread* thread){
	
#if defined(OPIFEX_WINDOWS)
	WaitForSingleObject(thread->Thread, INFINITE);
#else
	pthread_join(thread->Thread, &(thread->Return));
#endif
	return 1;
}
//-----------------------------------------------------------------------------
OPmutex OPmutexCreate(){
	OPmutex out;
	OPbzero(&out, sizeof(OPmutex));
	
#if defined(OPIFEX_WINDOWS)
	out.Mutex = CreateMutex(NULL, false, NULL);
	if(out.Mutex == NULL) {
		OPlog("Mutex create failed");
	}
#else
	OPint err = -1;
	err = pthread_mutex_init(&out.Mutex, NULL);
	if(err){
		const OPchar* f = "OPmutex: Creation failed.";
		switch(err){
			case EAGAIN:
				OPlog("%s process limit of %d may have been reached.", f, RLIMIT_NPROC);
				break;
			case ENOMEM:
				OPlog("%s Insufficient memory exists to initialise the mutex.", f);
				break;
			case EPERM:
				OPlog("%s Insufficent permissions to spawn thread.", f);
				break;
		}
	}
#endif

	return out;
}
//-----------------------------------------------------------------------------
OPint OPmutexLock(OPmutex* mutex){
#if defined(OPIFEX_WINDOWS)
	WaitForSingleObject(mutex->Mutex, INFINITE);
	return 0;
#else
	pthread_mutex_lock(&mutex->Mutex);
	return 0;
#endif
}
//-----------------------------------------------------------------------------
OPint OPmutexUnlock(OPmutex* mutex){
#if defined(OPIFEX_WINDOWS)
	ReleaseMutex(mutex->Mutex);
	return 0;
#else
	pthread_mutex_unlock(&mutex->Mutex);
	return 0;
#endif
}

