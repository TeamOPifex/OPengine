#include "ExampleSelectorState.h"

#ifdef ADDON_assimp
#include "./OPassimp.h"
#endif

//////////////////////////////////////
// Application Methods
//////////////////////////////////////

void ApplicationInit() {

	OP_LOG_LEVEL = 2000;

	OPloadersAddDefault();
	OPscriptAddLoader();
	OPskeletonAddLoader();
	OPskeletonAnimationAddLoader();
	SpineAddLoader();
#ifdef ADDON_assimp
	OPassimpAddLoaders();
#endif
	OPlog("Assets %s", OPIFEX_ASSETS);
	OPcmanInit(OPIFEX_ASSETS);

	OPoculusStartup();
	OPrenderInit();

	OPrenderCreateWindow(NULL, false, true, "Main Window", 1280, 720);
	
	OPGAMEPADSYSTEM.SetDeadzones(0.2f);	

	OPgameStateChange(&GS_EXAMPLE_SELECTOR);
}

OPint ApplicationUpdate(OPtimer* timer) {
	OPrenderUpdate();

	OPinputSystemUpdate(timer);
	OPcmanUpdate(timer);

	if (OPkeyboardWasReleased(OPKEY_ESCAPE)) return 1;
	if ((OPkeyboardWasReleased(OPKEY_BACKSPACE) || OPgamePadGet(OPGAMEPAD_ONE)->WasPressed(OPGAMEPADBUTTON_BACK)) && ActiveState != &GS_EXAMPLE_SELECTOR) {
		OPgameStateChange(&GS_EXAMPLE_SELECTOR);
	}

	return ActiveState->Update(timer);
}

void ApplicationRender(OPfloat delta) {
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

#ifdef OPIFEX_IOS
#import <UIKit/UIKit.h>
#import "./Human/include/Rendering/AppDelegate.h"

int main(int argc, char * argv[]) {
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}

#else

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

	//OP_MAIN_START
	OP_MAIN_START_STEPPED
	OP_MAIN_END
	OP_MAIN_SUCCESS
}
#endif
