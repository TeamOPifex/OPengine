#include "./Core/include/OPlog.h"

OPint LogToHandle = 1;

#ifdef OPIFEX_ANDROID

#include <android/log.h>


void OPlogSetOutput(OPint handle) {
	if (handle > 0) {
		LogToHandle = handle;
	}
}

void OPlog(const char* message, ...){
	va_list args;
	va_start(args, message);
	__android_log_vprint(ANDROID_LOG_INFO, "OPIFEX", message, args);
	va_end(args);
}

void OPlg(const char* message, ...){
	char buffer[1024];
	va_list args;
	va_start(args, message);
	__android_log_vprint(ANDROID_LOG_INFO, "OPIFEX", message, args);
	va_end(args);
}

void OPlogDebug(const char* message, ...) {
#ifndef OPIFEX_OPTION_RELEASE
	va_list args;
	va_start(args, message);
	__android_log_vprint(ANDROID_LOG_DEBUG, "OPIFEX", message, args);
#endif
}

void OPlogInfo(const char* message, ...) {
#ifndef OPIFEX_OPTION_RELEASE
	va_list args;
	va_start(args, message);
	__android_log_vprint(ANDROID_LOG_INFO, "OPIFEX", message, args);
#endif
}

void OPlogWarn(const char* message, ...) {
	va_list args;
	va_start(args, message);
	__android_log_vprint(ANDROID_LOG_WARN, "OPIFEX", message, args);
}

void OPlogErr(const char* message, ...) {
	va_list args;
	va_start(args, message);
	__android_log_vprint(ANDROID_LOG_ERROR, "OPIFEX", message, args);
}

#else



void OPlogSetOutput(OPint handle) {
	if(handle > 0) {
		LogToHandle = handle;
	}
}

void _log(va_list args, const char* message) {
	char buffer[1024];

	vsnprintf(buffer, sizeof buffer, message, args);

	if(errno) {
		perror("SYSTEM ERROR");
		errno = 0;
	}
	write(LogToHandle, buffer, strlen(buffer));
	write(LogToHandle, "\n", 1);	
}

void OPlog(const char* message, ...){
    va_list args;
	va_start(args, message);

	_log(args, message);

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
	#ifndef OPIFEX_OPTION_RELEASE
	va_list args;
	va_start(args, message);

	write(LogToHandle, "DEBUG: ", 7);
	_log(args, message);

    va_end(args);

	#endif
}

void OPlogInfo(const char* message, ...) {
	#ifndef OPIFEX_OPTION_RELEASE
	va_list args;
	va_start(args, message);

	write(LogToHandle, "INFO: ", 6);
	_log(args, message);

    va_end(args);
	#endif
}

void OPlogWarn(const char* message, ...) {
	va_list args;
	va_start(args, message);

	write(LogToHandle, "WARNING: ", 9);
	_log(args, message);

    va_end(args);
}

void OPlogErr(const char* message, ...) {
	va_list args;
	va_start(args, message);

	write(LogToHandle, "ERROR: ", 7);
	_log(args, message);

    va_end(args);
}

#endif
