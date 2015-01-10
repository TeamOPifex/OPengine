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

	extern OPint LogToHandle;

	EXPORT_METHOD void OPlogSetOutput(OPint handle);

// Prints out to the console, accepts variable arguments like sprintf
EXPORT_METHOD void OPlg(const char* message, ...);

// Prints out to the console, accepts variable arguments like sprintf, 
// does not write a new line at the end of the message
EXPORT_METHOD void OPlog(const char* message, ...);

EXPORT_METHOD void OPlogDebug(const char* message, ...);

EXPORT_METHOD void OPlogInfo(const char* message, ...);

EXPORT_METHOD void OPlogWarn(const char* message, ...);

EXPORT_METHOD void OPlogErr(const char* message, ...);

#ifdef __cplusplus
};
#endif

#endif