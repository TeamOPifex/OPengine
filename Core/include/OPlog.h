#ifndef OP_ENGINE_CORE_LOG
#define OP_ENGINE_CORE_LOG

#include "OPtypes.h"
#include <stdarg.h>

#ifdef OPIFEX_UNIX
#include <unistd.h>
#endif
#include <string.h>
#include <stdio.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

extern i32 LogToHandle;

void OPlogSetOutput(i32 handle);

// Prints out to the console, accepts variable arguments like sprintf
void OPlg(const char* message, ...);

// Prints out to the console, accepts variable arguments like sprintf, 
// does not write a new line at the end of the message
void OPlog(const char* message, ...);

void OPlogDebug(const char* message, ...);

void OPlogInfo(const char* message, ...) ;

void OPlogWarn(const char* message, ...);

void OPlogErr(const char* message, ...);

#ifdef __cplusplus
};
#endif

#endif