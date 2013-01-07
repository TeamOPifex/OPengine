#include "./../include/Log.h"

#include <android/log.h>
void OPLog(const char* message) {
	__android_log_write(ANDROID_LOG_ERROR, "OPIFEX", message);
}
void OPLogNum(i32 number) {
	__android_log_print(ANDROID_LOG_ERROR, "OPIFEX", "%d", number);
}
void OPLogFloat(f32 number) {
	__android_log_print(ANDROID_LOG_ERROR, "OPIFEX", "%f", number);
}