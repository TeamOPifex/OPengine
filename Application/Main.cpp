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
	OPwebServerQueue(server, "queued", "tested", 6);
}

// Initialize
void Init(){
	OPcmanInit(OP_DEFAULT_LOADERS, 9);

	OPaudInit();
	OPaudInitThread(10);

	server = OPwebServerCreate("8080");
	OPwebServerOnKey(server, "test", MsgHandler, NULL);

#ifndef OPIFEX_ANDROID
	OPrenderInit(width, height, false);
#else
	OPrenderInit(JNIWidth(), JNIHeight(), true);
#endif

	OPgameStateChange(&State0);

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
	if (OPkeyboardWasReleased(OPKEY_SPACE)) {
		OPwebServerQueue(server, "queued", "tested", 6);
		OPwebServerQueue(server, "another", "tested", 6);
		OPwebServerQueue(server, "death", "tested", 6);
	}
	OPwebServerUpdate(server);
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
	int main() {
#endif
 	OPmyoConnect();
	OPscriptInit();
	OPscriptLog("TEST LOGGING!!!");


	OPinitialize = Init;
	OPupdate = UpdateState;
	OPdestroy = Destroy;


	//ActiveState = &State0;//OPgameStateCreate(NULL, Update, NULL);

#ifdef OPIFEX_ANDROID
	return;
#else
	OPstart();
	OPend();
	return 0;
#endif
}
