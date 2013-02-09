//////////////////////////////// OPEngine MAIN ////////////////////////////////

#include "../Core/include/Core.h"
#include "../Data/include/OPgameStates.h"

#include "./GameManager.h"
#include "./Human/Rendering/RenderSystem.h"
#include "./Human/Input/GamePadSystem.h"

#include "./Core/include/Log.h"
#include "./Human/Audio/Jukebox.h"

#include "Data\include\OPlinkedList.h"
#include "Data\include\OPheap.h"
#include "Data\include\OPlist.h"

#include "Human\Input\Controller.h"

#ifdef OPIFEX_ANDROID
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "./Data/include/OPfile.h"
#else
#include <direct.h>
#endif

GameManager* GM;
GamePadSystem* GPS;

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
	OPlinkedList* ll = OPllCreate();
	OPminHeap* heap = OPminHeapCreate(20);
	OPlist* list = OPlistCreate(5, sizeof(OPint));

	GPS = new GamePadSystem();

	printf("Inserting ");
	for(OPint i = 20; i--;){
		OPint* j = (OPint*)OPalloc(sizeof(OPint));
		*j = i;
		OPllInsertLast(ll, (ui8*)j);
		OPminHeapPush(heap, i);
		OPlistPush(list, (ui8*)&i);
		printf("%d ", *j);
	}
	printf("\n");

	OPint j = 1337;
	OPlistRemoveAt(list, 9);
	OPlistInsert(list, (ui8*)&j, 9);

	printf("Reading: ");
	OPllNode* node = ll->First;
	while (node)
	{
		printf("%d ", *(OPint*)(node->Data));
		node = node->Next;
	}
	printf("\nList: ");

	for(OPint i = OPlistSize(list); i--;){
		OPint j = *((OPint*)OPlistGet(list, i));
		printf("%d ", j);
	}
		printf("\nHeap: ");
	for(OPint i = 20; i--;){
		printf("%d ", heap->_indices[i]);
	}

	printf("\nPopping sorted: ");
	while(OPminHeapSize(heap) > 0){
		printf("%d ", OPminHeapPop(heap));
	}

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
	GPS->Update();
	GamePadState* gps = GPS->Controller(GamePadIndex_One);
	if(gps->IsConnected()){
		OPfloat r = gps->IsDown(GamePad_Button_A) ? 1.0f : 0.0f;
		OPfloat g = gps->LeftThumbX() / 2.0f + 0.5f;
		OPfloat b = gps->LeftTrigger();
		RenderSystem::ClearColor(r, g, b);
	}
	else
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

void UpdateState(OPtimer* timer){
	ActiveState->Update(timer);
}

#ifdef OPIFEX_ANDROID
#else
int main()
{
	OPinitialize = Init;
	OPupdate = UpdateState;
	OPdestroy = Destroy;

	ActiveState = OPgameStateCreate(NULL, Update, NULL);

	OPstart();
	OPend();
	return 0;
}
#endif