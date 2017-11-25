#pragma once

#include "./OPlog.h"
#include "./OPcore.h"

#if _MSC_VER && !__INTEL_COMPILER
	#include <intrin.h>
	#define IDE_BREAK() __debugbreak();
#else
	#define IDE_BREAK() 
#endif

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
			IDE_BREAK();\
		} \
	} while (0)
#else
	#define ASSERT(condition, message) ;
#endif