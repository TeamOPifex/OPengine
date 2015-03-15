#include "./ExampleSelectorState.h"
#include "./Human/include/Input/OPinputRecord.h"


typedef struct {
	const OPchar* name;
	OPint available;
	OPgameState* state;
} Example;

#define ExampleCount 13

typedef struct {
	i32 Selected;
	OPfontManager* FontManager;
	Example Examples[ExampleCount];
} ExampleSelector;

ExampleSelector exampleSelector = { 0 };
ExampleSelector* exampleSelectorPtr = &exampleSelector;

void ExampleSelectorEnter(OPgameState* last) {

	OPcmanLoad("Ubuntu.opf");
	OPcmanPurge();

	OPfontSystemLoadEffects();

	const OPchar* Names[ExampleCount] = {
		"Audio",
		"Model",
		"Textured",
		"Particle System",
		"IMGUI",
		"Physics",
		"Skinning",
		"Deferred",
		"Oculus",
		"Spine",
		"Scripting",
		"Spherical Cube",
		"Sprite"
	};

	exampleSelector.FontManager = OPfontManagerSetup("Ubuntu.opf", Names, ExampleCount, 0.5);
	exampleSelector.FontManager->scale = 0.75;

	for (OPint i = 0; i < ExampleCount; i++) {
		exampleSelector.Examples[i].name = Names[i];
		exampleSelector.Examples[i].available = 1;
	}



	OPint pos = 0;

	exampleSelector.Examples[pos++].state = &GS_EXAMPLE_AUDIO;
	exampleSelector.Examples[pos++].state = &GS_EXAMPLE_MODEL;
	exampleSelector.Examples[pos++].state = &GS_EXAMPLE_TEXTURED;
	exampleSelector.Examples[pos++].state = &GS_EXAMPLE_PARTICLESYSTEM;
	exampleSelector.Examples[pos++].state = &GS_EXAMPLE_IMGUI;

#ifndef OPIFEX_OPTION_PHYSICS
	exampleSelector.Examples[pos].available = 0;
#endif
	exampleSelector.Examples[pos++].state = &GS_EXAMPLE_PHYSICS;
	exampleSelector.Examples[pos++].state = &GS_EXAMPLE_SKINNING;
	
	exampleSelector.Examples[pos].available = 0;	// Deferred Example in flux
	exampleSelector.Examples[pos++].state = &GS_EXAMPLE_DEFERRED;

#ifndef OPIFEX_OPTION_OCULUS
	exampleSelector.Examples[pos].available = 0;
#endif
	exampleSelector.Examples[pos++].state = &GS_EXAMPLE_OCULUS;

#ifndef OPIFEX_OPTION_SPINE
	exampleSelector.Examples[pos].available = 0;
#endif
	exampleSelector.Examples[pos++].state = &GS_EXAMPLE_SPINE;

#ifndef OPIFEX_OPTION_V8
	exampleSelector.Examples[pos].available = 0;
#endif
	exampleSelector.Examples[pos++].state = &GS_EXAMPLE_SCRIPTING;
	exampleSelector.Examples[pos++].state = &GS_EXAMPLE_SPHERICALCUBE;
	exampleSelector.Examples[pos++].state = &GS_EXAMPLE_SPRITE;

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

	if (exampleSelector.Examples[exampleSelector.Selected].available && (OPkeyboardWasPressed(OPKEY_SPACE) || OPkeyboardWasPressed(OPKEY_ENTER))) {
		OPgameStateChange(exampleSelector.Examples[exampleSelector.Selected].state);
		return 0;
	}

	OPrenderClear(0,0,0,1);

	OPint isInActive = 0, isAvailable = 0;
	OPfloat start = -(exampleSelector.Selected) * 40 + OPRENDER_SCALED_HEIGHT / 2;

	OPfontRenderBegin(exampleSelector.FontManager);

	OPfontColor(OPvec4Create(1, 1, 1, 1)); 
	OPfontRender("EXAMPLES", OPvec2Create(50, start - 60));

	f32 r, g, b;
	for (i32 i = 0; i < ExampleCount; i++) {
		isInActive = exampleSelector.Selected != i;
		r = 1, g = b = isInActive;
		if (!exampleSelector.Examples[i].available) {
			r = g = b = 0.2 + !isInActive * 0.4;
		}
		OPfontColor(OPvec4Create(r,g,b,1)); 
		OPfontRender(exampleSelector.Examples[i].name, OPvec2Create(75, start + 40 * i));
	}

	OPfontRenderEnd();

	OPrenderPresent();
	return false;
}

OPint ExampleSelectorExit(OPgameState* next) {
	OPfontManagerDestroy(exampleSelector.FontManager);
	OPeffectBind(NULL);
	OPcmanDelete("Ubuntu.opf");
	return 0;
}


OPgameState GS_EXAMPLE_SELECTOR = {
	ExampleSelectorEnter,
	ExampleSelectorUpdate,
	ExampleSelectorExit
};
