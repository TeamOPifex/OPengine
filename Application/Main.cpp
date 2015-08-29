#include "ExampleSelectorState.h"
#include "FontTest.h"
#include "./Scripting/include/OPloaderOPS.h"
#include "./Scripting/include/JavaScript/OPjavaScriptV8.h"
#include "./Pipeline/include/Loaders/OPloaderOPskeleton.h"
#include "./Pipeline/include/Loaders/OPloaderOPanimation.h"


//////////////////////////////////////
// Application Methods
//////////////////////////////////////

void ApplicationInit() {
	OPloadersAddDefault();
	OPscriptAddLoader();
	OPskeletonAddLoader();
	OPskeletonAnimationAddLoader();
	SpineAddLoader();
	OPcmanInit(OPIFEX_ASSETS);

	OPrenderInit();
	OPgamePadSetDeadZones(0.2f);

	OPgameStateChange(&GS_EXAMPLE_SELECTOR);
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

void ApplicationDestroy() {
	ActiveState->Exit(ActiveState);
	OPcmanDestroy();
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
	OP_MAIN_END
	OP_MAIN_SUCCESS
}
