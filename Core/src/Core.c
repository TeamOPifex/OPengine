#include "./../include/Core.h"
#include "./../include/Log.h"

OPtimer* OPtime;
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
	OPtime = OPcreateTimer();
	OPLog("Timer Created");
	OPinitialize();
	OPLog("OPInitialized");
}

JNIEXPORT int JNICALL Java_com_opifex_GL2JNILib_step(JNIEnv * env, jobject obj, jobject assetManager){	
	if(!_OPengineRunning) return 1;

	_JNIAssetManager = assetManager;
	OPtimerTick(OPtime);
	int val = OPupdate(OPtime);
	if(val > 0) {
		OPLog("Returning %d to Java", val);
		_OPengineRunning = 0;
	}
	return val;
}

JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_destroy(JNIEnv * env, jobject obj){
	OPLog("Destroy");
	OPdestroy();
	OPdestroyTimer(OPtime);
}

JNIEnv* JNIEnvironment() { return _JNIEnvironment; }
jobject JNIAssetManager() { return _JNIAssetManager; }
jint JNIWidth() { return _JNIWidth; }
jint JNIHeight() { return _JNIHeight; }

#endif

#ifndef __cplusplus
void (*OPinitialize)();
int(*OPupdate)(OPtimer*);
void (*OPdestroy)();

void OPstart(){
	// Initialize the engine and game
	OPtime = OPcreateTimer();
	_OPengineRunning = 1;
	OPinitialize();

	// main game loop
	while(_OPengineRunning){
		// update the timer
		OPtimerTick(OPtime);
		
		// update the game
		if(OPupdate(OPtime)) {
			_OPengineRunning = 0;
		}
	}

	// game loop has finished, clean up
	OPdestroy();
	OPdestroyTimer(OPtime);
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