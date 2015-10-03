#include "./Core/include/OPcore.h"
#include "./Core/include/OPlog.h"

struct OPtimer OPtime;
OPchar* _startUpDir = NULL;
OPchar* _execDir = NULL;
OPint _OPengineRunning;


#ifdef OPIFEX_ANDROID

struct android_app *OPAndroidState = NULL;

JNIEnv* _JNIEnvironment;
jobject _JNIAssetManager;
jint _JNIHeight;
jint _JNIWidth;


JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager){
	_JNIAssetManager = assetManager;
	_JNIEnvironment = env;
	_JNIWidth = width;
	_JNIHeight = height;
	_OPengineRunning = 1;
	OPtimerCreate(&OPtime);
	OPinitialize();
	OPlog("Android NDK Bridge Connected");
	OPlog("Window Size %d, %d", width, height);
}

JNIEXPORT int JNICALL Java_com_opifex_GL2JNILib_step(JNIEnv * env, jobject obj, jobject assetManager){
	if(!_OPengineRunning) return 1;

	_JNIAssetManager = assetManager;
	OPtimerTick(&OPtime);
	int val = OPupdate(&OPtime);
	if(val > 0) {
		OPlog("Returning %d to Java", val);
		_OPengineRunning = 0;
	}
	return val;
}

JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_destroy(JNIEnv * env, jobject obj){
	OPlog("Destroy");
	OPdestroy();
}

JNIEnv* JNIEnvironment() { return _JNIEnvironment; }
jobject JNIAssetManager() { return _JNIAssetManager; }
jint JNIWidth() { return _JNIWidth; }
jint JNIHeight() { return _JNIHeight; }

#endif

void (*OPinitialize)();
int(*OPupdate)(struct OPtimer*);
void (*OPdestroy)();


#ifdef OPIFEX_ANDROID

#include <jni.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

OPint _OPengineRendering = 0;
OPint _OPengineInitialize = 0;

static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
	//struct engine* engine = (struct engine*)app->userData;
	switch (cmd) {
		//case APP_CMD_SAVE_STATE:
		//	// The system has asked us to save our current state.  Do so.
		//	engine->app->savedState = malloc(sizeof(struct saved_state));
		//	*((struct saved_state*)engine->app->savedState) = engine->state;
		//	engine->app->savedStateSize = sizeof(struct saved_state);
		//	break;
	case APP_CMD_INIT_WINDOW:
		// The window is being shown, get it ready.
		OPlog("APP_CMD_INIT_WINDOW");
		if (OPAndroidState->window != NULL) {
			OPlog("Window is not null");
			_OPengineInitialize = 1;
			_OPengineRendering = 1;
		}
		break;
	case APP_CMD_TERM_WINDOW:
		// The window is being hidden or closed, clean it up.
		_OPengineRendering = 0;
		_OPengineRunning = 0;
		break;
		//case APP_CMD_GAINED_FOCUS:
		//	// When our app gains focus, we start monitoring the accelerometer.
		//	if (engine->accelerometerSensor != NULL) {
		//		ASensorEventQueue_enableSensor(engine->sensorEventQueue,
		//			engine->accelerometerSensor);
		//		// We'd like to get 60 events per second (in us).
		//		ASensorEventQueue_setEventRate(engine->sensorEventQueue,
		//			engine->accelerometerSensor, (1000L / 60) * 1000);
		//	}
		//	break;
		//case APP_CMD_LOST_FOCUS:
		//	// When our app loses focus, we stop monitoring the accelerometer.
		//	// This is to avoid consuming battery while not being used.
		//	if (engine->accelerometerSensor != NULL) {
		//		ASensorEventQueue_disableSensor(engine->sensorEventQueue,
		//			engine->accelerometerSensor);
		//	}
		//	// Also stop animating.
		//	engine->animating = 0;
		//	engine_draw_frame(engine);
		//	break;
	}
}

void OPstart(struct android_app* state) {
	// Make sure glue isn't stripped.
	// This is for the Android NDK Native Activity
	// DO NOT REMOVE
	app_dummy();

	OPAndroidState = state;
	OPAndroidState->onAppCmd = engine_handle_cmd;

	_OPengineRunning = 1;

	while (_OPengineRunning) {

		int ident;
		int events;
		struct android_poll_source* source;

		while ((ident = ALooper_pollAll(_OPengineRendering ? 0 : -1, NULL, &events, (void**)&source)) > 0) {
			if (source != NULL) {
				source->process(state, source);
			}
			if (OPAndroidState->destroyRequested != 0) {
				_OPengineRunning = 0;
				_OPengineRendering = 0;
				break;
			}
		}

		if (_OPengineInitialize) {
			_OPengineInitialize = 0;
			OPlog("Initialize Engine");
			OPtimerCreate(&OPtime);
			OPinitialize();
			_OPengineRendering = 1;
		}

		if (_OPengineRendering) {
			OPtimerTick(&OPtime);

			// update the game
			if (OPupdate(&OPtime)) {
				_OPengineRunning = 0;
			}
		}
	}

	// game loop has finished, clean up
	OPdestroy();

}
#else
void OPstart(int argc, char** args) {
	// Initialize the engine and game
	_startUpDir = OPdirCurrent();
	_execDir = OPdirExecutable();
	OPtimerCreate(&OPtime);
	_OPengineRunning = 1;
	OPinitialize();

	// main game loop
	while(_OPengineRunning){
		// update the timer
		OPtimerTick(&OPtime);

		// update the game
		if (OPupdate(&OPtime)) {
			_OPengineRunning = 0;
		}
	}

	// game loop has finished, clean up
	OPdestroy();

	OPfree(_startUpDir);
	OPfree(_execDir);

	#ifndef OPIFEX_OPTION_RELEASE
	OPlog("Alloc/Dealloc/Diff: %d / %d / %d", OPallocations, OPdeallocations, (OPallocations - OPdeallocations));
	ASSERT((OPallocations - OPdeallocations) == 0, "ALERT - Not all allocated memory was freed");
	#endif
}
#endif

//----------------------------------------------------------------------------
void OPend(){
	_OPengineRunning = 0;
}

struct OPtimer* OPgetTime() {
	return &OPtime;
}

OPchar* OPgetStartupDir() {
	return _startUpDir;
}
OPchar* OPgetExecutableDir() {
	return _execDir;
}
