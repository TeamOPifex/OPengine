#ifndef OP_ENGINE_CORE_LOG
#define OP_ENGINE_CORE_LOG

#include "OPtypes.h"
#include <stdarg.h>

#ifdef OPIFEX_UNIX
#include <unistd.h>
#endif
#include <stdio.h>
#include <errno.h>

extern i32 LogToHandle;
extern ui32 OP_LOG_LEVEL;

extern void(*OPlogHandler)(ui32, const char*, const char*);

void OPlogSetOutput(i32 handle);

// Prints out to the console, accepts variable arguments like sprintf
void OPlg(const char* message, ...);

// Prints out to the console, accepts variable arguments like sprintf,
// does not write a new line at the end of the message
void OPlog(const char* message, ...);

void OPvlog(ui32 level, const char* channel, const char* message, va_list args);

void OPlogChannel(i32 level, const char* channel, const char* message, ...);

// Level 10
void OPlogDebug(const char* message, ...);

// Level 30
void OPlogInfo(const char* message, ...) ;

// Level 20
void OPlogWarn(const char* message, ...);

// Level 0
void OPlogErr(const char* message, ...);


#endif
