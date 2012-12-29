#include <assert.h>
#include <jni.h>
#include <string.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

jboolean Java_com_example_nativeaudio_NativeAudio_createAssetAudioPlayer(JNIEnv* env, jclass clazz, jobject assetManager, jstring filename){
	SLresult result;

	// Java string to UTF-8
	const jbyte *utf8 = (*env)->GetStringUTFChars*env, filename, NULL);
	assert(NULL != utf8);

	// Use the Android Asset Manager to open the file
	AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
	assert(NULL != mgr);
	AAsset* asset = AAssetManager_open(mgr, (const char *) utf8, AASSET_MODE_UNKNOWN);

	(*env)->ReleaseStringUTFChars(env, filename, utf8);

	// File must not have been found
	if(NULL == asset) return JNI_FALSE;
	
	off_t start, length;
	int fd = AAsset_openFileDescriptor(asset, &start, &length);
	assert (0 <= fd); //Ensure no errors getting file descriptor
	AAsset_close(asset);
}