#pragma once
#include "Types.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

void OPlog(const char* message, ...);
void OPlogLn(const char* message, ...);

#ifdef __cplusplus
};
#endif
