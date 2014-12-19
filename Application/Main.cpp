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

	//OPfile f;
	// f = OPfileOpen("/Users/garretthoofman/engine/test.txt");
	// if(f._handle != 0) {
	// 	OPfileWritei32(&f, 1337);
	// 	OPfileClose(&f);
	// 	OPlog("Finished file");
	// } else {
	// 	OPlog("Failed to create file.");
	// 	return;
	// }

	// f = OPfileOpen("/Users/garretthoofman/engine/test.txt");
	// if(f._handle != 0) {
	// 	i32 number = OPfileReadi32(&f);
	// 	OPfileClose(&f);
	// 	OPlog("Read number: %d", number);
	// 	return;
	// } else {
	// 	OPlog("Failed to open file.");
	// 	return;
	// }

	OPmat4 a, b, c;
	OPmat4buildScl(&a, 2, 2, 2);
	OPmat4buildTranslate(&b, 2, 2, 2);
	OPmat4mul(&c, &b, &a);
	OPmat4Log("Test", &c);

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
