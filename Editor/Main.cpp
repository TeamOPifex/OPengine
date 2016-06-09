#include "./OPengine.h"
#include "./EditorState.h"
#include "./Scripting/include/OPloaderOPS.h"
#include "./Scripting/include/JavaScript/OPjavaScriptV8.h"
#include "./Pipeline/include/Loaders/OPloaderOPskeleton.h"
#include "./Pipeline/include/Loaders/OPloaderOPanimation.h"
#include "./Data/include/OPlogToFile.h"
#include "./OPassimp.h"
#include "./OPcef.h"

#include <bitset>
#include <string>
#include <jansson.h>

//////////////////////////////////////
// Application Methods
//////////////////////////////////////
/* forward refs */

void ApplicationInit() {
	//OPallocator* allocator = OPallocatorLinearCreate(MB(128));
	//OPDEFAULT_ALLOCATOR = *allocator;

	OP_LOG_LEVEL = 2000;
	
	OPloadersAddDefault();
	OPscriptAddLoader();
	OPskeletonAddLoader();
	OPskeletonAnimationAddLoader();
	SpineAddLoader();
	OPassimpAddLoaders();
	OPlog("Assets %s", OPIFEX_ASSETS);
	OPcmanInit(OPIFEX_ASSETS);

	OPjson* ground = (OPjson*)OPcmanLoadGet("ground.meta");
	OPjson model = OPjsonGet(*ground, "model");
	OPlog("MODEL from JSON: %s", OPjsonString(model));

	OPoculusStartup();
	OPrenderInit();
	OPgamePadSetDeadZones(0.2f);

	OPcefInit();
	OPcefLoad("http://google.com");

	OPgameStateChange(&GS_EDITOR);
}

OPint ApplicationUpdate(OPtimer* timer) {
	OPrenderUpdate();

	OPinputSystemUpdate(timer);
	OPcmanUpdate(timer);

	if (OPkeyboardWasReleased(OPKEY_ESCAPE)) return 1;

	return ActiveState->Update(timer);
}

void ApplicationRender(OPfloat delta) {
	// OPlog("[%f]", delta);
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

OP_MAIN{
#ifdef OPIFEX_OPTION_V8
	// If the V8 engine is compiled in,
	// see if we have a script to run at startup
	if (argc > 1) {
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
