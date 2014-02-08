#include "./Log.h"
#include "./Core.h"

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
			throw 1;\
		} \
	} while (0)
#else
	#define ASSERT ;
#endif


#ifdef _WARNING
	#define ASSERTC(condition, message) \
	do {\
	if (!(condition)) {\
		OPlog("ASSERT: %s", message); \
	} \
	} while (0)
#elif defined _DEBUG
	#define ASSERTC(condition, message) \
	do {\
		if (!(condition)) {\
			OPlog("ASSERT: %s", message); \
			exit(1);\
		} \
	} while (0)
#else
	#define ASSERTC ;
#endif