#pragma once
#include "Types.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

// Prints out to the console, accepts variable arguments like sprintf
void OPlg(const char* message, ...);

// Prints out to the console, accepts variable arguments like sprintf, 
// does not write a new line at the end of the message
void OPlog(const char* message, ...);

#ifdef __cplusplus
};
#endif
