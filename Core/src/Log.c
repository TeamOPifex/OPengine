#include "./../include/Log.h"

#ifdef OPIFEX_ANDROID
#include <android/log.h>

void OPlog(const char* message, ...){
    va_list arg;
	va_start(arg, message);
	__android_log_vprint(ANDROID_LOG_ERROR, "OPIFEX", message, arg);
   va_end(arg);
}

#else
#include <string.h>
#include <stdio.h>
#include <errno.h>

void OPlog(const char* message, ...){
	char buffer[1024];
    va_list args;
	va_start(args, message);
	vsnprintf(buffer, sizeof buffer, message, args);
	if(errno) {
		perror(buffer);
		errno = 0;
	} else {
		write(1, buffer, strlen(buffer));
	}
    va_end(args);
}

void OPlogLn(const char* message, ...){
	char buffer[1024];
    va_list args;
	va_start(args, message);
	vsnprintf(buffer, sizeof buffer, message, args);
	if(errno) {
		perror(buffer);
		errno = 0;
	} else {
		write(1, buffer, strlen(buffer));
		write(1, "\n", 1);
	}
    va_end(args);
}
#endif
