#include "ExampleSelectorState.h"
#include "EmptyState.h"
#include "FontTest.h"
#include "./Scripting/include/OPloaderOPS.h"
#include "./Scripting/include/JavaScript/OPjavaScriptV8.h"
#include "./Pipeline/include/Loaders/OPloaderOPskeleton.h"
#include "./Pipeline/include/Loaders/OPloaderOPanimation.h"
#include "./Data/include/OPlogToFile.h"

//////////////////////////////////////
// Application Methods
//////////////////////////////////////

void ApplicationInit() {

	//OPlogToFile(".opengine.debug.txt");

    OPlog("Size ui8: %d", sizeof(ui8));
    OPlog("Size ui16: %d", sizeof(ui16));
    OPlog("Size ui32: %d", sizeof(ui32));
    OPlog("Size i8: %d", sizeof(i8));
    OPlog("Size i16: %d", sizeof(i16));
    OPlog("Size i32: %d", sizeof(i32));


	OPloadersAddDefault();
	//OPscriptAddLoader();
	OPskeletonAddLoader();
	OPskeletonAnimationAddLoader();
	//SpineAddLoader();
	OPlog("Assets %s", OPIFEX_ASSETS);
	OPcmanInit(OPIFEX_ASSETS);

	OPcmanLoadResourcePack("pack.oppack");

	OPoculusStartup();
	OPrenderInit();
	OPgamePadSetDeadZones(0.2f);

	OPgameStateChange(&GS_EXAMPLE_SELECTOR);
	//OPgameStateChange(&GS_EXAMPLE_MODEL);
}

int ApplicationUpdate(OPtimer* timer) {

	OPinputSystemUpdate(timer);
	OPcmanUpdate(timer);

	if (OPkeyboardWasReleased(OPKEY_ESCAPE)) return 1;
	if ((OPkeyboardWasReleased(OPKEY_BACKSPACE) || OPgamePadWasPressed(OPgamePadGet(OPGAMEPAD_ONE), OPGAMEPADBUTTON_BACK)) && ActiveState != &GS_EXAMPLE_SELECTOR) {
		OPgameStateChange(&GS_EXAMPLE_SELECTOR);
	}

	return ActiveState->Update(timer);
}

void ApplicationRender(OPfloat delta) {
	//OPlog("[%f]", delta);
	ActiveState->Render(delta);
}

void ApplicationDestroy() {
	ActiveState->Exit(ActiveState);
	OPcmanDestroy();
}

void ApplicationSetup() {
	OPinitialize = ApplicationInit;
	OPupdate = ApplicationUpdate;
	OPrender = ApplicationRender;
	OPdestroy = ApplicationDestroy;
}

//////////////////////////////////////
// Application Entry Point
//////////////////////////////////////

#ifndef OPIFEX_IOS
OP_MAIN {
	#ifdef OPIFEX_OPTION_V8
	// If the V8 engine is compiled in,
	// see if we have a script to run at startup
	if(argc > 1) {
		//chdir(OPIFEX_ASSETS);
		OPjavaScriptV8SetupRun(args[2]);
		OP_MAIN_SUCCESS
	}
	#endif

	ApplicationSetup();

	OP_MAIN_START
	//OP_MAIN_START_STEPPED
	OP_MAIN_END
	OP_MAIN_SUCCESS
}
#endif
