#ifdef OPIFEX_ANDROID
#include <jni.h>
#include "./Core/include/Core.h"

typedef struct JniMethodInfo_ {
    JNIEnv *    env;
    jclass      classID;
    jmethodID   methodID;
} JniMethodInfo;	

jclass getClassID(const char *className) {
    jclass ret = JNIEnvironment()->FindClass(className);
    if (!ret) {
        OPLog("Failed to find class of %s", className);
    }
 
    return ret;
}

bool getMethodInfo(JniMethodInfo &methodInfo,
        const char *className, const char *methodName, const char *paramCode) {
    jmethodID methodID = 0;
    bool bRet = false;
 
    do {
        if (!JNIEnvironment()) {
            break;
        }
 
        jclass classID = getClassID(className);
 
        methodID = JNIEnvironment()->GetMethodID(classID, methodName, paramCode);
        if (!methodID) {
            OPLog("Failed to find method id of %s", methodName);
            break;
        }
 
        methodInfo.classID = classID;
        methodInfo.env = JNIEnvironment();
        methodInfo.methodID = methodID;
 
        bRet = true;
    } while (0);
 
    return bRet;
}


bool getStaticMethodInfo(JniMethodInfo &methodinfo,
        const char *className, const char *methodName, const char *paramCode) {
    jmethodID methodID = 0;
    bool bRet = false;
 
    do {
        if (!JNIEnvironment()) {
            break;
        }
 
        jclass classID = getClassID(className);
 
        methodID = JNIEnvironment()->GetStaticMethodID(classID, methodName, paramCode);
        if (!methodID) {
            OPLog("Failed to find static method id of %s", methodName);
            break;
        }
 
        methodinfo.classID = classID;
        methodinfo.env = JNIEnvironment();
        methodinfo.methodID = methodID;
 
        bRet = true;
    } while (0);
 
    return bRet;	
}

#endif