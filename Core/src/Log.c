#include "./../include/Log.h"

#ifdef OPIFEX_ANDROID
#include <android/log.h>
void OPLog(const char* message) {
	__android_log_write(ANDROID_LOG_ERROR, "OPIFEX", message);
}
void OPLog_i32(i32 number) {
	__android_log_print(ANDROID_LOG_ERROR, "OPIFEX", "%d", number);
}
void OPLog_f32(f32 number) {
	__android_log_print(ANDROID_LOG_ERROR, "OPIFEX", "%f", number);
}
#else

#endif