//////////////////////////////// OPEngine MAIN ////////////////////////////////
#include <stdio.h>

#include "./Core/include/Core.h"
#include "./Data/include/OPgameStates.h"

#include "./GameManager.h"
#include "./Human/include/Rendering/RenderSystem.h"
#include "./Human/include/Input/GamePadSystem.h"

#include "./Core/include/Log.h"
#include "./Human/include/Audio/Jukebox.h"

#include "./Data/include/OPlinkedList.h"
#include "./Data/include/OPheap.h"
#include "./Data/include/OPlist.h"

#if defined(OPIFEX_ANDROID)
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
#include <direct.h>
#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#endif
#include "./Data/include/OPfile.h"
#include "./Human/include/Audio/OPaudio2.h"
#include "./Human/include/Audio/OPaudioEmitter.h"
#include "./Human/include/Audio/OPaudioPlayer.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPcontentManager.h"

GameManager* GM;
GamePadSystem* GPS;
OPaudioSource Sound, Sound1;
OPaudioEmitter Emitter, Emitter1;
OPaudioPlayer player;
OPfloat vol = 0.05f;

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

void KeyDown(int key, int action){
	OPLog("Pizza");
	OPaudSetPlayer(&player);
	OPaudPlayerPlay();
}

// Initialize
#ifdef OPIFEX_ANDROID
JNIEXPORT void JNICALL Java_com_opifex_smrf_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject assetManager){
	timer = OPcreateTimer();
	OPfileInit( AAssetManager_fromJava(env, assetManager));
	//Jukebox::Initialize();
#else
void Init(){
	#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	_chdir("assets\\");
	#else
	if(chdir("./assets")){
		OPLog("Directory changed!\n");
	}
	else
		OPLog("Directory change failed!\n");
	#endif

	i32 width = 1280;
	i32 height = 720;
#endif
	OPlinkedList* ll = OPllCreate();
	OPminHeap* heap = OPminHeapCreate(20);
	OPlist* list = OPlistCreate(5, sizeof(OPint));

	OPassetLoader loaders[] ={
		{
			".wav",
			"/assets/Audio/",
			sizeof(OPaudioSource),
			NULL,
			(OPint (*)(void*))OPaudCloseWave
		}
	};

	OPcmanInit(loaders, 1);

	GPS = new GamePadSystem();

	//OPAudio::Init();

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

	OPLog("Reading: ");
	OPllNode* node = ll->First;
	while (node){
		OPLog("%d ", *(OPint*)(node->Data));
		node = node->Next;
	}
	OPLog("\nList: ");

	for(OPint i = OPlistSize(list); i--;){
		OPint j = *((OPint*)OPlistGet(list, i));
		OPLog("%d ", j);
	}
		OPLog("\nHeap: ");
	for(OPint i = 20; i--;){
		OPLog("%d ", heap->_indices[i]);
	}

	OPLog("\nPopping sorted: ");
	while(OPminHeapSize(heap) > 0){
		OPLog("%d ", OPminHeapPop(heap));
	}

	GM = new GameManager(width, height);

#if defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	  	glfwSetKeyCallback(KeyDown);
#endif

        OPLog("Main: Song loading...");
        OPchar songPath[] = {"Audio/background.ogg"};
        //Song = OPAudio::ReadOgg(songPath);
        OPaudInit();

        OPLog("Main: Song loaded");

        Sound = OPaudOpenOgg("Audio/background.ogg");
        Sound1 = OPaudOpenWave("Audio/pew.wav");
        OPLog("Reading done!\n");
        Emitter1 = OPaudCreateEmitter(&Sound1, 0);
        player = OPaudPlayerCreate(&Sound1, 5, 0);
        Emitter = OPaudCreateEmitter(&Sound, 1);
        OPLog("Emitter created\n");

		OPaudSetEmitter(&Emitter);
        OPaudVolume(0.00f);
        OPLog("Emitter set\n");
        OPLog("Emitter proc'd\n");
        OPaudPlay();
	
	//OPaudSetPlayer(&player);
	//OPaudPlayerPlay();

	OPLog("Main: 1");
	//SoundEmitter = new OPSoundEmitter(&Sound, 8);
	//SoundEmitter->SetVolume(0.95f);
	OPLog("Main: 2");
	//SoundEmitter->Looping = true;
	//SoundEmitter->Play();
	OPLog("Main: 3");
	return;
}

// Step
#ifdef OPIFEX_ANDROID


void Purchase( JNIEnv * env, const char* item ) {

	jstring jstr = env->NewStringUTF(item);
    jclass clazz = env->FindClass( "com/opifex/smrf/GL2JNILib" );

	jmethodID midCallBackStatic = env->GetStaticMethodID(clazz, "Purchase", "(Ljava/lang/String;)Ljava/lang/String;");
   	if (NULL != midCallBackStatic) {
   		jobject resultJNIStr = env->CallStaticObjectMethod(clazz, midCallBackStatic, jstr);
   		jstring str = (jstring)resultJNIStr;
   		jboolean isCopy;
   		const char *resultCStr = env->GetStringUTFChars(str, &isCopy);
   		if (NULL != resultCStr) {
		   OPLog("In C, the returned string is %s\n", resultCStr);
		   if(isCopy != JNI_FALSE) {
			   env->ReleaseStringUTFChars(str, resultCStr);
			}
   		} else {
   			OPLog("Purchase: Failed to get Result");
   		}
   	} else {
   		OPLog("Purchase:Failed to find method");
   	}
}


JNIEXPORT int JNICALL Java_com_opifex_smrf_GL2JNILib_step(JNIEnv * env, jobject obj){	

	Purchase(env, "This comes from jni.");

	OPtimerTick(timer);
	//GamePadSystem::Update(env);
	GPS->Update(env);

#else
void Update( OPtimer* timer){
	GPS->Update();
#endif

	bool result = GM->Update( timer );


	GamePadState* gps = GPS->Controller(GamePadIndex_One);
	if(gps->IsConnected()) {

		OPfloat r = gps->IsDown(GamePad_Button_A) ? 1.0f : 0.0f;
		OPfloat g = gps->LeftThumbX() / 2.0f + 0.5f;
		OPfloat b = gps->LeftTrigger();

		if(gps->WasPressed(GamePad_Button_A)){
			r = g = b = 1.0f;
		}

		RenderSystem::ClearColor(r, g, b);

		#ifdef OPIFEX_ANDROID
	  	if(gps->WasPressed(GamePad_Button_A)){
	  		OPaudSetPlayer(&player);
	  		OPaudPlayerPlay();
	  	}
		#endif
	}
	else {
		RenderSystem::ClearColor(0,0,0);
	}


	OPaudSetPlayer(&player);
	OPaudPlayerUpdate(OPaudProcess);

    OPaudSetEmitter(&Emitter);
    OPaudUpdate(OPaudProcess);
	//SoundEmitter->Update();
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

	OPint a = 0;
	OPLog("Test %d %d %d", a, a, a);

	ActiveState = OPgameStateCreate(NULL, Update, NULL);

	OPstart();
	OPend();
	return 0;
}
#endif
