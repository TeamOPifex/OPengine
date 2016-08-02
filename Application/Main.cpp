#include "ExampleSelectorState.h"

#ifdef ADDON_assimp
#include "./OPassimp.h"
#endif

//////////////////////////////////////
// Application Methods
//////////////////////////////////////

OPwindow mainWindow;

void ApplicationInit() {

	OP_LOG_LEVEL = 2000;
	//OPlogToFile("output.txt");
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
	OPwindowSystemInit();
	mainWindow.Init(NULL, OPwindowParameters("Main Window", true, 1280, 720));
	
	OPGAMEPADSYSTEM.SetDeadzones(0.2f);	

	OPgameStateChange(&GS_EXAMPLE_SELECTOR);
}

OPint ApplicationUpdate(OPtimer* timer) {
	if (mainWindow.Update()) {
		return 1;
	}

	OPinputSystemUpdate(timer);
	OPcmanUpdate(timer);

	if (OPkeyboardWasReleased(OPkeyboardKey::ESCAPE)) return 1;
	if ((OPkeyboardWasReleased(OPkeyboardKey::BACKSPACE) || OPgamePadGet(OPGAMEPAD_ONE)->WasPressed(OPGAMEPADBUTTON_BACK)) && ActiveState != &GS_EXAMPLE_SELECTOR) {
		OPgameStateChange(&GS_EXAMPLE_SELECTOR);
	}

	return ActiveState->Update(timer);
}

void ApplicationRender(OPfloat delta) {
	ActiveState->Render(delta);
}

void ApplicationDestroy() {
	//ActiveState->Exit(ActiveState);
	OPcmanDestroy();
	OPlogToFileClose();
}


void RenderTestInit() {
	OPwindowSystemInit();
	mainWindow.Init(NULL, OPwindowParameters("Main Window", true, 1280, 720));
	OPrenderInit(OPrendererType::OPENGL);
}

OPint RenderTestUpdate(OPtimer* timer) {
	mainWindow.Update();
	OPrenderClear(0, 0, 0);
	OPrenderPresent();
	return 0;
}

void RenderTestRender(OPfloat delta) {

}

void RenderTestDestroy() {

}

void ApplicationSetup() {
	OPinitialize = ApplicationInit;
	OPupdate = ApplicationUpdate;
	OPrender = ApplicationRender;
	OPdestroy = ApplicationDestroy;

	//OPinitialize = RenderTestInit;
	//OPupdate = RenderTestUpdate;
	//OPrender = RenderTestRender;
	//OPdestroy = RenderTestDestroy;
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
		return 0;
	}
	#endif

	ApplicationSetup();

	//OP_MAIN_RUN
	OP_MAIN_RUN_STEPPED

OP_MAIN_END
#endif
