#include "./../include/Log.h"

#ifdef OPIFEX_ANDROID
#include <android/log.h>
void OPLog_i32(i32 number) {
	__android_log_print(ANDROID_LOG_ERROR, "OPIFEX", "%d", number);
}
void OPLog_ui32(ui32 number) {
	__android_log_print(ANDROID_LOG_ERROR, "OPIFEX", "%d", number);
}
void OPLog_i64(i64 number) {
	__android_log_print(ANDROID_LOG_ERROR, "OPIFEX", "%d", number);
}
void OPLog_ui64(ui64 number) {
	__android_log_print(ANDROID_LOG_ERROR, "OPIFEX", "%d", number);
}
void OPLog_f32(f32 number) {
	__android_log_print(ANDROID_LOG_ERROR, "OPIFEX", "%f", number);
}

void OPLog_str_i32(const char* message, i32 number){
	__android_log_print(ANDROID_LOG_ERROR, "OPIFEX", "%s%d", message, number);
}

void OPLog(const char* message, ...){
    va_list arg;
	va_start(arg, message);
	__android_log_vprint(ANDROID_LOG_ERROR, "OPIFEX", message, arg);
   va_end(arg);
}

#else
#include <stdio.h>
void OPLog_i32(i32 number) {
	printf("%d", number);
	printf("\n");
}
void OPLog_ui32(ui32 number) {
	printf("%d", number);
	printf("\n");
}
void OPLog_i64(i64 number) {
	printf("%d", number);
	printf("\n");
}
void OPLog_ui64(ui64 number) {
	printf("%d", number);
	printf("\n");
}
void OPLog_f32(f32 number) {
	printf("%f", number);
	printf("\n");
}
void OPLog_str_i32(const char* message, i32 number){
	printf("%s%d\n", message, number);
}
void OPLog(const char* message, ...){
	char buffer[1024];
    va_list args;
	va_start(args, message);
	vsnprintf(buffer, sizeof buffer, message, args);
	perror(buffer);
    va_end(args);
}
#endif