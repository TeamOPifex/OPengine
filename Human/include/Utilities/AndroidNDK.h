#ifdef OPIFEX_ANDROID
#include <jni.h>

typedef struct JniMethodInfo_ {
    JNIEnv *    env;
    jclass      classID;
    jmethodID   methodID;
} JniMethodInfo;	

jclass getClassID(JNIEnv *pEnv, const char *className) {
    jclass ret = pEnv->FindClass(className);
    if (!ret) {
        OPLog("Failed to find class of %s", className);
    }
 
    return ret;
}

bool getMethodInfo(JniMethodInfo &methodInfo,
        const char *className, const char *methodName, const char *paramCode,
        JNIEnv* pEnv) {
    jmethodID methodID = 0;
    bool bRet = false;
 
    do {
        if (!pEnv) {
            break;
        }
 
        jclass classID = getClassID(pEnv, className);
 
        methodID = pEnv->GetMethodID(classID, methodName, paramCode);
        if (!methodID) {
            OPLog("Failed to find method id of %s", methodName);
            break;
        }
 
        methodInfo.classID = classID;
        methodInfo.env = pEnv;
        methodInfo.methodID = methodID;
 
        bRet = true;
    } while (0);
 
    return bRet;
}


bool getStaticMethodInfo(JniMethodInfo &methodinfo,
        const char *className, const char *methodName, const char *paramCode,
        JNIEnv* pEnv) {
    jmethodID methodID = 0;
    bool bRet = false;
 
    do {
        if (!pEnv) {
            break;
        }
 
        jclass classID = getClassID(pEnv, className);
 
        methodID = pEnv->GetStaticMethodID(classID, methodName, paramCode);
        if (!methodID) {
            OPLog("Failed to find static method id of %s", methodName);
            break;
        }
 
        methodinfo.classID = classID;
        methodinfo.env = pEnv;
        methodinfo.methodID = methodID;
 
        bRet = true;
    } while (0);
 
    return bRet;	
}

#endif