#include "ExampleSelectorState.h"
#include "EmptyState.h"
#include "FontTest.h"
#include "./Scripting/include/OPloaderOPS.h"
#include "./Scripting/include/JavaScript/OPjavaScriptV8.h"
#include "./Pipeline/include/Loaders/OPloaderOPskeleton.h"
#include "./Pipeline/include/Loaders/OPloaderOPanimation.h"
#include "./Data/include/OPlogToFile.h"

#ifdef ADDON_assimp
#include "./OPassimp.h"
#endif

#ifdef ADDON_imgui
#include "OPimgui.h"
#endif

#include <bitset>
#include <string>
#include <jansson.h>

//////////////////////////////////////
// Application Methods
//////////////////////////////////////
/* forward refs */

OPwindow* mainWindow;
OPwindow* secondWindow;
void ApplicationInit() {
	//OPallocator* allocator = OPallocatorLinearCreate(MB(128));
	//OPDEFAULT_ALLOCATOR = *allocator;

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

    //OPlog("Size ui8: %d", sizeof(ui8));
    //OPlog("Size ui16: %d", sizeof(ui16));
    //OPlog("Size ui32: %d", sizeof(ui32));
    //OPlog("Size i8: %d", sizeof(i8));
    //OPlog("Size i16: %d", sizeof(i16));
    //OPlog("Size i32: %d", sizeof(i32));


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

	//OPcmanLoadResourcePack("pack.oppack");

    OPjson* ground = (OPjson*)OPcmanLoadGet("ground.meta");
    OPjson model = OPjsonGet(*ground, "model");
    OPlog("MODEL from JSON: %s", OPjsonString(model));

	OPoculusStartup();
	OPrenderInit();
	OPlog("Render init");

	i32 contain = 50;
	mainWindow = OPrenderCreateWindow(NULL, false, true, "Main Window", 1280, 720);

	OPlog("Render set"); 
	//mainWindow->SetPosition(OPMONITOR_LIST[0].X + contain, OPMONITOR_LIST[0].Y + contain);
	
	OPlog("Window created");
//	secondWindow = OPrenderCreateWindow(NULL, false, "Secondary Window", OPMONITOR_LIST[1].VideoModeCurrent.Width - contain - contain, OPMONITOR_LIST[1].VideoModeCurrent.Height - contain - contain);
//	secondWindow->SetPosition(OPMONITOR_LIST[1].X + contain, OPMONITOR_LIST[1].Y + contain);

#ifdef ADDON_imgui
//	OPimguiInit(secondWindow->Window, true);
#endif

	OPGAMEPADSYSTEM.SetDeadzones(0.2f);

	mainWindow->Bind();
	mainWindow->Focus();

	

	OPgameStateChange(&GS_EXAMPLE_SELECTOR);
}

OPint ApplicationUpdate(OPtimer* timer) {
// 	secondWindow->Bind();
// 	OPrenderUpdate();

	OPlog("Update 1");
	//mainWindow->Bind();
	OPrenderUpdate();

	OPlog("Update  2");
	OPinputSystemUpdate(timer);
	OPcmanUpdate(timer);
	OPlog("Update 3");

	if (OPkeyboardWasReleased(OPKEY_ESCAPE)) return 1;
	if ((OPkeyboardWasReleased(OPKEY_BACKSPACE) || OPgamePadGet(OPGAMEPAD_ONE)->WasPressed(OPGAMEPADBUTTON_BACK)) && ActiveState != &GS_EXAMPLE_SELECTOR) {
		OPgameStateChange(&GS_EXAMPLE_SELECTOR);
	}

	OPlog("Update 4");
	return ActiveState->Update(timer);
}

void ApplicationRender(OPfloat delta) {
	// OPlog("[%f]", delta);
	//mainWindow->Bind();
	ActiveState->Render(delta);

// 	secondWindow->Bind();
// #ifdef ADDON_imgui
// 	//OPlog("Frame Started");
// 	OPimguiNewFrame();
// 	{
// 		static float f = 0.0f;
// 		ImGui::Text("Hello, world!");
// 		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
// 		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
// 	}

// #endif
// 	OPrenderClear(0.2, 0.2, 0.2);
// #ifdef ADDON_imgui
// 	ImGui::Render();
// #endif
// 	OPrenderPresent();
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
