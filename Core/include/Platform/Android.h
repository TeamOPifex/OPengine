#pragma once

#include "./Core/include/OPtypes.h"

// Android entry points
#ifdef OPIFEX_ANDROID
	#include <jni.h>
	#include <android_native_app_glue.h>
	#include <android/sensor.h>
	#include <android/log.h>
	extern struct android_app *OPAndroidState;

	JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_touch(JNIEnv * env, jobject obj,  jint evt, jfloat x, jfloat y);
	JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager);
	JNIEXPORT int JNICALL Java_com_opifex_GL2JNILib_step(JNIEnv * env, jobject obj, jobject assetManager);
	JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_destroy(JNIEnv * env, jobject obj);
	JNIEnv* JNIEnvironment();
	jobject JNIAssetManager();
	jint JNIWidth();
	jint JNIHeight();

	void OPstart(struct android_app* state);
	void OPstartStepped(struct android_app* state);


// Helper methods to create a more cross-platform code structure for the entry point of your main

	#define OP_MAIN_START void android_main(struct android_app* state) {
	#define OP_MAIN_SUCCESS return;
	#define OP_MAIN_RUN OPstart(state);
	#define OP_MAIN_RUN_STEPPED OPstartStepped(state);
	#define OP_MAIN_END OPend(); }
#endif
