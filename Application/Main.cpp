//////////////////////////////// OPEngine MAIN ////////////////////////////////
#define WIN32_LEAN_AND_MEAN 0
#include <stdio.h>
#include <stdlib.h>

#include "./Core/include/Core.h"
#include "./Data/include/GameStates.h"

//#include "./GameManager.h"
#include "./Human/include/Rendering/Renderer.h"
#include "./Human/include/Input/GamePadSystem.h"

#include "./Core/include/Log.h"

#include "./Data/include/LinkedList.h"
#include "./Data/include/Heap.h"
#include "./Data/include/List.h"

#include "./Performance/include/OPthread.h"
#include "./Human/include/Input/Oculus.h"

#if defined(OPIFEX_ANDROID)
#include <jni.h>
#elif defined(OPIFEX_WINDOWS)
#include <direct.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
#endif

#include "./Data/include/File.h"
#include "./Human/include/Audio/Audio.h"
#include "./Human/include/Audio/AudioEmitter.h"
#include "./Human/include/Audio/AudioPlayer.h"
#include "./Human/include/Utilities/LoaderOPM.h"
#include "./Human/include/Utilities/ImagePNG.h"
#include "./Human/include/Rendering/Effect.h"
#include "./Data/include/File.h"
#include "./Data/include/ContentManager.h"
#include "./Data/include/EntHeap.h"
#include "./Human/include/Rendering/Font.h"
#include "GameStates.h"

//GameManager* GM;
OPaudioSource *Sound, *Sound1, *Sound2;
OPaudioEmitter *Emitter;
OPaudioPlayer player, player1;
OPentHeap* ents;
void* entData;

OPfloat vol = 0.05f;

OPassetLoader loaders[] = {
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
		sizeof(OPtexture),
		(OPint (*)(const OPchar*, void**))OPimagePNGLoad,
		(OPint (*)(void*))OPimagePNGUnload
	},
	{
		".vert",
		"Shaders/",
		sizeof(OPshader),
		(OPint (*)(const OPchar*, void**))OPrenderLoadVertexShader,
		(OPint (*)(void*))OPrenderUnloadShader
	},
	{
		".frag",
		"Shaders/",
		sizeof(OPshader),
		(OPint (*)(const OPchar*, void**))OPrenderLoadFragmentShader,
		(OPint (*)(void*))OPrenderUnloadShader
	},
	{
		".opm",
		"Models/",
		sizeof(OPmesh),
		(OPint (*)(const OPchar*, void**))OPMload,
		(OPint (*)(void*))OPMUnload
	},
	{
		".opf",
		"Fonts/",
		sizeof(OPfont),
		(OPint(*)(const OPchar*, void**))OPfontLoad,
		(OPint(*)(void*))OPfontUnload
	}
};

void KeyDown(int key, int action){
	OPlog("Pizza %d", key);

	switch(key){
		case 65:
			OPaudSetPlayer(&player);
			OPaudPlayerPlay();
			break;
		case 83:
			OPaudSetPlayer(&player1);
			OPaudPlayerPlay();
			break;
	}

}

#ifndef OPIFEX_ANDROID
i32 width = 1280;
i32 height = 720;
#endif

// Initialize
void Init(){
	#if defined(OPIFEX_WINDOWS)
	_chdir("assets\\");
	#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64) || defined(OPIFEX_ANDROID)
		if(chdir("./assets")) OPlog("Directory changed!\n"); 
		else OPlog("Directory change failed!!!\n");
	#else
		if(chdir("./assets") == 0) OPlog("Directory changed!\n"); 
		else OPlog("Directory change failed!!!\n");
	#endif
	

	OPcmanInit(loaders, 7);

	OPaudInit();
	OPaudInitThread(10);

#ifndef OPIFEX_ANDROID
	OPrenderInit(width, height, false);
#else
	OPrenderInit(JNIWidth(), JNIHeight());
#endif

	OPgameStateChange(&State0);

#ifndef OPIFEX_ANDROID//defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	  	//glfwSetKeyCallback(window, KeyDown);
#endif

       // OPlog("Main: Song loading...");
        //OPchar songPath[] = {"Audio/background.ogg"};
        //Song = OPAudio::ReadOgg(songPath);

        OPlog("Main: Song loaded");

		//		OPminHeap* heap = OPminHeapCreate(10);

		//for(OPint j = 2; j--;){
		//for(OPint i =10; i--;){
		//	OPint ind = 0;
		//	//OPentHeapActivate(heap, &ind);
		//	OPminHeapPush(heap, i);
		//	printf("Pushing %d\n", i);
		//}
		//printf("\n");
		//for(OPint i =10; i--;){
		//	OPint ind = 0;
		//	//OPentHeapActivate(heap, &ind);
		//	printf("Freeing %d\n", OPminHeapPop(heap));
		//}
		//		printf("\n");
		//}
		//printf("Freeing\n");

		// void* test = OPalloc(OPentHeapSize(4, 10));
		// OPentHeap* heap = OPentHeapCreate(test, 4, 10);

		//for(OPint i =10; i--;){
		//	OPint ind = 0;
		//	OPentHeapActivate(heap, &ind);
		//	printf("%d @ %d\n", i, ind);
		//}
		//printf("\n");

		// for(OPint j = 4; j--;){
		// 	OPint ind = 0;
		// 	OPentHeapActivate(heap, &ind);
		// 	while(ind >= 0){
		// 		printf("Act %d\n", ind);
		// 		OPentHeapActivate(heap, &ind);
		// 	}
		// 	printf(">>>Max %d\n", heap->MaxIndex);
		// 	for(OPint i =5; i--;){
		// 		OPuint ind = (OPuint)OPrandRange(0, 10);
		// 		OPentHeapKill(heap, ind);
		// 		printf("free @ %d\n", ind);
		// 	}
		// 	printf(">>>Max %d\n\n", heap->MaxIndex);
		// }
/*
		Sound1 = (OPaudioSource*)OPcmanGet("impact.wav");
		Sound2 = (OPaudioSource*)OPcmanGet("boom.wav");
		Sound = (OPaudioSource*)OPcmanGet("background.ogg");

        OPlog("Reading done!\n");
        player = OPaudPlayerCreate(Sound1, 5, 0);
        player1 = OPaudPlayerCreate(Sound2, 4, 0);
		Emitter = OPaudCreateEmitter(Sound, EMITTER_THREADED | EMITTER_LOOPING);
		OPaudRecycleEmitter(Emitter);
		Emitter = OPaudGetEmitter(Sound, EMITTER_LOOPING);
        OPlog("Emitter created\n");

		OPaudSetEmitter(Emitter);
        OPaudVolume(0.05f);
        OPlog("Emitter set\n");
        OPlog("Emitter proc'd\n");
        OPaudPlay();
		*/
	return;
}

int Update( OPtimer* timer){
	
	// GamePadController* gamePad = OPgamePadController(GamePadIndex_One);
	// OPgamePadUpdate(gamePad);
	// if(OPgamePadIsConnected(gamePad) && OPgamePadWasPressed(gamePad, GamePad_Button_START)){
	// 	return true;
	// }

	// if(OPgamePadIsConnected(gamePad) && OPgamePadWasPressed(gamePad, GamePad_Button_A)){
	// 	OPlog("Playing Audio");
	//   	OPaudSetPlayer(&player);
	//   	OPaudPlayerPlay();
	// }

	
	return 0;
}

void Destroy()
{
	ActiveState->OnExit(ActiveState);
	return;
}

int UpdateState(OPtimer* timer){
	return ActiveState->Update(timer);	
}

#include "./Math/include/Tweening.h"
#include "./Core/include/Assert.h"

#ifdef OPIFEX_ANDROID
extern "C" {
	JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_start(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_start(JNIEnv * env, jobject obj) {
#else
	int main() {
#endif

	//ASSERT(false, "Test!");
	//ASSERT(true, "TEST!");

	//if (OPoculusInitialize()) {
	//	OPoculusUpdate();
	//	OPvec4 state = OPoculusHmd();
	//	OPvec2 screen = OPoculusScreenSize();
	//	width = screen.x;
	//	height = screen.y;
	//}

	//	OPstream* str;
	OPmat4 scl;
	OPmat4buildScl(&scl, 2.0f, 2.0f, 2.0f);
	OPvec3 test = { 1.0f, 1.5f, 2.0f };
	test *= scl;

	OPvec2 one = { 0.0, 1.5 };
	OPvec2 two = { 0.5, -0.5 };
	OPvec2 three = one + two;
	one = one * 2.0f;
	one = one * one;
	one += two;
	one *= two;
	one /= two;

	OPmat4 rot1, rot2;
	OPmat4buildRotX(&rot1, 1.0f);
	OPmat4buildRotY(&rot2, 1.0f);
	rot1 *= rot2;

	/*str = OPstreamCreate(sizeof(int) * 100);

    for(int i = 0; i < 100000; i++){
    	int oldSize = str->Length;

    	OPwrite(str, &i, sizeof(int));

    	if(oldSize != str->Length){
    	OPlog("Stream resized from %d to %d",oldSize,str->Length);
    	OPlog("@ %x",(OPuint)str->Data);
    	}
    }

    write(1, "It worked\n", 10);
	*/
	OPinitialize = Init;
	OPupdate = UpdateState;
	OPdestroy = Destroy;

	OPint a = 0;
	OPlog("Test %d %d %d", a, a, a);
	//ActiveState = &State0;//OPgameStateCreate(NULL, Update, NULL);

#ifdef OPIFEX_ANDROID
	return;
#else
	OPstart();
	OPend();
	return 0;
#endif
}
