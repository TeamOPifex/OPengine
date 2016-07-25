#include "./OPlog.h"
#include "./OPcore.h"

// A Standard ASSERT call, if it evaluates to true, 
// If in DEBUG mode it exits the program
// If in WARNING mode it will only spit out a message
// Otherwise it will do nothing
#ifdef _WARNING
	#define ASSERT(condition, message) \
	do {\
	if (!(condition)) {\
		OPlog("ASSERT: (%s:%d) %s", __FILE__, __LINE__, message); \
	} \
	} while (0)
#elif defined _DEBUG
	#include "stdlib.h"
	#define ASSERT(condition, message) \
	do {\
		if (!(condition)) {\
			OPlog("ASSERT: (%s:%d) %s", __FILE__, __LINE__, message); \
			exit(1);\
		} \
	} while (0)
#else
	#define ASSERT(condition, message) ;
#endif