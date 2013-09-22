#include "./Core/include/Target.h"

#ifdef OPIFEX_ANDROID

#include "./Human/include/Utilities/AndroidNDK.h"
#include "./Core/include/Log.h"
#include "./Core/include/Core.h"

jclass OPjniGetClassID(const char *className) {
	jclass ret = JNIEnvironment()->FindClass(className);
	if (!ret) {
		OPLog("Failed to find class of %s", className);
	}

	return ret;
}

bool OPjniGetMethodInfo(OPJniMethodInfo &methodInfo,
	const char *className, const char *methodName, const char *paramCode) {
		jmethodID methodID = 0;
		bool bRet = false;

		do {
			if (!JNIEnvironment()) {
				break;
			}

			jclass classID = OPjniGetClassID(className);

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


bool OPjniGetStaticMethodInfo(OPJniMethodInfo &methodinfo,
	const char *className, const char *methodName, const char *paramCode) {
		jmethodID methodID = 0;
		bool bRet = false;

		do {
			if (!JNIEnvironment()) {
				break;
			}

			jclass classID = OPjniGetClassID(className);

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