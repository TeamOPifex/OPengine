#include "ExampleSelectorState.h"

#ifdef ADDON_assimp
#include "./OPassimp.h"
#endif

#ifdef ADDON_libjpeg_turbo
#include "OPlibjpegturbo.h"
#endif

#ifdef ADDON_socketio
#include "OPsocketGamePadSystem.h"
#endif

#ifdef ADDON_mono
#include "OPmono.h"
#endif

#ifdef ADDON_spirv
#include "OPspirv.h"
#endif

#ifdef ADDON_imgui
#include "./OPimgui.h"
#endif

//////////////////////////////////////
// Application Methods
//////////////////////////////////////

OPwindow mainWindow;

void ApplicationInit() {
	TIMED_BLOCK;

	OPcam _camera;
	_camera.SetPerspective(OPvec3(0, 0, 5), OPvec3(0, 0, 0), OPvec3(0, 1, 0), 0.1f, 50.0f, 45.0f, 1920.0f / 1080.0f);

	OPmat4 _proj = _camera.proj;
	OPmat4 _view = _camera.view;


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

#ifdef ADDON_spirv
	OPsprivAddLoader();
#endif


	OPlogErr("Loaders added");

	OPoculusStartup();


	OPlogErr("OPrenderSetup");
	OPrenderSetup();
	OPlogErr("OPrenderSetup Finished");
	OPwindowSystemInit();
	OPlogErr("OPwindowSystemInit Finished");


	OPmonitorResult result = OPmonitor::GetAll();
	//mainWindow.Init(NULL, OPwindowParameters("Main Window", false, result.primary.VideoModeCurrent.Width, result.primary.VideoModeCurrent.Height));
	mainWindow.Init(NULL, OPwindowParameters("Main Window", false, 600, 600));
	//mainWindow.Init(&result.primary, OPwindowParameters("Main Window", false, result.primary.VideoModeCurrent.Width, result.primary.VideoModeCurrent.Height));
	OPlogErr("mainWindow.Init Finished");

	OPrenderInit(&mainWindow);
	OPlogErr("OPrenderInit Finished");

	OPGAMEPADS.SetDeadzones(0.2f);
	OPtouch::Init();

#ifdef ADDON_socketio
	OPSOCKETGAMEPADS.Init();
	OPSOCKETGAMEPADS.SetDeadzones(0.2f);
#endif

	OPCMAN.LoadGet("cemetery.png");
	//OPCMAN.LoadGet("Factory_Catwalk_Env.hdr");

	OPVISUALDEBUGINFO.Init();

#ifdef ADDON_imgui
	OPimguiInit(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE, true);
#endif
	
	//GS_EXAMPLE_MODEL
	OPgameState::Change(GS_EXAMPLE_SELECTOR);
}

OPint ApplicationUpdate(OPtimer* timer) {
	//return 0;
	if (mainWindow.Update()) {
		return 1;
	}
	OPVISUALDEBUGINFO.Update(timer);

	OPinputSystemUpdate(timer);
#ifdef ADDON_socketio
	OPSOCKETGAMEPADS.Update(timer);
#endif
	OPCMAN_UPDATE(timer);

	// if (OPKEYBOARD.WasReleased(OPkeyboardKey::ESCAPE)) return 1;
	if ((OPKEYBOARD.WasReleased(OPkeyboardKey::ESCAPE) || OPGAMEPADS[0]->WasPressed(OPgamePadButton::BACK)) && ActiveState != GS_EXAMPLE_SELECTOR) {
		OPgameState::Change(GS_EXAMPLE_SELECTOR);
	}
#ifdef ADDON_socketio
	if ((OPSOCKETGAMEPADS[0]->WasPressed(OPgamePadButton::BACK)) && ActiveState != GS_EXAMPLE_SELECTOR) {
		OPgameState::Change(GS_EXAMPLE_SELECTOR);
	}
#endif

	return ActiveState->Update(timer);
}

void ApplicationRender(OPfloat delta) {
	ActiveState->Render(delta);
	
	// OPrenderClear(1,0,0);
	// OPrenderPresent();
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
		OPLOGLEVEL = (ui32)OPlogLevel::TIMING;

		OPlogErr("Started!");

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