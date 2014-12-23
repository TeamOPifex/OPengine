#include "./Core/include/OPlog.h"

OPint LogToHandle = 1;

#ifdef OPIFEX_ANDROID

#include <android/log.h>

void OPlog(const char* message, ...){
    va_list arg;
	va_start(arg, message);
	__android_log_vprint(ANDROID_LOG_ERROR, "OPIFEX", message, arg);
   va_end(arg);
}

#else



void OPlogSetOutput(OPint handle) {
	if(handle > 0) {
		LogToHandle = handle;
	}
}


void OPlog(const char* message, ...){
	char buffer[1024];
    va_list args;
	va_start(args, message);
	vsnprintf(buffer, sizeof buffer, message, args);

	if(errno) {
		perror("SYSTEM ERROR");
		errno = 0;
	}
	write(LogToHandle, buffer, strlen(buffer));
	write(LogToHandle, "\n", 1);
    va_end(args);
}

void OPlg(const char* message, ...){
	char buffer[1024];
    va_list args;
	va_start(args, message);
	vsnprintf(buffer, sizeof buffer, message, args);
	if(errno) {
		perror("SYSTEM ERROR");
		errno = 0;
	}
	write(LogToHandle, buffer, strlen(buffer));
    va_end(args);
}

void OPlogDebug(const char* message, ...) {
	#ifndef OPIFEX_RELEASE
		write(LogToHandle, "DEBUG: ", 7);
	    va_list args;
		OPlog(message, args);
	#endif
}

void OPlogInfo(const char* message, ...) {
	#ifndef OPIFEX_RELEASE
		write(LogToHandle, "INFO: ", 6);
	    va_list args;
		OPlog(message, args);
	#endif
}

void OPlogWarn(const char* message, ...) {
	write(LogToHandle, "WARNING: ", 9);
    va_list args;
	OPlog(message, args);
}

void OPlogErr(const char* message, ...) {
	write(LogToHandle, "ERROR: ", 7);
    va_list args;
	OPlog(message, args);
}

#endif
