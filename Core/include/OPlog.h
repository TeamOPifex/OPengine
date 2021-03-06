#pragma once

#include "OPtypes.h"
#include <stdarg.h>

#ifdef OPIFEX_UNIX
#include <unistd.h>
#endif
#include <stdio.h>
#include <errno.h>

#ifdef OPIFEX_WINDOWS
extern HANDLE LogToHandle;
#else
extern i32 LogToHandle;
#endif

enum struct OPlogLevel {
	ERRORS = 0,
	DEBUG = 10,
	WARNINGS = 20,
	INFO = 30,
	VERBOSE = 3000,
	TIMING = 4000,
	MEMORY = 5000
};

extern ui32 OPLOGLEVEL;

extern void(*OPlogHandler)(ui32, const char*, const char*);

#ifdef OPIFEX_WINDOWS
void OPlogSetOutput(HANDLE handle);
#else
void OPlogSetOutput(i32 handle);
#endif

// Prints out to the console, accepts variable arguments like sprintf
void OPlg(const char* message, ...);

// Prints out to the console, accepts variable arguments like sprintf,
// does not write a new line at the end of the message
void OPlog(const char* message, ...);

void OPvlog(ui32 level, const char* channel, const char* message, va_list args);

void OPlogChannel(ui32 level, const char* channel, const char* message, ...);

// Level 10
void OPlogDebug(const char* message, ...);

// Level 30
void OPlogInfo(const char* message, ...) ;

// Level 20
void OPlogWarn(const char* message, ...);

// Level 0
void OPlogErr(const char* message, ...);