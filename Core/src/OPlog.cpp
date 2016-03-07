#include "./Core/include/OPlog.h"
#include <string>
#include <stdio.h>
#include <ostream>

i32 LogToHandle = 1;
ui32 OP_LOG_LEVEL = 999;
void(*OPlogHandler)(ui32, const char*, const char*) = NULL;

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
	char buffer[4096];
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



void OPlogSetOutput(i32 handle) {
	if(handle > 0) {
		LogToHandle = handle;
	}
}


void OPvlog(ui32 level, const char* channel, const char* message, va_list args) {
	char buffer[4096], buffer2[4096];

	if (errno) {
		perror("SYSTEM ERROR");
		errno = 0;
	}

	if (level > OP_LOG_LEVEL) {
		return;
	}

	if (OPlogHandler != NULL) {
		vsnprintf(buffer, sizeof buffer, message, args);
		OPlogHandler(level, channel, buffer);
	} else {
		sprintf(buffer2, "%s: %s\n", channel, message);
		vsnprintf(buffer, sizeof buffer, buffer2, args);
#ifndef OPIFEX_IOS
		write(LogToHandle, buffer, strlen(buffer));
#else
        printf(buffer);
#endif
	}
}

void OPlog(const char* message, ...){
    va_list args;
	va_start(args, message);

	OPvlog(100, "", message, args);

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
	//write(LogToHandle, buffer, strlen(buffer));
    va_end(args);
}

void OPlogChannel(i32 level, const char* channel, const char* message, ...) {
	va_list args;
	va_start(args, message);

	OPvlog(level, channel, message, args);

	va_end(args);
}

void OPlogDebug(const char* message, ...) {
	#ifndef OPIFEX_OPTION_RELEASE
	va_list args;
	va_start(args, message);

	OPvlog(10, "DEBUG", message, args);

    va_end(args);

	#endif
}

void OPlogInfo(const char* message, ...) {
	#ifndef OPIFEX_OPTION_RELEASE
	va_list args;
	va_start(args, message);

	OPvlog(30, "INFO", message, args);

    va_end(args);
	#endif
}

void OPlogWarn(const char* message, ...) {
	va_list args;
	va_start(args, message);

	OPvlog(20, "WARNING", message, args);

    va_end(args);
}

void OPlogErr(const char* message, ...) {
	va_list args;
	va_start(args, message);

	OPvlog(0, "ERROR", message, args);

    va_end(args);
}

#endif
