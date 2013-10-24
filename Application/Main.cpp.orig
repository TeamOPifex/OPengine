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
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#endif
#include "./Data/include/OPfile.h"
#include "./Human/include/Audio/OPaudio2.h"
#include "./Human/include/Audio/OPaudioEmitter.h"
#include "./Human/include/Audio/OPaudioPlayer.h"
#include "./Human/include/Resources/Texture/ImagePNG.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPcontentManager.h"
#include "./Data/include/OPentHeap.h"

GameManager* GM;
GamePadSystem* GPS;
OPaudioSource *Sound, *Sound1;
OPaudioEmitter Emitter, Emitter1;
OPaudioPlayer player;
OPentHeap* ents;
void* entData;

OPfloat vol = 0.05f;

void KeyDown(int key, int action){
	OPLog("Pizza");
	OPaudSetPlayer(&player);
	OPaudPlayerPlay();
}

// Initialize
void Init(){

#ifdef OPIFEX_ANDROID
	OPfileInit( AAssetManager_fromJava(JNIEnvironment(), JNIAssetManager()));
	i32 width = JNIWidth();
	i32 height = JNIHeight();
#else
	#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
		_chdir("assets\\");
	#else
		if(chdir("./assets")){
			OPLog("Directory changed!\n");
		}
		else
			OPLog("Directory change failed!!!\n");
	#endif

		i32 width = 1280;
		i32 height = 720;
#endif


	OPLog("Allocating");
	OPlinkedList* ll = OPllCreate();
	OPminHeap* heap = OPminHeapCreate(20);
	OPlist* list = OPlistCreate(5, sizeof(OPint));

	write(1, "Creating EntHeap...\n", 21);
	entData = OPalloc(OPentHeapSize(sizeof(OPint), 100));
	ents = OPentHeapCreate(entData, sizeof(OPint), 100);
OPLog("Created EntHeap!");
	OPint* ints = (OPint*)ents->Entities;
	for(OPint i = 100; i--;){
		OPint ind = 0;
		OPentHeapActivate(ents, &ind);
		OPLog("ind: %d", ind);
		ints[ind] = i;
	}

	OPLog("Max: %d\n", ents->MaxIndex);
	for(OPint i = 0; i < ents->MaxIndex; i++){
		OPLog_i32(ints[i]);
	}

	OPassetLoader loaders[] ={
		{
			".wav",
			"Audio/",
			sizeof(OPaudioSource),
			(OPint (*)(const OPchar*, void**))OPaudOpenWave,
			(OPint (*)(void*))OPaudCloseWave
		},
		{
			".ogg",
			"Audio/",
			sizeof(OPaudioSource),
			(OPint (*)(const OPchar*, void**))OPaudOpenOgg,
			(OPint (*)(void*))OPaudCloseOgg
		},
		{
			".png",
			"Textures/",
			sizeof(Texture2D),
			(OPint (*)(const OPchar*, void**))OPimagePNGLoad,
			(OPint (*)(void*))OPimagePNGUnload
		}
	};

	OPcmanInit(loaders, 3);

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

	OPaudInit();
	//Sound = OPaudOpenOgg("Audio/background.ogg");
	RenderSystem::Initialize(width, height);

	OPcmanLoad("pew.wav");
	OPcmanLoad("background.ogg");
	OPcmanLoad("test.png");

	GM = new GameManager(width, height);


#ifndef OPIFEX_ANDROID//defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	  	glfwSetKeyCallback(KeyDown);
#endif

        OPLog("Main: Song loading...");
        OPchar songPath[] = {"Audio/background.ogg"};
        //Song = OPAudio::ReadOgg(songPath);

        OPLog("Main: Song loaded");

		Sound1 = (OPaudioSource*)OPcmanGet("pew.wav");
		Sound = (OPaudioSource*)OPcmanGet("background.ogg");

		//OPcmanDelete("pew.wav");
		//OPcmanDelete("background.ogg");
		//OPcmanPurge();

        OPLog("Reading done!\n");
        Emitter1 = OPaudCreateEmitter(Sound1, 0);
        player = OPaudPlayerCreate(Sound1, 5, 0);
        Emitter = OPaudCreateEmitter(Sound, 1);
        OPLog("Emitter created\n");

		OPaudSetEmitter(&Emitter);
        OPaudVolume(0.05f);
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




void Update( OPtimer* timer){
	GPS->Update();

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

		if(gps->WasPressed(GamePad_Button_A)){
	  		OPaudSetPlayer(&player);
	  		OPaudPlayerPlay();
	  	}
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
	
	if(!result)
		exit(0);
	return;
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

extern "C" {
	JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_main(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_main(JNIEnv * env, jobject obj){
	OPinitialize = Init;
	OPupdate = UpdateState;
	OPdestroy = Destroy;
	ActiveState = OPgameStateCreate(NULL, Update, NULL);
}
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
