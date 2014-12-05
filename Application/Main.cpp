#include "ExampleSelectorState.h"

void InitializeLoaders() {
	OPloadersAddDefault();
}

//////////////////////////////////////
// Application Methods
//////////////////////////////////////

void ApplicationInit() {
	OPchar* assetDir = NULL;
#ifdef OPIFEX_REPO
	assetDir = OPstringCreateCopy(OPIFEX_REPO);
#endif

	InitializeLoaders();
	OPcmanInit(assetDir);

	OPvec3 a = OPvec3create(1,1,1);
	OPvec3 b = OPvec3create(2,2,2);
	OPvec3 c = OPvec3create(3,3,3);

	OPvec3 d = a - b * c;

	OPrenderInit();

	OPgameStateChange(&GS_EXAMPLE_SELECTOR);
}

int ApplicationUpdate(OPtimer* timer) {
	OPcmanUpdate();
	OPinputSystemUpdate();
	if (OPkeyboardWasReleased(OPKEY_ESCAPE)) return 1;	
	if (OPkeyboardWasReleased(OPKEY_BACKSPACE)) {
		OPgameStateChange(&GS_EXAMPLE_SELECTOR);
	}
	return ActiveState->Update(timer);
}

void ApplicationDestroy() {
	ActiveState->Exit(ActiveState);
}

void ApplicationSetup() {
	OPinitialize = ApplicationInit;
	OPupdate = ApplicationUpdate;
	OPdestroy = ApplicationDestroy;
}


//////////////////////////////////////
// Application Entry Point
//////////////////////////////////////

#ifdef OPIFEX_ANDROID

#include <jni.h>
extern "C" {
	JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_start(JNIEnv * env, jobject obj);
};
JNIEXPORT void JNICALL Java_com_opifex_GL2JNILib_start(JNIEnv * env, jobject obj) {
	ApplicationSetup();
}

#else

int main(int argc, char** args) {
	ApplicationSetup();

	OP_MAIN_START
	OP_MAIN_END
	OP_MAIN_SUCCESS
}

#endif
