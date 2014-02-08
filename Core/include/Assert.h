#include "./Log.h"
#include "./Core.h"

#ifdef _WARNING
	#define ASSERT(condition, message) \
	do {\
	if (!(condition)) {\
		OPlog("ASSERT: %s", message); \
	} \
	} while (false)
#elif defined _DEBUG
	#define ASSERT(condition, message) \
	do {\
		if (!(condition)) {\
			OPlog("ASSERT: %s", message); \
			throw 1;\
		} \
	} while (false)
#else
	#define ASSERT ;
#endif