#include "ExampleSelectorState.h"
#include "EmptyState.h"
#include "FontTest.h"
#include "./Scripting/include/OPloaderOPS.h"
#include "./Scripting/include/JavaScript/OPjavaScriptV8.h"
#include "./Pipeline/include/Loaders/OPloaderOPskeleton.h"
#include "./Pipeline/include/Loaders/OPloaderOPanimation.h"
#include "./Data/include/OPlogToFile.h"

#include <bitset>
#include <string>
#include <jansson.h>

//////////////////////////////////////
// Application Methods
//////////////////////////////////////
/* forward refs */

void ApplicationInit() {
    OPjson* root = OPjsonLoad("{ \"test\": \"Message\", \"Count\": 1337, \"State\": true }");

    if (root) {
        OPjson* test = OPjsonGet(root, "test");
        OPjson* count = OPjsonGet(root, "Count");
        OPjson* state = OPjsonGet(root, "State");
        OPlog("TEST: %s", OPjsonString(test));
        OPlog("Number: %d", OPjsonI64(count));
        OPlog("State: %d", OPjsonBool(state));
        OPjsonDestroy(root);
    } else {
        OPlog("FAILED to print JSON");
    }



	OP_LOG_LEVEL = 2000;
    // ui64 val = 4;
    // ui64 val2 = 8 << 3;
    // ui64 result = val | val2;

    // std::bitset<64> bitset1 { result };   // the bitset representation of 4
    // std::string str = bitset1.to_string();

    // OPlogDebug("Bitset %s", str.c_str());
    // exit(0);
    // return;

	//OPlogToFile(".opengine.debug.txt");

    OPlog("Size ui8: %d", sizeof(ui8));
    OPlog("Size ui16: %d", sizeof(ui16));
    OPlog("Size ui32: %d", sizeof(ui32));
    OPlog("Size i8: %d", sizeof(i8));
    OPlog("Size i16: %d", sizeof(i16));
    OPlog("Size i32: %d", sizeof(i32));


	OPloadersAddDefault();
	//OPscriptAddLoader();
	OPskeletonAddLoader();
	OPskeletonAnimationAddLoader();
	//SpineAddLoader();
	OPlog("Assets %s", OPIFEX_ASSETS);
	OPcmanInit(OPIFEX_ASSETS);

	//OPcmanLoadResourcePack("pack.oppack");

    OPjson* ground = (OPjson*)OPcmanLoadGet("ground.meta");
    OPjson* model = OPjsonGet(ground, "model");
    OPlog("MODEL from JSON: %s", OPjsonString(model));

	OPoculusStartup();
	OPrenderInit();
	OPgamePadSetDeadZones(0.2f);

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

	OP_MAIN_START
	//OP_MAIN_START_STEPPED
	OP_MAIN_END
	OP_MAIN_SUCCESS
}
#endif
