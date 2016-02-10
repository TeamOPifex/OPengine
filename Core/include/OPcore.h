#pragma once
#ifndef OPEngine_Core
#define OPEngine_Core
#define WIN32_LEAN_AND_MEAN 0

#include "OPtypes.h"
#include "OPgameCycle.h"
#include "OPmemory.h"
#include "OPtimer.h"
#include "Assert.h"
#include "OPlog.h"
#include "OPdir.h"

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

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
#endif

//---- Function prototypes ---------------------------------------------------

#ifdef OPIFEX_ANDROID
	void OPstart(struct android_app* state);
	void OPstartStepped(struct android_app* state);
#elif defined(OPIFEX_IOS)
	void OPstartRender();
    OPint OPstartUpdate();
    void OPstart(int argc, char** args);
#else
	/**
	 * Begins the game cycle.
	 *	This function is responsible for several actions. It creates an
	 *	OPtimer instance, and invokes the function pointer OPinitialize() to
	 *	perform user defined initializations. Once initialized, the game loop
	 *	is started. The timer instance is updated on each iteration and passed
	 *	to the OPupdate function pointer. The game loop runs until OPend() is
	 *	called. At which point the OPdestroy() function pointer is called and
	 *	and clean up is performed.
	 * @param argc Number of arguments passed through on start
	 * @param args Each of the arguments passed into the program at start
	 */
	void OPstart(int argc, char** args);
	void OPstartStepped(int argc, char** args);
#endif

//----------------------------------------------------------------------------
/**
 * Ends the game cycle.
 *	This function sets an internal flag which will cease the game loop.
 *	and result in the termination and clean up of all user code and
 *	data.
 */
void OPend();

//----------------------------------------------------------------------------
/* Gets the current OPtimer being used by the engine
 * @return A pointer to the current OPtimer being used by the OPengine
*/
struct OPtimer* OPgetTime();
OPchar* OPgetStartupDir();
OPchar* OPgetExecutableDir();

// Helper methods to create a more cross-platform code structure for the entry point of your main
#ifdef OPIFEX_ANDROID
	#define OP_MAIN void android_main(struct android_app* state)
	#define OP_MAIN_SUCCESS return;
	#define OP_MAIN_START OPstart(state);
	#define OP_MAIN_END OPend();
#elif OPIFEX_IOS
	#define OP_MAIN int _OP_WRAPPED_MAIN(int argc, char** args)
	#define OP_MAIN_START OPstart(argc, args);
	#define OP_MAIN_START_STEPPED OPstartStepped(argc, args);
	#define OP_MAIN_END OPend();
	#define OP_MAIN_SUCCESS return 0;
#else

#if defined(OPIFEX_OPTION_RELEASE) && defined(OPIFEX_WINDOWS)
	#define OP_MAIN  int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
	#define OP_MAIN_START OPstart(__argc, __argv);
	#define OP_MAIN_START_STEPPED OPstartStepped(__argc, __argv);
#else
	#define OP_MAIN int main(int argc, char** args)
	#define OP_MAIN_START OPstart(argc, args);
	#define OP_MAIN_START_STEPPED OPstartStepped(argc, args);
#endif

#define OP_MAIN_SUCCESS return 0;
#define OP_MAIN_END OPend();

#endif

#ifdef __cplusplus
};
#endif

#endif
