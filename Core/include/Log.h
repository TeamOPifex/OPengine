#pragma once
#include "Types.h"
#include "Target.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef OPIFEX_ANDROID
void OPLog(const char* message);
void OPLogNum(i32 number);
void OPLogFloat(f32 number);
#else

#endif
	
#ifdef __cplusplus
};
#endif