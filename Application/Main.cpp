//////////////////////////////// OPEngine MAIN ////////////////////////////////

#include "../Core/include/Core.h"


#include "./GameManager.h"
#include "./Human/Rendering/RenderSystem.h"
#include "./Human/Input/GamePadSystem.h"

#include "./Core/include/Log.h"
#include "./Human/Audio/Jukebox.h"

#ifdef OPIFEX_ANDROID
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "./Data/include/OPfile.h"
#else
#include <direct.h>
#endif

GameManager* GM;

#ifdef OPIFEX_ANDROID
	OPtimer* timer;
extern "C" {
	JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager);
	JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_setControllerButton(JNIEnv * env, jobject obj,  jint player,  jint button,  jint state);
    JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_setControllerAxes(JNIEnv * env, jobject obj,  jint player,  jint axes,  jfloat position);
    JNIEXPORT int JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_setConnected(JNIEnv * env, jobject obj,  jint player,  jint state);
};

// Set Controller Buttons
JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_setControllerButton(JNIEnv * env, jobject obj,  jint player,  jint button,  jint state){
	OPLog("Player Pushed:");
	OPLog_i32(button);
	GamePadSystem::Controller((GamePadIndex)(player-1))->SetButton((GamePadButton)button, state == 1);
}

// Set
JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_setControllerAxes(JNIEnv * env, jobject obj,  jint player,  jint axes,  jfloat position){
	GamePadSystem::Controller((GamePadIndex)(player-1))->SetAxis((GamePadAxes)axes, position);
}

// Set Connected
JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_setConnected(JNIEnv * env, jobject obj,  jint player,  jint state){
	OPLog("Player Connected:");
	OPLog_i32(player);
	GamePadSystem::Controller((GamePadIndex)(player - 1))->SetConnected(state == 1);
}
#endif

// Initialize
#ifdef OPIFEX_ANDROID
JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager){
	timer = OPcreateTimer();
	OPfileInit( AAssetManager_fromJava(env, assetManager));
	Jukebox::Initialize();
#else
void Init(){
	_chdir("Assets\\");
	i32 width = 1280;
	i32 height = 720;
#endif

	GM = new GameManager(width, height);
	return;
}

// Step
#ifdef OPIFEX_ANDROID
JNIEXPORT int JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj){	
	OPtimerTick(timer);
	GamePadSystem::Update();
#else
void Update( OPtimer* timer){
#endif

	bool result = GM->Update( timer );
	RenderSystem::ClearColor(0,0,0);
	GM->Draw();
	RenderSystem::Present();
	
#ifdef OPIFEX_ANDROID
	if(!result)
		return 1;
	return 0;
#else
	if(!result)
		exit(0);
	return;
#endif
}

void Destroy()
{
	delete GM;
	return;
}

#ifdef OPIFEX_ANDROID
#else
int main()
{
	OPinitialize = Init;
	OPupdate = Update;
	OPdestroy = Destroy;

	OPstart();
	OPend();
	return 0;
}
#endif