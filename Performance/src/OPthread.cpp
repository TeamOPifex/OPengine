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

	return out;
}
//-----------------------------------------------------------------------------
OPint OPthreadStop(void* retval){
	//thread->Status = OPTHREAD_STOPPED;
	pthread_exit(retval);
	return 1;
}
//-----------------------------------------------------------------------------
OPint OPthreadJoin(OPthread* thread){
	pthread_join(thread->Thread, &(thread->Return));
	return 1;
}
//-----------------------------------------------------------------------------
OPmutex OPmutexCreate(){
	OPmutex out;
	OPbzero(&out, sizeof(OPmutex));

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

	return out;
}
//-----------------------------------------------------------------------------
OPint OPmutexLock(OPmutex* mutex){
	pthread_mutex_lock(&mutex->Mutex);
}
//-----------------------------------------------------------------------------
OPint OPmutexUnlock(OPmutex* mutex){
	pthread_mutex_unlock(&mutex->Mutex);
}

