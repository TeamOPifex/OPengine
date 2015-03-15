#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./OPengine.h"

typedef struct {
	OPeffect effect;
	OPchar buffer[100];
	OPint textTimer;
	OPint textActive;
	OPvec3 clear;
	OPint checked;
	OPfontManager* FontManager;
	OPimgui* imgui;
} IMGUIExample;

IMGUIExample imguiExample;

void keystream(OPchar c) {
	if(!imguiExample.textActive) return;
	OPint p = strlen(imguiExample.buffer);
	imguiExample.buffer[p] = c;
	imguiExample.buffer[p+1] = NULL;
	OPlog("MyCode %c", c);
}

void ExampleIMGUIEnter(OPgameState* last) {

	OPKEYBOARD_STREAM = keystream;

	OPchar* data = "Testing";
	for(OPint i = 0; i < strlen(data); i++) {
		imguiExample.buffer[i] = data[i];
	}
	imguiExample.textTimer = 0;
	imguiExample.textActive = 0;
	imguiExample.clear = OPvec3Create(0,0,0);

	imguiExample.FontManager = OPfontManagerSetup("Ubuntu.opf", NULL, 0, 0.5);
	imguiExample.FontManager->scale = 0.75;


	imguiExample.effect = OPeffectGen(
		"imgui.vert",
		"imgui.frag",
		OPATTR_POSITION | OPATTR_COLOR,
		"IMGUI Shader",
		sizeof(OPfloat) * 7
		);
	imguiExample.imgui = OPimguiCreate(&imguiExample.effect, imguiExample.FontManager);
	imguiExample.imgui->primaryColor = OPvec4Create(1, 0, 0, 1);
	imguiExample.imgui->secondaryColor = OPvec4Create(1, 1, 1, 1);
	imguiExample.imgui->hoverColor = OPvec4Create(0.9, 0.9, 0.9, 1);
	imguiExample.imgui->padding = OPvec4Create(10, 10, 10, 10);

	OPimguiBind(imguiExample.imgui);
}

OPint ExampleIMGUIUpdate(OPtimer* time) {


	if(OPkeyboardWasPressed(OPKEY_BACKSPACE) && imguiExample.textActive && strlen(imguiExample.buffer) > 0) {
		OPlog("Buff %s", imguiExample.buffer);
		imguiExample.buffer[strlen(imguiExample.buffer) - 1] = NULL;
		OPlog("Buff %s", imguiExample.buffer);
	}


	OPrenderClear(imguiExample.clear);

	OPimguiBegin();

	if(OPimguiCheckbox(imguiExample.checked, OPvec2Create(570, 20), OPvec2Create(20, 20))) { //, OPvec4Create(0.9, 0.9, 0.9, 1), OPvec4Create(0.1,0.1,0.1,1));
		imguiExample.checked = !imguiExample.checked;
	}
	//OPimguiLabel(OPvec2Create(50, 50), "TEST");

	//OPimguiRadio(OPvec2Create(5,5), OPvec2Create(20, 20), OPvec4Create(0.9, 0.9, 0.9, 1), OPvec4Create(0.1,0.1,0.1, 1));
	//OPimguiRadio(exampleSelector.imgui, OPvec2Create(300,150), OPvec2Create(10, 10), OPvec4Create(0.7, 0.7, 0.7, 1));

	if(OPimguiButton(OPvec2Create(440, 20), "TOGGLE")) {
		imguiExample.clear.z = !imguiExample.clear.z;
	}

	imguiExample.textTimer += time->Elapsed;
	//OPlog(" Active %d", exampleSelector.textActive);
	OPint state = OPimguiTextbox(OPvec2Create(20, 20), imguiExample.buffer, "Your Name", imguiExample.textActive, imguiExample.textTimer % 500 > 250);
	if(state == 1) imguiExample.textActive = true;
	else if (state == 2) imguiExample.textActive = false;

	OPimguiEnd();

	OPrenderPresent();

	return false;

}

OPint ExampleIMGUIExit(OPgameState* next) {
	if(imguiExample.textActive) return 1;

	return 0;
}

OPint GS_EXAMPLE_IMGUI_AVAILABLE = 1;
// This is the Game State for this IMGUIExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_IMGUI = {
	ExampleIMGUIEnter,
	ExampleIMGUIUpdate,
	ExampleIMGUIExit
};
