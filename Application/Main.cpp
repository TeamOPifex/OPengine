#include "ExampleSelectorState.h"
#include "./Pipeline/include/SpineLoader.h"

//////////////////////////////////////
// Application Methods
//////////////////////////////////////

void ApplicationInit() {

#ifdef OPIFEX_REPO
	const OPchar* assetDir = OPIFEX_REPO;
#else
	const OPchar* assetDir = NULL;
#endif
	
	OPlog("Asset Dir: %s", assetDir);

	OPloadersAddDefault();
	SpineAddLoader();
	OPcmanInit(assetDir);

	OPrenderInit();

	OPcmanLoad("untitled.opm");
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

OP_MAIN {
	OPlog("Starting up OPifex Engine");

	OPinitialize = ApplicationInit;
	OPupdate = ApplicationUpdate;
	OPdestroy = ApplicationDestroy;

	OP_MAIN_START
	OP_MAIN_END
	OP_MAIN_SUCCESS
}