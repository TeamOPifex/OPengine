#include "./../include/Core.h"
#include "./COre/include/Log.h"

OPtimer* ApplicationTimer;
OPint _OPengineRunning;


#ifdef OPIFEX_ANDROID

JNIEnv* _JNIEnvironment;
jobject _JNIAssetManager;
jint _JNIHeight;
jint _JNIWidth;




JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager){
	OPLog("Init Engine");
	_JNIAssetManager = assetManager;
	_JNIEnvironment = env;
	_JNIWidth = width;
	_JNIHeight = height;
	_OPengineRunning = 1;
	OPLog("Create environment");
	ApplicationTimer = OPcreateTimer();
	OPLog("Timer Created");
	OPinitialize();
	OPLog("OPInitialized");
}

JNIEXPORT int JNICALL Java_com_opifex_GL2JNILib_step(JNIEnv * env, jobject obj){	
	OPtimerTick(ApplicationTimer);
	OPupdate(ApplicationTimer);
}

JNIEXPORT int JNICALL Java_com_opifex_GL2JNILib_destroy(JNIEnv * env, jobject obj){	
	OPdestroy();
	OPdestroyTimer(ApplicationTimer);
}

JNIEnv* JNIEnvironment() { return _JNIEnvironment; }
jobject JNIAssetManager() { return _JNIAssetManager; }
jint JNIWidth() { return _JNIWidth; }
jint JNIHeight() { return _JNIHeight; }

#endif

#ifndef __cplusplus
void (*OPinitialize)();
void (*OPupdate)(OPtimer*);
void (*OPdestroy)();

void OPstart(){
	// Initialize the engine and game
	ApplicationTimer = OPcreateTimer();
	_OPengineRunning = 1;
	OPinitialize();

	// main game loop
	while(_OPengineRunning){
		// update the timer
		OPtimerTick(ApplicationTimer);
		
		// update the game
		OPupdate(ApplicationTimer);
	}

	// game loop has finished, clean up
	OPdestroy();
	OPdestroyTimer(ApplicationTimer);
}
//----------------------------------------------------------------------------
void OPend(){
	_OPengineRunning = 0;
}
#else
/*
using namespace OPEngine::Core;

void OPCore::Start(){
	// Initialize the engine and game
	OPtimer* timer = OPcreateTimer();
	_OPengineRunning = 1;
	Initialize();

	// main game loop
	while(_OPengineRunning){
		// update the timer
		OPtimerTick(timer);
		
		// update the game
		Update(timer);
	}

	// game loop has finished, clean up
	Destroy();
	OPdestroyTimer(timer);
}

void OPCore::End(){
	_OPengineRunning = 0;
}*/
#endif