//////////////////////////////// OPEngine MAIN ////////////////////////////////
#define WIN32_LEAN_AND_MEAN 0
#include <stdio.h>
#include <stdlib.h>

#include "./Core/include/Core.h"
#include "./Data/include/GameStates.h"

#include "./Application/Examples/Audio.h"
#include "./Application/Examples/Model.h"
#include "./Application/Examples/Oculus.h"
#include "./Application/Examples/ParticleSystem.h"
#include "./Application/Examples/Physics.h"
#include "./Application/Examples/Skinning.h"
#include "./Application/ExampleSelectorState.h"

//#include "./GameManager.h"
#include "./Human/include/Rendering/Renderer.h"
#include "./Human/include/Input/GamePadSystem.h"

#include "./Core/include/Log.h"

#include "./Data/include/LinkedList.h"
#include "./Data/include/Heap.h"
#include "./Data/include/List.h"

#include "./Performance/include/OPthread.h"

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
#include "./Data/include/File.h"
#include "./Data/include/ContentManager.h"
#include "./Data/include/EntHeap.h"
#include "GameStates.h"
#include "./Human/include/Human.h"
#include "./Scripting/include/Scripting.h"
#include "./Pipeline/include/DefaultLoaders.h"

#include "GameWebServer.h"

OPWebServer* server = NULL;

OPentHeap* ents;
void* entData;

OPfloat vol = 0.05f;

#ifndef OPIFEX_ANDROID
i32 width = 1280;
i32 height = 720;
#endif

void MsgHandler(OPstream* str, void* param) {
	OPlog("Hit the handler!");
	//i8* test = "tested";
	//OPwebServerQueue(server, "queued", test, 6);
}

// Initialize
void Init(){
#ifdef OPIFEX_REPO
	OPcmanInit(OP_DEFAULT_LOADERS, 10, OPIFEX_REPO);
#else
	OPcmanInit(OP_DEFAULT_LOADERS, 10, NULL);
#endif
				//OPscriptInit();
				//OPcmanLoad("app.ops");
		//OPstream* stream = OPreadFile("Scripts/app.ops");
		//	OPlog("File Read");
		//OPscriptCompileAndRunStream(stream);

	//OPaudInit();
	//OPaudInitThread(10);

	//server = OPwebServerCreate("8080");
	//OPwebServerOnKey(server, "test", MsgHandler, NULL);


	//OPoculusStartup();
	//OPoculusInitialize();

#ifndef OPIFEX_ANDROID
	OPrenderInit(width, height, false);
#else
	OPrenderInit(JNIWidth(), JNIHeight(), true);
#endif
	//OPgameStateChange(&State0);
	//OPgameStateChange(&GS_EXAMPLE_AUDIO);
	//OPgameStateChange(&GS_EXAMPLE_MODEL);
	//OPgameStateChange(&GS_EXAMPLE_PARTICLESYSTEM);
	//OPgameStateChange(&GS_EXAMPLE_PHYSICS);
	//OPgameStateChange(&GS_EXAMPLE_SKINNING);
	//OPgameStateChange(&GS_EXAMPLE_OCULUS);
	OPgameStateChange(&GS_EXAMPLE_SELECTOR);
	
	return;
}

int Update( OPtimer* timer){	
	return 0;
}

void Destroy()
{
	ActiveState->OnExit(ActiveState);
	return;
}


int UpdateState(OPtimer* timer){
	OPkeyboardUpdate();
	OPmouseUpdate();
	OPgamePadSystemUpdate();

	if (OPkeyboardWasReleased(OPKEY_ESCAPE)) {
		return 1;
	}
	if (OPkeyboardWasReleased(OPKEY_BACKSPACE)) {
		OPgameStateChange(&GS_EXAMPLE_SELECTOR);
	}
	return ActiveState->Update(timer);	
}



#include "./Math/include/Tweening.h"
#include "./Core/include/Assert.h"
#include "./Human/include/Rendering/Video.h"

#ifdef OPIFEX_ANDROID
extern "C" {
	JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_start(JNIEnv * env, jobject obj);
};
JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_start(JNIEnv * env, jobject obj) {
#else
int main(int argc, char** args) {
	if (argc > 2) {
		ui32 arg2len = strlen(args[1]);
		const i8* p = "-script";
		ui32 plen = strlen(p);
		if (arg2len == plen){
			i32 match = OPmemcmp(args[1], p, arg2len);
			if (match == 0) {
				i8* script = args[2];

				OPscriptInit();
				OPstream* stream = OPreadFile(script);
				OPscriptCompileAndRunStream(stream);
			}
		}
	}
	else {
#endif

		OPinitialize = Init;
		OPupdate = UpdateState;
		OPdestroy = Destroy;

		ActiveState = OPgameStateCreate(NULL, Update, NULL);

#ifdef OPIFEX_ANDROID
		return;
#else
		OPstart();
		OPend();

	}
	return 0;

#endif
}
