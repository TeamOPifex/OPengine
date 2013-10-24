//////////////////////////////// OPEngine MAIN ////////////////////////////////
#define WIN32_LEAN_AND_MEAN 0
#include <stdio.h>
#include <stdlib.h>

#include "./Core/include/Core.h"
#include "./Data/include/OPgameStates.h"

//#include "./GameManager.h"
#include "./Human/include/Rendering/Renderer.h"
#include "./Human/include/Input/GamePadSystem.h"

#include "./Core/include/Log.h"

#include "./Data/include/OPlinkedList.h"
#include "./Data/include/OPheap.h"
#include "./Data/include/OPlist.h"

#include "./Performance/include/OPthread.h"

#if defined(OPIFEX_ANDROID)
#include <jni.h>
#elif defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
#include <direct.h>
#include <GL/glew.h>
#include <GL/glfw.h>
//#include <glm/glm.hpp>
#elif defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
#include <GL/glew.h>
#include <GL/glfw.h>
//#include <glm/glm.hpp>
#endif

#include "./Data/include/OPfile.h"
#include "./Human/include/Audio/OPaudio2.h"
#include "./Human/include/Audio/OPaudioEmitter.h"
#include "./Human/include/Audio/OPaudioPlayer.h"
#include "./Human/include/Utilities/OPMloader.h"
#include "./Human/include/Resources/Texture/ImagePNG.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPcontentManager.h"
#include "./Data/include/OPentHeap.h"

#include "GameStates.h"

//GameManager* GM;
OPaudioSource *Sound, *Sound1, *Sound2;
OPaudioEmitter *Emitter;
OPaudioPlayer player, player1;
OPentHeap* ents;
void* entData;

OPfloat vol = 0.05f;

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
	}
};

void KeyDown(int key, int action){
	OPLog("Pizza %d", key);

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

// Initialize
void Init(){
	#if defined(OPIFEX_WIN32) || defined(OPIFEX_WIN64)
	_chdir("assets\\");
	#else
	if(chdir("./assets")){
		OPLog("Directory changed!\n");
	}
	else
		OPLog("Directory change failed!!!\n");
	#endif


#ifndef OPIFEX_ANDROID
	i32 width = 640;
	i32 height = 480;
#endif

	OPcmanInit(loaders, 6);

	OPaudInit();
	OPaudInitThread(10);

#ifndef OPIFEX_ANDROID
	OPrenderInit(width, height);
#else
	OPrenderInit(JNIWidth(), JNIHeight());
#endif

	OPgameStateChange(&State0);

#ifndef OPIFEX_ANDROID//defined(OPIFEX_LINUX32) || defined(OPIFEX_LINUX64)
	  	glfwSetKeyCallback(KeyDown);
#endif

        OPLog("Main: Song loading...");
        OPchar songPath[] = {"Audio/background.ogg"};
        //Song = OPAudio::ReadOgg(songPath);

        OPLog("Main: Song loaded");

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

		void* test = OPalloc(OPentHeapSize(4, 10));
		OPentHeap* heap = OPentHeapCreate(test, 4, 10);

		//for(OPint i =10; i--;){
		//	OPint ind = 0;
		//	OPentHeapActivate(heap, &ind);
		//	printf("%d @ %d\n", i, ind);
		//}
		//printf("\n");

		for(OPint j = 4; j--;){
			OPint ind = 0;
			OPentHeapActivate(heap, &ind);
			while(ind >= 0){
				printf("Act %d\n", ind);
				OPentHeapActivate(heap, &ind);
			}
			printf(">>>Max %d\n", heap->MaxIndex);
			for(OPint i =5; i--;){
				OPuint ind = (OPuint)OPrandRange(0, 10);
				OPentHeapKill(heap, ind);
				printf("free @ %d\n", ind);
			}
			printf(">>>Max %d\n\n", heap->MaxIndex);
		}
/*
		Sound1 = (OPaudioSource*)OPcmanGet("impact.wav");
		Sound2 = (OPaudioSource*)OPcmanGet("boom.wav");
		Sound = (OPaudioSource*)OPcmanGet("background.ogg");

        OPLog("Reading done!\n");
        player = OPaudPlayerCreate(Sound1, 5, 0);
        player1 = OPaudPlayerCreate(Sound2, 4, 0);
		Emitter = OPaudCreateEmitter(Sound, EMITTER_THREADED | EMITTER_LOOPING);
		OPaudRecycleEmitter(Emitter);
		Emitter = OPaudGetEmitter(Sound, EMITTER_LOOPING);
        OPLog("Emitter created\n");

		OPaudSetEmitter(Emitter);
        OPaudVolume(0.05f);
        OPLog("Emitter set\n");
        OPLog("Emitter proc'd\n");
        OPaudPlay();
		*/
	return;
}

bool Update( OPtimer* timer){
	
	GamePadController* gamePad = OPgamePadController(GamePadIndex_One);
	OPgamePadUpdate(gamePad);
	if(OPgamePadIsConnected(gamePad) && OPgamePadWasPressed(gamePad, GamePad_Button_START)){
		return true;
	}

	if(OPgamePadIsConnected(gamePad) && OPgamePadWasPressed(gamePad, GamePad_Button_A)){
		OPLog("Playing Audio");
	  	OPaudSetPlayer(&player);
	  	OPaudPlayerPlay();
	}

	OPrenderPresent();
	
	return false;
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


#ifdef OPIFEX_ANDROID
extern "C" {
	JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_start(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_start(JNIEnv * env, jobject obj) {
#else
	int main() {
#endif
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

	OPinitialize = Init;
	OPupdate = UpdateState;
	OPdestroy = Destroy;

	OPint a = 0;
	OPLog("Test %d %d %d", a, a, a);
	//ActiveState = &State0;//OPgameStateCreate(NULL, Update, NULL);

#ifdef OPIFEX_ANDROID
	return;
#else
	OPstart();
	OPend();
	return 0;
#endif
}
