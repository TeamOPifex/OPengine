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

#define ExampleCount 8

typedef struct {
	OPfontManager* FontManager;
	Example Examples[ExampleCount];
	i32 Selected;
} ExampleSelector;

ExampleSelector* exampleSelector;


void ExampleSelectorEnter(OPgameState* last) {
	exampleSelector = (ExampleSelector*)OPallocZero(sizeof(ExampleSelector));
	exampleSelector->Selected = 0;

	OPcmanLoad("Ubuntu.opf");
	OPcmanPurge();

	OPsystemsLoadFontEffect();

	exampleSelector->FontManager = OPfontManagerCreate((OPfont*)OPcmanGet("Ubuntu.opf"));
	OPfontManagerBind(exampleSelector->FontManager);
	OPfontManagerSetAlign(exampleSelector->FontManager, OPFONT_ALIGN_LEFT);

	const OPchar* Names[ExampleCount] = {
		"Audio",
		"Model",
		"Oculus",
		"Particle System",
		"Physics",
		"Skinning",
		"Deferred",
		"Textured",
	};

	exampleSelector->Examples[0].name = Names[0];
	exampleSelector->Examples[0].state = &GS_EXAMPLE_AUDIO;
	exampleSelector->Examples[0].available = 1;
	exampleSelector->Examples[1].name = Names[1];
	exampleSelector->Examples[1].state = &GS_EXAMPLE_MODEL;
	exampleSelector->Examples[1].available = 1;
	exampleSelector->Examples[2].name = Names[2];
	exampleSelector->Examples[2].state = &GS_EXAMPLE_OCULUS;
#ifdef OPIFEX_OCULUS
	exampleSelector->Examples[2].available = 1;
#endif
	exampleSelector->Examples[3].name = Names[3];
	exampleSelector->Examples[3].state = &GS_EXAMPLE_PARTICLESYSTEM;
	exampleSelector->Examples[3].available = 1;
	exampleSelector->Examples[4].name = Names[4];
	exampleSelector->Examples[4].state = &GS_EXAMPLE_PHYSICS;
#ifdef OPIFEX_PHYSICS
	exampleSelector->Examples[4].available = 1;
#endif
	exampleSelector->Examples[5].name = Names[5];
	exampleSelector->Examples[5].state = &GS_EXAMPLE_SKINNING;
	exampleSelector->Examples[5].available = 1;

	exampleSelector->Examples[6].name = Names[6];
	exampleSelector->Examples[6].state = &GS_EXAMPLE_DEFERRED;
	exampleSelector->Examples[6].available = 1;

	exampleSelector->Examples[7].name = Names[7];
	exampleSelector->Examples[7].state = &GS_EXAMPLE_TEXTURED;
	exampleSelector->Examples[7].available = 1;

	//exampleSelector->Examples[8].name = "Network Server";
	//exampleSelector->Examples[8].state = &GS_EXAMPLE_NETWORK_SERVER;
	//exampleSelector->Examples[8].available = 1;

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
			mem[0].Memory = (void**)&exampleSelector;
			mem[0].MemorySize = sizeof(ExampleSelector);
			OPinputRecordBegin(time, mem, 1);
		}
	}
	OPkeyboardUpdatePost(time);


	if (OPkeyboardWasPressed(OPKEY_W) || OPkeyboardWasPressed(OPKEY_UP)) {
		exampleSelector->Selected--;
		if (exampleSelector->Selected < 0) exampleSelector->Selected = ExampleCount - 1;
	}
	if (OPkeyboardWasPressed(OPKEY_S) || OPkeyboardWasPressed(OPKEY_DOWN)) {
		exampleSelector->Selected++;
		if (exampleSelector->Selected >= ExampleCount) exampleSelector->Selected = 0;
	}
	if (exampleSelector->Examples[exampleSelector->Selected].available && (OPkeyboardWasPressed(OPKEY_SPACE) || OPkeyboardWasPressed(OPKEY_ENTER))) {
		OPgameStateChange(exampleSelector->Examples[exampleSelector->Selected].state);
		return 0;
	}

	OPrenderClear(0, 0, 0);

	OPint isInActive = 0, isAvailable = 0;
	OPmat4 w;
	OPmat4buildScl(&w, 1, 1, 1);
	OPmat4translate(&w, -0.9, (exampleSelector->Selected) * 0.1, 0);

	f32 r, g, b;
	for (i32 i = 0; i < ExampleCount; i++) {
		isInActive = exampleSelector->Selected != i;
		r = 1, g = b = isInActive;
		if (!exampleSelector->Examples[i].available) {
			r = g = b = 0.2 + !isInActive * 0.4;
		}
		OPmat4translate(&w, 0, -0.1, 0);
		OPrenderText(exampleSelector->Examples[i].name, OPvec4create(r,g,b,1), &w);
	}

	OPrenderPresent();
	return false;
}

void ExampleSelectorExit(OPgameState* next) {
	OPfontManagerDestroy(exampleSelector->FontManager);
	OPrenderBindEffect(NULL);
	OPcmanDelete("Ubuntu.opf");
	OPfree(exampleSelector->Examples);
	OPfree(exampleSelector);
}
