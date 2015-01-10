#include "ExampleSelectorState.h"
#include "./Scripting/include/OPloaderOPS.h"

void InitializeLoaders() {
	OPloadersAddDefault(); 
	OPscriptAddLoader();
}

//////////////////////////////////////
// Application Methods
//////////////////////////////////////

OPfile logFile;
void ApplicationInit() {
	//logFile = OPfileOpen("log.txt");
	//OPlogSetOutput(logFile._handle);

	OPlogDebug("App starting");

	const OPchar* assetDir = NULL;
#ifdef OPIFEX_ASSETS
	assetDir = OPIFEX_ASSETS;
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

	OPrenderInit();


	OPgameStateChange(&GS_EXAMPLE_SELECTOR);
}


int ApplicationUpdate(OPtimer* timer) {
		
	OPinputSystemUpdate(timer);
	OPcmanUpdate();

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

OP_MAIN {
	ApplicationSetup();

	OP_MAIN_START
	OP_MAIN_END
	OP_MAIN_SUCCESS
}