#include "ExampleSelectorState.h"

#ifdef ADDON_assimp
#include "./OPassimp.h"
#endif

//////////////////////////////////////
// Application Methods
//////////////////////////////////////

OPwindow* mainWindow;

void ApplicationInit() {

	OP_LOG_LEVEL = 2000;
	OPlogToFile("output.txt");
	OPlog("test");

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

	mainWindow = OPrenderCreateWindow(NULL, false, true, "Main Window", 1280, 720);
	
	OPrenderInitDevice(mainWindow);

	//OPcmanLoadGet("Tutorial02.vert");
	//OPcmanLoadGet("Tutorial02.frag");

	//OPGAMEPADSYSTEM.SetDeadzones(0.2f);	

	//OPgameStateChange(&GS_EXAMPLE_SELECTOR);
}

OPint ApplicationUpdate(OPtimer* timer) {
	if (mainWindow->Update()) {
		return 1;
	}
	//OPrenderUpdate();

	//OPinputSystemUpdate(timer);
	//OPcmanUpdate(timer);

	//if (OPkeyboardWasReleased(OPKEY_ESCAPE)) return 1;
	//if ((OPkeyboardWasReleased(OPKEY_BACKSPACE) || OPgamePadGet(OPGAMEPAD_ONE)->WasPressed(OPGAMEPADBUTTON_BACK)) && ActiveState != &GS_EXAMPLE_SELECTOR) {
	//	OPgameStateChange(&GS_EXAMPLE_SELECTOR);
	//}

	//return ActiveState->Update(timer);
	return 0;
}

void ApplicationRender(OPfloat delta) {
	//ActiveState->Render(delta);
	OPrenderClear(0, 0, 0.5f);
	OPrenderPresent();
}

void ApplicationDestroy() {
	//ActiveState->Exit(ActiveState);
	OPcmanDestroy();
	OPlogToFileClose();
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

OP_MAIN_START

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

	//OP_MAIN_RUN
	OP_MAIN_RUN_STEPPED

OP_MAIN_END
#endif
