#include "./ExampleSelectorState.h"
#include "./Human/include/Input/OPinputRecord.h"

#define TotalCategories 4
#define TotalEntries (ExampleCount + TotalCategories)

typedef struct {
	const OPchar* name;
	OPgameState* state;
	OPint available;
    OPint parent;
} Example;

typedef struct {
	i32 Selected;
    OPint Initialized;
	OPfontManager* FontManager;
	Example Examples[TotalEntries];
    OPint CurrentHierarchy;
    OPint HierarchyDepth[TotalEntries];
    OPint CurrentDepth;
	OPtexture2D* bg;
    OPgamePad* Controller;
} ExampleSelector;

ExampleSelector exampleSelector = { 0 };
ExampleSelector* exampleSelectorPtr = &exampleSelector;

void ExampleSelectorEnter(OPgameState* last) {

	OPcmanLoad("Ubuntu.opf");

	OPfontSystemLoadEffects();

	exampleSelector.bg = OPtexture2DCreate((OPtexture*)OPcmanLoadGet("subtle-irongrip.png"), NULL, OPVEC2_ZERO, OPvec2Create(3, 3));
	exampleSelector.bg->Scale = OPvec2Create(2,2);

    if(!exampleSelector.Initialized) {
        exampleSelector.Initialized = 1;
        exampleSelector.Controller = OPgamePadGet(OPGAMEPAD_ONE);
        Example examples[TotalEntries] = {
            // Categories
            { "Basics", NULL, 1, -1 },
            { "Intermediate", NULL, 1, -1 },
            { "Advanced", NULL, 1, -1 },
            { "Exit", NULL, 1, -1 },

            // Actual Examples
            { "Audio", &GS_EXAMPLE_AUDIO, GS_EXAMPLE_AUDIO_AVAILABLE, 0 },
            { "FMOD", &GS_EXAMPLE_FMOD, GS_EXAMPLE_FMOD_AVAILABLE, 0 },
            { "Free Flight Camera", &GS_EXAMPLE_FREEFLIGHT, GS_EXAMPLE_FREEFLIGHT_AVAILABLE, 0 },
            { "Model", &GS_EXAMPLE_MODEL, GS_EXAMPLE_MODEL_AVAILABLE, 0 },
            { "Textured", &GS_EXAMPLE_TEXTURED, GS_EXAMPLE_TEXTURED_AVAILABLE, 0 },
            { "Sprite", &GS_EXAMPLE_SPRITE, GS_EXAMPLE_SPRITE_AVAILABLE, 0 },
            { "Sprite System", &GS_EXAMPLE_SPRITESYSTEM, GS_EXAMPLE_SPRITESYSTEM_AVAILABLE, 0 },

            { "Mesh Builder", &GS_EXAMPLE_MESH_BUILDER, GS_EXAMPLE_MESH_BUILDER_AVAILABLE, 1 },
            { "Particle System", &GS_EXAMPLE_PARTICLESYSTEM, GS_EXAMPLE_PARTICLESYSTEM_AVAILABLE, 1 },
            { "Spine", &GS_EXAMPLE_SPINE, GS_EXAMPLE_SPINE_AVAILABLE, 1 },
            { "IMGUI", &GS_EXAMPLE_IMGUI, GS_EXAMPLE_IMGUI_AVAILABLE, 1 },
            { "Spherical Cube", &GS_EXAMPLE_SPHERICALCUBE, GS_EXAMPLE_SPHERICALCUBE_AVAILABLE, 1 },

            { "Physics", &GS_EXAMPLE_PHYSICS, GS_EXAMPLE_PHYSICS_AVAILABLE, 2 },
            { "Skinning", &GS_EXAMPLE_SKINNING, GS_EXAMPLE_SKINNING_AVAILABLE, 2 },
            { "Shadows", &GS_EXAMPLE_SHADOWS, GS_EXAMPLE_SHADOWS_AVAILABLE, 2 },
            { "Deferred", &GS_EXAMPLE_DEFERRED, GS_EXAMPLE_DEFERRED_AVAILABLE, 2 }, // in flux
            { "Scripting", &GS_EXAMPLE_SCRIPTING, GS_EXAMPLE_SCRIPTING_AVAILABLE, 2 },
            { "Oculus", &GS_EXAMPLE_OCULUS, GS_EXAMPLE_OCULUS_AVAILABLE, 2 }
        };
        OPmemcpy(exampleSelector.Examples, examples, sizeof(Example) * TotalEntries);


        exampleSelector.CurrentHierarchy = -1;
    }

    const OPchar* Names[TotalEntries];
    for (OPint i = 0; i < TotalEntries; i++) {
        Names[i] = exampleSelector.Examples[i].name;
    }

    exampleSelector.FontManager = OPfontManagerSetup("Ubuntu.opf", Names, TotalEntries, 0.5);
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

    i32 pos = 0;
    for (i32 i = 0; i < TotalEntries; i++) {
        if (exampleSelector.Examples[i].parent != exampleSelector.CurrentHierarchy) continue;
        pos++;
    }
	if (OPkeyboardWasPressed(OPKEY_W) || OPkeyboardWasPressed(OPKEY_UP) || OPgamePadLeftThumbNowUp(exampleSelector.Controller) || OPgamePadWasPressed(exampleSelector.Controller, OPGAMEPADBUTTON_DPAD_UP)) {
		exampleSelector.Selected--;
	}
	if (OPkeyboardWasPressed(OPKEY_S) || OPkeyboardWasPressed(OPKEY_DOWN) || OPgamePadLeftThumbNowDown(exampleSelector.Controller) || OPgamePadWasPressed(exampleSelector.Controller, OPGAMEPADBUTTON_DPAD_DOWN)) {
		exampleSelector.Selected++;
	}
    if (exampleSelector.Selected < 0) exampleSelector.Selected = pos - 1;
    if (exampleSelector.Selected >= pos) exampleSelector.Selected = 0;

    OPuint actualSelected = 0;
    pos = 0;
    for (i32 i = 0; i < TotalEntries && pos <= exampleSelector.Selected; i++) {
        if (exampleSelector.Examples[i].parent != exampleSelector.CurrentHierarchy) continue;
        actualSelected = i;
        pos++;
    }

	if (exampleSelector.Examples[actualSelected].available && (OPkeyboardWasPressed(OPKEY_SPACE) || OPkeyboardWasPressed(OPKEY_E)|| OPkeyboardWasPressed(OPKEY_D) || OPkeyboardWasPressed(OPKEY_ENTER)  || OPgamePadWasPressed(exampleSelector.Controller, OPGAMEPADBUTTON_A) || OPgamePadWasPressed(exampleSelector.Controller, OPGAMEPADBUTTON_DPAD_RIGHT))) {
        if(actualSelected == 3) {
            return 1;
        }
        if(exampleSelector.Examples[actualSelected].state == NULL) {

            exampleSelector.HierarchyDepth[exampleSelector.CurrentHierarchy + 1] = actualSelected;
            exampleSelector.CurrentHierarchy = actualSelected;
            exampleSelector.CurrentDepth++;
            exampleSelector.Selected = exampleSelector.HierarchyDepth[exampleSelector.CurrentHierarchy + 1];
        } else {
            OPgameStateChange(exampleSelector.Examples[actualSelected].state);
            return 0;
        }
	}

    if ((OPkeyboardWasPressed(OPKEY_BACKSPACE) || OPkeyboardWasPressed(OPKEY_A) || OPgamePadWasPressed(exampleSelector.Controller, OPGAMEPADBUTTON_BACK)|| OPgamePadWasPressed(exampleSelector.Controller, OPGAMEPADBUTTON_B) || OPgamePadWasPressed(exampleSelector.Controller, OPGAMEPADBUTTON_DPAD_LEFT))) {
        exampleSelector.HierarchyDepth[exampleSelector.CurrentHierarchy + 1] = exampleSelector.Selected;
        exampleSelector.CurrentHierarchy = -1;
        exampleSelector.CurrentDepth--;
        exampleSelector.Selected = exampleSelector.HierarchyDepth[exampleSelector.CurrentHierarchy + 1];
    }

	OPrenderClear(0,0,0,1);

	OPtexture2DRender(exampleSelector.bg);

	OPint isInActive = 0, isAvailable = 0;
	OPfloat start = -(exampleSelector.Selected) * 40 + OPRENDER_SCALED_HEIGHT / 2;

	OPfontRenderBegin(exampleSelector.FontManager);

	OPfontColor(OPvec4Create(1.0, 1.0, 1.0, 1)); 
	OPfontRender("OPengine v0.4", OPvec2Create(50, start - 60));

	f32 r, g, b;
    pos = 0;
	for (i32 i = 0; i < TotalEntries; i++) {
        OPint isActiveCategory = i == exampleSelector.CurrentHierarchy;
        if(exampleSelector.Examples[i].parent != exampleSelector.CurrentHierarchy && !isActiveCategory) continue;

		isInActive = exampleSelector.Selected != pos;
		r = 0.95, g = 0.84; b = 0;
		if(isInActive) {
			r = g = b = 1.0;
		}
		if (!exampleSelector.Examples[i].available) {
			r = g = b = 0.3 + !isInActive * 0.2;
		}
        if(isActiveCategory) {
            r = g = 0.5; b = 0.9;
        }
		OPfontColor(OPvec4Create(r,g,b,1));
        if(isActiveCategory) {
            OPfontRender(exampleSelector.Examples[i].name,
                         OPvec2Create(75, start + 40 * pos));
        } else {
            OPfontRender(exampleSelector.Examples[i].name,
                         OPvec2Create(75 + 40 * (exampleSelector.CurrentHierarchy != -1), start + 40 * (pos + (exampleSelector.CurrentHierarchy != -1))));
            pos++;
        }
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
