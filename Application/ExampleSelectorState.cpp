#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/RenderSystem.h"

#include "./Data/include/ContentManager.h"
#include "./Human/include/Input/Input.h"
#include "./Human/include/Systems/FontSystem.h"

#include "./Application/Examples/Audio.h"
#include "./Application/Examples/Model.h"
#include "./Application/Examples/Oculus.h"
#include "./Application/Examples/ParticleSystem.h"
#include "./Application/Examples/Physics.h"
#include "./Application/Examples/Skinning.h"
#include "./Application/Examples/Deferred.h"
#include "./Application/Examples/Textured.h"

OPgameState GS_EXAMPLE_SELECTOR = {
	ExampleSelectorEnter,
	ExampleSelectorUpdate,
	ExampleSelectorExit
};

typedef struct {
	OPchar* name;
	OPint available;
	OPgameState* state;
} Example;

typedef struct {
	OPfontManager* FontManager;
	Example* Examples;
	i32 Selected;
} ExampleSelector;

ExampleSelector* exampleSelector;

#define ExampleCount 8

void ExampleSelectorEnter(OPgameState* last) {
	exampleSelector = (ExampleSelector*)OPalloc(sizeof(ExampleSelector));
	exampleSelector->Selected = 0;

	OPcmanLoad("Ubuntu.opf");
	OPcmanPurge();

	OPsystemsLoadFontEffect();

	exampleSelector->FontManager = OPfontManagerCreate((OPfont*)OPcmanGet("Ubuntu.opf"));
	OPfontManagerBind(exampleSelector->FontManager);
	OPfontManagerSetAlign(exampleSelector->FontManager, OPFONT_ALIGN_LEFT);

	exampleSelector->Examples = (Example*)OPalloc(sizeof(Example)* ExampleCount);
	OPbzero(exampleSelector->Examples, sizeof(Example)* ExampleCount);
	exampleSelector->Examples[0].name = "Audio";
	exampleSelector->Examples[0].state = &GS_EXAMPLE_AUDIO;
	exampleSelector->Examples[0].available = 1;
	exampleSelector->Examples[1].name = "Model";
	exampleSelector->Examples[1].state = &GS_EXAMPLE_MODEL;
	exampleSelector->Examples[1].available = 1;
	exampleSelector->Examples[2].name = "Oculus";
	exampleSelector->Examples[2].state = &GS_EXAMPLE_OCULUS;
#ifdef OPIFEX_OCULUS
	exampleSelector->Examples[2].available = 1;
#endif
	exampleSelector->Examples[3].name = "Particle System";
	exampleSelector->Examples[3].state = &GS_EXAMPLE_PARTICLESYSTEM;
	exampleSelector->Examples[3].available = 1;
	exampleSelector->Examples[4].name = "Physics";
	exampleSelector->Examples[4].state = &GS_EXAMPLE_PHYSICS;
#ifdef OPIFEX_PHYSICS
	exampleSelector->Examples[4].available = 1;
#endif
	exampleSelector->Examples[5].name = "Skinning";
	exampleSelector->Examples[5].state = &GS_EXAMPLE_SKINNING;
	exampleSelector->Examples[5].available = 1;

	exampleSelector->Examples[6].name = "Deferred";
	exampleSelector->Examples[6].state = &GS_EXAMPLE_DEFERRED;
	exampleSelector->Examples[6].available = 1;

	exampleSelector->Examples[7].name = "Textured";
	exampleSelector->Examples[7].state = &GS_EXAMPLE_TEXTURED;
	exampleSelector->Examples[7].available = 1;

	OPlog("Entered Example Selector");
}

int ExampleSelectorUpdate(OPtimer* time) {
	if (OPkeyboardWasPressed(OPKEY_W) || OPkeyboardWasPressed(OPKEY_UP)) {
		exampleSelector->Selected--;
		if (exampleSelector->Selected <= 0) exampleSelector->Selected = 0;
	}
	if (OPkeyboardWasPressed(OPKEY_S) || OPkeyboardWasPressed(OPKEY_DOWN)) {
		exampleSelector->Selected++;
		if (exampleSelector->Selected >= ExampleCount) exampleSelector->Selected = ExampleCount - 1;
	}
	if (exampleSelector->Examples[exampleSelector->Selected].available && (OPkeyboardWasPressed(OPKEY_SPACE) || OPkeyboardWasPressed(OPKEY_ENTER))) {
		OPgameStateChange(exampleSelector->Examples[exampleSelector->Selected].state);
		return 0;
	}

	OPrenderClear(0, 0, 0);

	OPint isInActive = 0, isAvailable = 0;
	f32 r, g, b;
	for (i32 i = 0; i < ExampleCount; i++) {
		isInActive = exampleSelector->Selected != i;
		r = 1, g = b = isInActive;
		if (!exampleSelector->Examples[i].available) {
			r = g = b = 0.2 + !isInActive * 0.4;
		}
		OPrenderTextRGBAXY(exampleSelector->Examples[i].name, r, g, b, 1, -0.9, (exampleSelector->Selected - i) * 0.1);
	}

	OPrenderPresent();
	return false;
}

void ExampleSelectorExit(OPgameState* next) {
	OPfontManagerDestroy(exampleSelector->FontManager);
	OPcmanDelete("Ubuntu.opf");
	OPfree(exampleSelector->Examples);
	OPfree(exampleSelector);
}
