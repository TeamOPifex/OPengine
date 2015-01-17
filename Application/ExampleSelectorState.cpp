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

#define ExampleCount 10

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

	OPsystemsLoadFontEffect();

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
		"Scripting"
	};

	exampleSelector.FontManager = OPfontManagerSetup("Ubuntu.opf", Names, ExampleCount);
	OPfontManagerBind(exampleSelector.FontManager);
	OPfontManagerSetAlign(exampleSelector.FontManager, OPFONT_ALIGN_LEFT);

	

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

	// Deferred Example in flux
	exampleSelector.Examples[6].available = 0;

#ifndef OPIFEX_OCULUS
	exampleSelector.Examples[7].available = 0;
#endif

#ifndef OPIFEX_PHYSICS
	exampleSelector.Examples[4].available = 0;
#endif

#ifndef OPIFEX_SPINE
	exampleSelector.Examples[8].available = 0;
#endif

#ifndef OPIFEX_V8
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
	if (exampleSelector.Examples[exampleSelector.Selected].available && (OPkeyboardWasPressed(OPKEY_SPACE) || OPkeyboardWasPressed(OPKEY_ENTER))) {
		OPgameStateChange(exampleSelector.Examples[exampleSelector.Selected].state);
		return 0;
	}

	OPrenderClear(0, 0, 0);

	OPint isInActive = 0, isAvailable = 0;
	OPmat4 w = OPmat4Scl(1, 1, 1);
	OPmat4Translate(&w, -0.9, (exampleSelector.Selected) * 0.1, 0);

	f32 r, g, b;
	for (i32 i = 0; i < ExampleCount; i++) {
		isInActive = exampleSelector.Selected != i;
		r = 1, g = b = isInActive;
		if (!exampleSelector.Examples[i].available) {
			r = g = b = 0.2 + !isInActive * 0.4;
		}
		OPmat4Translate(&w, 0, -0.1, 0);
		OPfontRender(exampleSelector.Examples[i].name, OPvec4Create(r,g,b,1), &w);
	}

	OPrenderPresent();
	return false;
}

void ExampleSelectorExit(OPgameState* next) {
	OPfontManagerDestroy(exampleSelector.FontManager);
	OPeffectBind(NULL);
	OPcmanDelete("Ubuntu.opf");
}
