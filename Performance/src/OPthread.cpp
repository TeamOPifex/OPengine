#include "Performance/include/OPthread.h"

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

#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	out.Thread = CreateThread( NULL,
		0,
		(LPTHREAD_START_ROUTINE)function,
		params,
		0,
		&out.ThreadId);
#else
	OPint err = -1;
	if(err = pthread_create(&out.Thread, NULL, function, params)){
		OPchar* f = "OPthread: Creation failed.";
		switch(err){
			case EAGAIN:
				OPLog("%s process limit of %d may have been reached.", f, RLIMIT_NPROC);
				break;
			case EINVAL:
				OPLog("%s Invalid attributes set.", f);
				break;
			case EPERM:
				OPLog("%s Insufficent permissions to spawn thread.", f);
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
#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	CloseHandle(NULL);
#else
	//thread->Status = OPTHREAD_STOPPED;
	pthread_exit(retval);
#endif
	return 1;
}
//-----------------------------------------------------------------------------
OPint OPthreadJoin(OPthread* thread){
	
#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
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
	
#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	out.Mutex = CreateMutex(NULL, false, NULL);
	if(out.Mutex == NULL) {
		OPLog("Mutex create failed");
	}
#else
	OPint err = -1;
	if(err = pthread_mutex_init(&out.Mutex, NULL)){
		OPchar* f = "OPmutex: Creation failed.";
		switch(err){
			case EAGAIN:
				OPLog("%s process limit of %d may have been reached.", f, RLIMIT_NPROC);
				break;
			case ENOMEM:
				OPLog("%s Insufficient memory exists to initialise the mutex.", f);
				break;
			case EPERM:
				OPLog("%s Insufficent permissions to spawn thread.", f);
				break;
		}
	}
#endif

	return out;
}
//-----------------------------------------------------------------------------
OPint OPmutexLock(OPmutex* mutex){
#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	WaitForSingleObject(mutex->Mutex, INFINITE);
	return 0;
#else
	pthread_mutex_lock(&mutex->Mutex);
#endif
}
//-----------------------------------------------------------------------------
OPint OPmutexUnlock(OPmutex* mutex){
#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	ReleaseMutex(mutex->Mutex);
	return 0;
#else
	pthread_mutex_unlock(&mutex->Mutex);
#endif
}

