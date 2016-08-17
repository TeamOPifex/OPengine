#pragma once

#ifdef OPIFEX_ANDROID

#include <jni.h>

typedef struct JniMethodInfo_ {
	JNIEnv *    env;
	jclass      classID;
	jmethodID   methodID;
} OPJniMethodInfo;	

jclass OPjniGetClassID(const char *className);

bool OPjniGetMethodInfo(OPJniMethodInfo &methodInfo, const char *className, const char *methodName, const char *paramCode);

bool OPjniGetStaticMethodInfo(OPJniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode);

#endif