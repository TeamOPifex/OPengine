#pragma once
#include "Types.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

void OPlg(const char* message, ...);
void OPlog(const char* message, ...);

#ifdef __cplusplus
};
#endif
