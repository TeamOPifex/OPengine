#include "./ExampleSelectorState.h"
#include "./Human/include/Input/OPinputRecord.h"

void ExampleSelectorEnter(OPgameState* last);
OPint ExampleSelectorUpdate(OPtimer* time);
void ExampleSelectorExit(OPgameState* next);

OPgameState GS_EXAMPLE_SELECTOR = {
	ExampleSelectorEnter,
	ExampleSelectorUpdate,
	ExampleSelectorExit
};

typedef struct {
	const OPchar* name;
	OPint available;
	OPgameState* state;
} Example;

#define ExampleCount 12

typedef struct {
	i32 Selected;
	OPint checked;
	OPvec3 clear;
	OPfontManager* FontManager;
	OPeffect effect;
	OPchar buffer[100];
	OPint textTimer;
	OPint textActive;
	OPimgui* imgui;
	Example Examples[ExampleCount];
} ExampleSelector;

ExampleSelector exampleSelector = { 0 };
ExampleSelector* exampleSelectorPtr = &exampleSelector;

void keystream(OPchar c) {
	if(!exampleSelector.textActive) return;
	OPint p = strlen(exampleSelector.buffer);
	exampleSelector.buffer[p] = c;
	exampleSelector.buffer[p+1] = NULL;
	OPlog("MyCode %c", c);
}

void ExampleSelectorEnter(OPgameState* last) {

	OPcmanLoad("Ubuntu.opf");
	OPcmanPurge();

	OPfontSystemLoadEffects();
	//OPfontSystemLoadScreenEffects();

	OPKEYBOARD_STREAM = keystream;

	const OPchar* Names[ExampleCount] = {
		"Audio",
		"Model",
		"Textured",
		"Particle System",
		"Physics",
		"Skinning",
		"Deferred",
		"Oculus",
		"Spine",
		"Scripting",
		"Spherical Cube",
		"Sprite"
	};

	OPchar* data = "Testing";
	for(OPint i = 0; i < strlen(data); i++) {
		exampleSelector.buffer[i] = data[i];
	}
	exampleSelector.textTimer = 0;
	exampleSelector.textActive = 0;
	exampleSelector.clear = OPvec3Create(0,0,0);
	exampleSelector.FontManager = OPfontManagerSetup("Ubuntu.opf", Names, ExampleCount);
	exampleSelector.FontManager->scale = 0.5;
	// OPfontManagerBind(exampleSelector.FontManager);
	// OPfontManagerSetAlign(exampleSelector.FontManager, OPFONT_ALIGN_LEFT);
	// OPfontManagerBind(NULL);

	exampleSelector.effect = OPeffectGen(
		"imgui.vert",
		"imgui.frag",
		OPATTR_POSITION | OPATTR_COLOR,
		"IMGUI Shader",
		sizeof(OPfloat) * 7
		);
	exampleSelector.imgui = OPimguiCreate(&exampleSelector.effect, exampleSelector.FontManager);
	exampleSelector.imgui->primaryColor = OPvec4Create(1, 0, 0, 1);
	exampleSelector.imgui->secondaryColor = OPvec4Create(1, 1, 1, 1);
	exampleSelector.imgui->hoverColor = OPvec4Create(0.7, 0.7, 0.7, 1);
	exampleSelector.imgui->padding = OPvec4Create(10, 10, 10, 10);

	OPimguiBind(exampleSelector.imgui);

	for (OPint i = 0; i < ExampleCount; i++) {
		exampleSelector.Examples[i].name = Names[i];
		exampleSelector.Examples[i].available = 1;
	}

	exampleSelector.Examples[0].state = &GS_EXAMPLE_AUDIO;
	exampleSelector.Examples[1].state = &GS_EXAMPLE_MODEL;
	exampleSelector.Examples[2].state = &GS_EXAMPLE_TEXTURED;
	exampleSelector.Examples[3].state = &GS_EXAMPLE_PARTICLESYSTEM;
	exampleSelector.Examples[4].state = &GS_EXAMPLE_PHYSICS;
	exampleSelector.Examples[5].state = &GS_EXAMPLE_SKINNING;
	exampleSelector.Examples[6].state = &GS_EXAMPLE_DEFERRED;
	exampleSelector.Examples[7].state = &GS_EXAMPLE_OCULUS;
	exampleSelector.Examples[8].state = &GS_EXAMPLE_SPINE;
	exampleSelector.Examples[9].state = &GS_EXAMPLE_SCRIPTING;
	exampleSelector.Examples[10].state = &GS_EXAMPLE_SPHERICALCUBE;
	exampleSelector.Examples[11].state = &GS_EXAMPLE_SPRITE;

	// Deferred Example in flux
	exampleSelector.Examples[6].available = 0;

#ifndef OPIFEX_OPTION_OCULUS
	exampleSelector.Examples[7].available = 0;
#endif

#ifndef OPIFEX_OPTION_PHYSICS
	exampleSelector.Examples[4].available = 0;
#endif

#ifndef OPIFEX_OPTION_SPINE
	exampleSelector.Examples[8].available = 0;
#endif

#ifndef OPIFEX_OPTION_V8
	exampleSelector.Examples[9].available = 0;
#endif

	OPlog("Entered Example Selector");
}


OPint ExampleSelectorUpdate(OPtimer* time) {


	if(OPinputRecordIsRunning() && OPkeyboardIsDown(OPKEY_K)) {
		// Stop the Playback & Recording cycle
		OPinputRecordEnd();
	} else if(OPkeyboardWasPressed(OPKEY_L)) {
		if(OPinputRecordIsRecording()) {
			// Start playing back the input that was recorded
			OPinputRecordPlayback();
		} else if(!OPinputRecordIsRunning()) {
			// Begin the Input Recording process
			OPinputRecordMemoryBase mem[1];
			mem[0].Memory = (void**)&exampleSelectorPtr;
			mem[0].MemorySize = sizeof(ExampleSelector);
			OPinputRecordBegin(time, mem, 1);
		}
	}
	OPkeyboardUpdatePost(time);


	if (OPkeyboardWasPressed(OPKEY_W) || OPkeyboardWasPressed(OPKEY_UP)) {
		exampleSelector.Selected--;
		if (exampleSelector.Selected < 0) exampleSelector.Selected = ExampleCount - 1;
	}
	if (OPkeyboardWasPressed(OPKEY_S) || OPkeyboardWasPressed(OPKEY_DOWN)) {
		exampleSelector.Selected++;
		if (exampleSelector.Selected >= ExampleCount) exampleSelector.Selected = 0;
	}
	// if (exampleSelector.Examples[exampleSelector.Selected].available && (OPkeyboardWasPressed(OPKEY_SPACE) || OPkeyboardWasPressed(OPKEY_ENTER))) {
	// 	OPgameStateChange(exampleSelector.Examples[exampleSelector.Selected].state);
	// 	return 0;
	// }

	if(OPkeyboardWasPressed(OPKEY_BACKSPACE) && exampleSelector.textActive && strlen(exampleSelector.buffer) > 0) {
		OPlog("Buff %s", exampleSelector.buffer);
		exampleSelector.buffer[strlen(exampleSelector.buffer) - 1] = NULL;
		OPlog("Buff %s", exampleSelector.buffer);
	}

	OPrenderClear(exampleSelector.clear);

	OPint isInActive = 0, isAvailable = 0;
	OPfloat start = (exampleSelector.Selected) * 0.1;

	//OPfontRenderBegin(exampleSelector.FontManager);

	// f32 r, g, b;
	// for (i32 i = 0; i < ExampleCount; i++) {
	// 	isInActive = exampleSelector.Selected != i;
	// 	r = 1, g = b = isInActive;
	// 	if (!exampleSelector.Examples[i].available) {
	// 		r = g = b = 0.2 + !isInActive * 0.4;
	// 	}
	// 	//OPvec4Create(r,g,b,1), 
	// 	OPfontRender(exampleSelector.Examples[i].name, OPvec2Create(-0.9, start - 0.1 * i));
	// }

	//OPfontRenderEnd();

	OPimguiBegin();

	if(OPimguiCheckbox(exampleSelector.checked, OPvec2Create(5,30), OPvec2Create(20, 20))) { //, OPvec4Create(0.9, 0.9, 0.9, 1), OPvec4Create(0.1,0.1,0.1,1));
		exampleSelector.checked = !exampleSelector.checked;
	}
	OPimguiLabel(OPvec2Create(50, 50), "TEST");

	OPimguiRadio(OPvec2Create(5,5), OPvec2Create(20, 20), OPvec4Create(0.9, 0.9, 0.9, 1), OPvec4Create(0.1,0.1,0.1, 1));
	//OPimguiRadio(exampleSelector.imgui, OPvec2Create(300,150), OPvec2Create(10, 10), OPvec4Create(0.7, 0.7, 0.7, 1));

	if(OPimguiButton(OPvec2Create(5, 150), "TOGGLE")) {
		exampleSelector.clear.z = !exampleSelector.clear.z;
	}

	exampleSelector.textTimer += time->Elapsed;
	//OPlog(" Active %d", exampleSelector.textActive);
	OPint state = OPimguiTextbox(OPvec2Create(5, 250), exampleSelector.buffer, "Your Name", exampleSelector.textActive, exampleSelector.textTimer % 500 > 250);
	if(state == 1) exampleSelector.textActive = true;
	else if (state == 2) exampleSelector.textActive = false;

	OPimguiEnd();

	OPrenderPresent();
	return false;
}

void ExampleSelectorExit(OPgameState* next) {
	OPfontManagerDestroy(exampleSelector.FontManager);
	OPeffectBind(NULL);
	OPcmanDelete("Ubuntu.opf");
}
