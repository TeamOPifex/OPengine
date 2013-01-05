#pragma once
#include "./../include/Target.h"

#ifdef OPIFEX_ANDROID
#include <android/log.h>
void OPLog(char* message) {
	__android_log_write(ANDROID_LOG_ERROR, "OPIFEX", message);
}
void OPLog(unsigned int number) {
	__android_log_print(ANDROID_LOG_ERROR, "OPIFEX", "%d", number);
}
#else
void OPLog(char* message) {

}
#endif