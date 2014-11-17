#include "./Log.h"
#include "./Core.h"

// A Standard ASSERT call, if it evaluates to true, 
// If in DEBUG mode it exits the program
// If in WARNING mode it will only spit out a message
// Otherwise it will do nothing
#ifdef _WARNING
	#define ASSERT(condition, message) \
	do {\
	if (!(condition)) {\
		OPlog("ASSERT: %s", message); \
	} \
	} while (0)
#elif defined _DEBUG
	#define ASSERT(condition, message) \
	do {\
		if (!(condition)) {\
			OPlog("ASSERT: %s", message); \
			exit(1);\
		} \
	} while (0)
#else
	#define ASSERT ;
#endif