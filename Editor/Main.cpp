#include "./OPengine.h"
#include "./EditorState.h"
#include "./Scripting/include/OPloaderOPS.h"
#include "./Scripting/include/JavaScript/OPjavaScriptV8.h"
#include "./Pipeline/include/Loaders/OPloaderOPskeleton.h"
#include "./Pipeline/include/Loaders/OPloaderOPanimation.h"
#include "./Data/include/OPlogToFile.h"
//#include "./OPassimp.h"
#include "./OPcef.h"

#include <bitset>
#include <string>
#include <jansson.h>

//////////////////////////////////////
// Application Methods
//////////////////////////////////////
/* forward refs */

OPwindow mainWindow;

void ApplicationInit() {
	//OPallocator* allocator = OPallocatorLinearCreate(MB(128));
	//OPDEFAULT_ALLOCATOR = *allocator;

	OPLOGLEVEL = 2000;

	OPCMAN.Init(OPIFEX_ASSETS);
	OPloadersAddDefault();
	OPscriptAddLoader();
	OPskeletonAddLoader();
	OPskeletonAnimationAddLoader();
	SpineAddLoader();
	//OPassimpAddLoaders();
	OPlogInfo("Assets %s", OPIFEX_ASSETS);

	OPoculusStartup();

	OPrenderSetup();
	OPwindowSystemInit();
	mainWindow.Init(NULL, OPwindowParameters("Main Window", false, 1920, 1080));
	OPrenderInit(&mainWindow);
	OPGAMEPADS.SetDeadzones(0.2f);

	OPcefInit();
	OPcefLoad("http://threejs.org");
	//OPcefLoad("about:blank");
	//OPchar* baseDir = OPstringCreateMerged("file:///", OPIFEX_ASSETS);
	//OPcefLoad(OPstringCreateMerged(baseDir, "index.html"));
	//OPcefLoad("file:///Web/index.html");

	OPgameState::Change(&GS_EDITOR);
}

OPint ApplicationUpdate(OPtimer* timer) {

	OPinputSystemUpdate(timer);
	OPCMAN.Update(timer);

	if (OPKEYBOARD.WasReleased(OPkeyboardKey::ESCAPE)) return 1;

	return ActiveState->Update(timer);
}

void ApplicationRender(OPfloat delta) {
	// OPlog("[%f]", delta);
	ActiveState->Render(delta);
}

void ApplicationDestroy() {
	ActiveState->Exit(ActiveState);
	OPCMAN.Destroy();
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
//if(argc > 1) {
//	//chdir(OPIFEX_ASSETS);
//	OPjavaScriptV8SetupRun(args[2]);
//	return 0;
//}
#endif

ApplicationSetup();

OP_MAIN_RUN
//OP_MAIN_RUN_STEPPED

OP_MAIN_END
#endif
