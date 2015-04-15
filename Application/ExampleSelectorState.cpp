#include "./ExampleSelectorState.h"
#include "./Human/include/Input/OPinputRecord.h"


typedef struct {
	const OPchar* name;
	OPgameState* state;
	OPint available;
} Example;

#define ExampleCount 16

typedef struct {
	i32 Selected;
	OPfontManager* FontManager;
	Example Examples[ExampleCount];
	OPtexture2D* bg;
} ExampleSelector;

ExampleSelector exampleSelector = { 0 };
ExampleSelector* exampleSelectorPtr = &exampleSelector;

void ExampleSelectorEnter(OPgameState* last) {

	OPcmanLoad("Ubuntu.opf");

	OPfontSystemLoadEffects();

	exampleSelector.bg = OPtexture2DCreate((OPtexture*)OPcmanLoadGet("subtle-irongrip.png"), NULL, OPVEC2_ZERO, OPvec2Create(3, 3));
	exampleSelector.bg->Scale = OPvec2Create(2,2);
	Example examples[ExampleCount] = {
		{ "Audio", &GS_EXAMPLE_AUDIO, GS_EXAMPLE_AUDIO_AVAILABLE },
		{ "Model", &GS_EXAMPLE_MODEL, GS_EXAMPLE_MODEL_AVAILABLE },
		{ "Textured", &GS_EXAMPLE_TEXTURED, GS_EXAMPLE_TEXTURED_AVAILABLE },
		{ "Particle System", &GS_EXAMPLE_PARTICLESYSTEM, GS_EXAMPLE_PARTICLESYSTEM_AVAILABLE },
		{ "IMGUI", &GS_EXAMPLE_IMGUI, GS_EXAMPLE_IMGUI_AVAILABLE },
		{ "Physics", &GS_EXAMPLE_PHYSICS, GS_EXAMPLE_PHYSICS_AVAILABLE },
		{ "Skinning", &GS_EXAMPLE_SKINNING, GS_EXAMPLE_SKINNING_AVAILABLE },
		{ "Deferred", &GS_EXAMPLE_DEFERRED, GS_EXAMPLE_DEFERRED_AVAILABLE }, // in flux
		{ "Oculus", &GS_EXAMPLE_OCULUS, GS_EXAMPLE_OCULUS_AVAILABLE },
		{ "Spine", &GS_EXAMPLE_SPINE, GS_EXAMPLE_SPINE_AVAILABLE },
		{ "Scripting", &GS_EXAMPLE_SCRIPTING, GS_EXAMPLE_SCRIPTING_AVAILABLE },
		{ "Spherical Cube", &GS_EXAMPLE_SPHERICALCUBE, GS_EXAMPLE_SPHERICALCUBE_AVAILABLE },
		{ "Sprite", &GS_EXAMPLE_SPRITE, GS_EXAMPLE_SPRITE_AVAILABLE },
		{ "Shadows", &GS_EXAMPLE_SHADOWS, GS_EXAMPLE_SHADOWS_AVAILABLE },
		{ "Mesh Builder", &GS_EXAMPLE_MESH_BUILDER, GS_EXAMPLE_MESH_BUILDER_AVAILABLE },
		{ "FMod", &GS_EXAMPLE_FMOD, GS_EXAMPLE_FMOD_AVAILABLE }
	};
	OPmemcpy(exampleSelector.Examples, examples, sizeof(Example) * ExampleCount);

	const OPchar* Names[ExampleCount];
	for (OPint i = 0; i < ExampleCount; i++) {
		Names[i] = examples[i].name;
	}
	exampleSelector.FontManager = OPfontManagerSetup("Ubuntu.opf", Names, ExampleCount, 0.5);
	exampleSelector.FontManager->scale = 0.75;

	OPcmanPurge();
	OPlog("Entered Example Selector");
}


OPint ExampleSelectorUpdate(OPtimer* time) {

	// TODO: (garrett) move to its own example
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

	OPtexture2DRender(exampleSelector.bg);

	OPint isInActive = 0, isAvailable = 0;
	OPfloat start = -(exampleSelector.Selected) * 40 + OPRENDER_SCALED_HEIGHT / 2;

	OPfontRenderBegin(exampleSelector.FontManager);

	OPfontColor(OPvec4Create(1.0, 1.0, 1.0, 1)); 
	OPfontRender("OPengine v0.4", OPvec2Create(50, start - 60));

	f32 r, g, b;
	for (i32 i = 0; i < ExampleCount; i++) {
		isInActive = exampleSelector.Selected != i;
		r = 0.95, g = 0.84; b = 0;
		if(isInActive) {
			r = g = b = 1.0;
		}
		if (!exampleSelector.Examples[i].available) {
			r = g = b = 0.4 + !isInActive * 0.4;
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
