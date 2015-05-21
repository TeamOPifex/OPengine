#include "ExampleSelectorState.h"
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
	OPgamePadSetDeadzones(0.2f);

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
	if(argc > 1) {
		OPjavaScriptV8Init();
		chdir(OPIFEX_ASSETS);
		OPscript *result = NULL;
		OPscriptLoad(args[2], &result);
		OPjavaScriptV8Compiled compiled;
		OPjavaScriptV8Compile(&compiled, result);
		OPjavaScriptV8Run(&compiled);
		OP_MAIN_SUCCESS
	}

	#endif
	ApplicationSetup();

	OP_MAIN_START
	OP_MAIN_END
	OP_MAIN_SUCCESS
}