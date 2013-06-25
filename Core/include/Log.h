#pragma once
#include "Types.h"
#include "Target.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

void OPLog_i32(i32 number);
void OPLog_ui32(ui32 number);
void OPLog_i64(i64 number);
void OPLog_ui64(ui64 number);
void OPLog_f32(f32 number);
void OPLog_str_i32(const char* message, i32 number);

void OPLog(const char* message, ...);

#ifdef __cplusplus
};
#endif
