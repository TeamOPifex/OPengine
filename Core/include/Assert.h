#pragma once

#include "./OPlog.h"
#include "./OPcore.h"

#include <intrin.h>

// A Standard ASSERT call, if it evaluates to true, 
// If in DEBUG mode it exits the program
// If in WARNING mode it will only spit out a message
// Otherwise it will do nothing
#ifdef _WARNING
	#define ASSERT(condition, message) \
	do {\
	if (!(condition)) {\
		OPlogWarn("ASSERT: (%s:%d) [%s] %s", __FILE__, __LINE__, __FUNCTION__, message); \
	} \
	} while (0)
#elif defined _DEBUG
	#include "stdlib.h"
	#define ASSERT(condition, message, ...) \
	do {\
		if (!(condition)) {\
			OPlogErr("ASSERT: (%s:%d) [%s] %s", __FILE__, __LINE__, __FUNCTION__, message); \
			__debugbreak();\
			exit(1);\
		} \
	} while (0)
#else
	#define ASSERT(condition, message) ;
#endif