#include "ExampleSelectorState.h"

#ifdef ADDON_assimp
#include "./OPassimp.h"
#endif

#ifdef ADDON_libjpeg_turbo
#include "OPlibjpegturbo.h"
#endif

//////////////////////////////////////
// Application Methods
//////////////////////////////////////

OPwindow mainWindow;

void ApplicationInit() {
	TIMED_BLOCK;

	//OPlogToFile("output.txt");

	OPCMAN.Init(OPIFEX_ASSETS);

	OPloadersAddDefault();
	OPscriptAddLoader();
	OPskeletonAddLoader();
	OPskeletonAnimationAddLoader();
	SpineAddLoader();
#ifdef ADDON_assimp
	OPassimpAddLoaders();
#endif

#ifdef ADDON_libjpeg_turbo
	OPCMAN.AddLoader(&OPASSETLOADER_JPG);
#endif

	OPoculusStartup();

	OPrenderSetup();
	OPwindowSystemInit();
	mainWindow.Init(NULL, OPwindowParameters("Main Window", false, 1920, 1080));
	OPrenderInit(&mainWindow);
	
	OPGAMEPADS.SetDeadzones(0.2f);

	OPVISUALDEBUGINFO.Init();

	OPgameState::Change(&GS_EXAMPLE_SELECTOR);
}

OPint ApplicationUpdate(OPtimer* timer) {
	if (mainWindow.Update()) {
		return 1;
	}
	OPVISUALDEBUGINFO.Update(timer);

	OPinputSystemUpdate(timer);
	OPCMAN_UPDATE(timer);

	if (OPKEYBOARD.WasReleased(OPkeyboardKey::ESCAPE)) return 1;
	if ((OPKEYBOARD.WasReleased(OPkeyboardKey::BACKSPACE) || OPGAMEPADS[0]->WasPressed(OPgamePadButton::BACK)) && ActiveState != &GS_EXAMPLE_SELECTOR) {
		OPgameState::Change(&GS_EXAMPLE_SELECTOR);
	}

	return ActiveState->Update(timer);
}

void ApplicationRender(OPfloat delta) {
	ActiveState->Render(delta);
}

void ApplicationDestroy() {
	ActiveState->Exit(ActiveState);
	OPVISUALDEBUGINFO.Destroy();
	OPCMAN.Destroy();
	OPlogToFileClose();
	mainWindow.Destroy();
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
	OPLOGLEVEL = (ui32)OPlogLevel::VERBOSE;

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
