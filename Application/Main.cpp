#include "ExampleSelectorState.h"
#include "./Scripting/include/OPloaderOPS.h"
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

	OPgameStateChange(&GS_EXAMPLE_SELECTOR);
}

int ApplicationUpdate(OPtimer* timer) {		
	OPinputSystemUpdate(timer);
	OPcmanUpdate(timer);

	if (OPkeyboardWasReleased(OPKEY_ESCAPE)) return 1;	
	if (OPkeyboardWasReleased(OPKEY_BACKSPACE) && ActiveState != &GS_EXAMPLE_SELECTOR) {
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