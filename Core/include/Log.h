#pragma once
#include "Types.h"
#include "Target.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef OPIFEX_ANDROID
void OPLog(const char* message);
void OPLog_i32(i32 number);
void OPLog_f32(f32 number);
#else

#endif
	
#ifdef __cplusplus
};
#endif
