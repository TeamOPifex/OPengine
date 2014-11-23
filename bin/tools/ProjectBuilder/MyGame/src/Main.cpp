#include "./Engine.h"
#include "./include/States/Sample.h"

//////////////////////////////////////
// Application Methods
//////////////////////////////////////

void ApplicationInit() {

	OPchar* assetDir = NULL;
#ifdef OPIFEX_REPO
	/* If in debug, we target the Assets dir in
	// the repository itself, otherwise it defaults
	// to looking the current directory for the Assets
	// dir. */
	assetDir = OPIFEX_REPO;
#endif

	OPloadersAddDefault();
	OPcmanInit(assetDir);

	OPaudInit();
	OPaudInitThread(100);

	OPrenderInit();
	OPsprite2DInit(NULL);
	OPsprite3DInit(NULL);

	OPsystemsLoadFontEffect();

	OPgamePadSetDeadzones(0.1f);
	OPmyoConnect();

	OPgameStateChange(&GS_SAMPLE);
}

int ApplicationUpdate(OPtimer* timer) {

	OPcmanUpdate();
	OPmyoUpdate(timer);
	OPinputSystemUpdate();

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
	OPlog("Starting up the OPengine");

	OPinitialize = ApplicationInit;
	OPupdate = ApplicationUpdate;
	OPdestroy = ApplicationDestroy;

	OP_MAIN_START
	OP_MAIN_END
	OP_MAIN_SUCCESS
}
