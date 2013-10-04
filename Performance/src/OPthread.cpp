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
	return out;
}
//-----------------------------------------------------------------------------
OPint OPthreadStop(void* retval){
	pthread_exit(retval);
	return 1;
}
//-----------------------------------------------------------------------------
OPint OPthreadJoin(OPthread* thread){
	pthread_join(thread->Thread, &(thread->Return));
	return 1;
}
//-----------------------------------------------------------------------------

