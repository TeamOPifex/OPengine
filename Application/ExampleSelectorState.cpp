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
	OPint Selected;
    OPint Initialized;
	OPfontManager* FontManager;
	Example Examples[TotalEntries];
    OPint CurrentHierarchy;
    OPint HierarchyDepth[TotalEntries];
    OPint CurrentDepth;
	OPtexture2D*Background;
    OPgamePad* Controller;
} ExampleSelector;

ExampleSelector exampleSelector = { 0 };
ExampleSelector* exampleSelectorPtr = &exampleSelector;

void ExampleSelectorEnter(OPgameState* last) {

	OPcmanLoad("Ubuntu.opf");

	OPfontSystemLoadEffects();

    // The background image to use
	exampleSelector.Background = OPtexture2DCreate((OPtexture*)OPcmanLoadGet("subtle-irongrip.png"), NULL, OPVEC2_ZERO, OPvec2Create(3, 3));
	exampleSelector.Background->Scale = OPvec2Create(2,2);

    if(!exampleSelector.Initialized) {
       // This ensures that our menu selections stay in place
       // And we don't re-initialize the examples array for no reason
       exampleSelector.Initialized = 1;
       exampleSelector.Controller = OPgamePadGet(OPGAMEPAD_ONE);
       Example examples[TotalEntries] = {
           // Categories
           { "Basics", NULL, 1, -1 },
           { "Intermediate", NULL, 1, -1 },
           { "Advanced", NULL, 1, -1 },
           { "Exit", NULL, 1, -1 },

           // Actual Examples
           // Basics
           { "Audio", &GS_EXAMPLE_AUDIO, GS_EXAMPLE_AUDIO_AVAILABLE, 0 },
           { "FMOD", &GS_EXAMPLE_FMOD, GS_EXAMPLE_FMOD_AVAILABLE, 0 },
           { "Free Flight Camera", &GS_EXAMPLE_FREEFLIGHT, GS_EXAMPLE_FREEFLIGHT_AVAILABLE, 0 },
           { "Model", &GS_EXAMPLE_MODEL, GS_EXAMPLE_MODEL_AVAILABLE, 0 },
           { "Material", &GS_EXAMPLE_MATERIAL, GS_EXAMPLE_MATERIAL_AVAILABLE, 0 },
           { "Textured", &GS_EXAMPLE_TEXTURED, GS_EXAMPLE_TEXTURED_AVAILABLE, 0 },
           { "Sprite", &GS_EXAMPLE_SPRITE, GS_EXAMPLE_SPRITE_AVAILABLE, 0 },
           { "Sprite System", &GS_EXAMPLE_SPRITESYSTEM, GS_EXAMPLE_SPRITESYSTEM_AVAILABLE, 0 },
           { "Cube Map", &GS_EXAMPLE_CUBE_MAP, GS_EXAMPLE_CUBE_MAP_AVAILABLE, 0 },

           // Intermediate
           { "Command Buckets", &GS_EXAMPLE_COMMAND_BUCKET, GS_EXAMPLE_COMMAND_BUCKET_AVAILABLE, 1 },
           { "Mesh Builder", &GS_EXAMPLE_MESH_BUILDER, GS_EXAMPLE_MESH_BUILDER_AVAILABLE, 1 },
           { "Particle System", &GS_EXAMPLE_PARTICLESYSTEM, GS_EXAMPLE_PARTICLESYSTEM_AVAILABLE, 1 },
           { "Spine", &GS_EXAMPLE_SPINE, GS_EXAMPLE_SPINE_AVAILABLE, 1 },
		   { "IMGUI", &GS_EXAMPLE_IMGUI, GS_EXAMPLE_IMGUI_AVAILABLE, 1 },
		   { "Ocornuts IMGUI", &GS_EXAMPLE_OCORNUT_IMGUI, GS_EXAMPLE_OCORNUT_IMGUI_AVAILABLE, 1 },
           { "Spherical Cube", &GS_EXAMPLE_SPHERICALCUBE, GS_EXAMPLE_SPHERICALCUBE_AVAILABLE, 1 },
           { "Mouse Intersect", &GS_EXAMPLE_MOUSE_INTERSECT, GS_EXAMPLE_MOUSE_INTERSECT_AVAILABLE, 1 },

           // Advanced
           { "Asio", &GS_EXAMPLE_ASIO, GS_EXAMPLE_ASIO_AVAILABLE, 2 },
           { "RakNet", &GS_EXAMPLE_RAKNET, GS_EXAMPLE_RAKNET_AVAILABLE, 2 },
           { "Physics", &GS_EXAMPLE_PHYSICS, GS_EXAMPLE_PHYSICS_AVAILABLE, 2 },
           { "Physics Character", &GS_EXAMPLE_PHYSICSCHARACTER, GS_EXAMPLE_PHYSICSCHARACTER_AVAILABLE, 2 },
           { "Skinning", &GS_EXAMPLE_SKINNING, GS_EXAMPLE_SKINNING_AVAILABLE, 2 },
           { "Shadows", &GS_EXAMPLE_SHADOWS, GS_EXAMPLE_SHADOWS_AVAILABLE, 2 },
           { "Deferred", &GS_EXAMPLE_DEFERRED, GS_EXAMPLE_DEFERRED_AVAILABLE, 2 }, // in flux
           { "Scripting", &GS_EXAMPLE_SCRIPTING, GS_EXAMPLE_SCRIPTING_AVAILABLE, 2 },
           { "Oculus", &GS_EXAMPLE_OCULUS, GS_EXAMPLE_OCULUS_AVAILABLE, 2 },
           { "Server Client", &GS_EXAMPLE_SERVER_CLIENT, GS_EXAMPLE_SERVER_CLIENT_AVAILABLE, 2 }
       };
       OPmemcpy(exampleSelector.Examples, examples, sizeof(Example) * TotalEntries);

       exampleSelector.CurrentHierarchy = -1;
    }

    // Copy the example names in an array for the Font Manager to use
    const OPchar* Names[TotalEntries];
    for (OPint i = 0; i < TotalEntries; i++) {
       Names[i] = exampleSelector.Examples[i].name;
    }

    exampleSelector.FontManager = OPfontManagerSetup("Ubuntu.opf", Names, TotalEntries);
    exampleSelector.FontManager->scale = 0.5;

	OPcmanPurge();
	OPlog("Entered Example Selector");
}


OPint ExampleSelectorUpdate(OPtimer* time) {

   // Get the number of available options in the current category
   i32 currentCategoryCount = 0;
   for (i32 i = 0; i < TotalEntries; i++) {
       if (exampleSelector.Examples[i].parent != exampleSelector.CurrentHierarchy) continue;
       currentCategoryCount++;
   }

   // Move the current menu selection up and down
   // Automatically wrap around if it exceeds the bounds of options
	if (OPkeyboardWasPressed(OPKEY_W) || OPkeyboardWasPressed(OPKEY_UP) || OPgamePadLeftThumbNowUp(exampleSelector.Controller) || OPgamePadWasPressed(exampleSelector.Controller, OPGAMEPADBUTTON_DPAD_UP)) {
		exampleSelector.Selected--;
	}
	if (OPkeyboardWasPressed(OPKEY_S) || OPkeyboardWasPressed(OPKEY_DOWN) || OPgamePadLeftThumbNowDown(exampleSelector.Controller) || OPgamePadWasPressed(exampleSelector.Controller, OPGAMEPADBUTTON_DPAD_DOWN)) {
		exampleSelector.Selected++;
	}
   if (exampleSelector.Selected < 0) exampleSelector.Selected = currentCategoryCount - 1;
   if (exampleSelector.Selected >= currentCategoryCount) exampleSelector.Selected = 0;

   // Determine the actual selection within our entire array
   // Skips passed the category entries
   OPuint actualSelected = 0;
   for (i32 i = 0, j = 0; i < TotalEntries && j <= exampleSelector.Selected; i++) {
       if (exampleSelector.Examples[i].parent != exampleSelector.CurrentHierarchy) continue;
       actualSelected = i;
       j++;
   }

   // When an example is selected:
	if (exampleSelector.Examples[actualSelected].available && (OPkeyboardWasPressed(OPKEY_SPACE) || OPkeyboardWasPressed(OPKEY_E)|| OPkeyboardWasPressed(OPKEY_D) || OPkeyboardWasPressed(OPKEY_ENTER)  || OPgamePadWasPressed(exampleSelector.Controller, OPGAMEPADBUTTON_A) || OPgamePadWasPressed(exampleSelector.Controller, OPGAMEPADBUTTON_DPAD_RIGHT))) {

       // Hard coded to category [3] which is Exit
       if(actualSelected == 3) {
           return 1;
       }

       // If the state is NULL then it's a category so we need to
       // delve further into the hierarchy
       if(exampleSelector.Examples[actualSelected].state == NULL) {
           exampleSelector.HierarchyDepth[exampleSelector.CurrentHierarchy + 1] = actualSelected;
           exampleSelector.CurrentHierarchy = actualSelected;
           exampleSelector.CurrentDepth++;
           exampleSelector.Selected = exampleSelector.HierarchyDepth[exampleSelector.CurrentHierarchy + 1];
       } else {
           // Otherwise it's an example, load it up.
           OPgameStateChange(exampleSelector.Examples[actualSelected].state);
           return 0;
       }
	}

   // Jump backwards in the hierarchy
   if ((OPkeyboardWasPressed(OPKEY_BACKSPACE) || OPkeyboardWasPressed(OPKEY_A) || OPgamePadWasPressed(exampleSelector.Controller, OPGAMEPADBUTTON_BACK)|| OPgamePadWasPressed(exampleSelector.Controller, OPGAMEPADBUTTON_B) || OPgamePadWasPressed(exampleSelector.Controller, OPGAMEPADBUTTON_DPAD_LEFT))) {
       exampleSelector.HierarchyDepth[exampleSelector.CurrentHierarchy + 1] = exampleSelector.Selected;
       exampleSelector.CurrentHierarchy = -1;
       exampleSelector.CurrentDepth--;
       exampleSelector.Selected = exampleSelector.HierarchyDepth[exampleSelector.CurrentHierarchy + 1];
   }

	return false;
}

void ExampleSelectorRender(OPfloat delta) {


	///////////////
	// RENDER
	///////////////

	OPrenderClear(1, 0, 0);
	//
	// // Render the background
	OPtexture2DRender(exampleSelector.Background);


	// Y coordinate to start drawing the text
	OPfloat start = -(exampleSelector.Selected) * 40.0f + OPRENDER_SCALED_HEIGHT / 2.0f;


	OPfontRenderBegin(exampleSelector.FontManager);

	OPfontColor(OPvec4Create(1.0, 1.0, 1.0, 1));
	exampleSelector.FontManager->scale = 0.75;
	OPfontRender("OPengine v0.4.6", OPvec2Create(50, start - 60));

	exampleSelector.FontManager->scale = 0.5;

	OPint notTheCurrentlySelectedMenuItem = 0, isActiveCategory = 0;
	f32 r, g, b;
	i32 pos = 0;
	for (i32 i = 0; i < TotalEntries; i++) {

		isActiveCategory = i == exampleSelector.CurrentHierarchy;
		if (exampleSelector.Examples[i].parent != exampleSelector.CurrentHierarchy && !isActiveCategory) continue;

		notTheCurrentlySelectedMenuItem = exampleSelector.Selected != pos;
		// Set Selected Color (bright yellow-ish gold)
		r = 0.95f, g = 0.84f; b = 0;

		if (notTheCurrentlySelectedMenuItem) {
			r = g = b = 1.0;
		}

		if (!exampleSelector.Examples[i].available) {
			// Menu item is not available so make it really dark
			r = g = b = 0.3f;
			// Menu item is not available but it's the currently selected
			// item, so we'll brighten it just a bit so that we know what
			// is selected.
			if (!notTheCurrentlySelectedMenuItem) {
				r = g = b = 0.5;
			}
		}

		// If this is a category, then it becomes light blue
		if (isActiveCategory || exampleSelector.Examples[i].state == NULL) {
			r = g = 0.7f; b = 1.0f;
			if (notTheCurrentlySelectedMenuItem && exampleSelector.CurrentDepth == 0) {
				r = g = 0.4f; b = 0.7f;
			}
		}


		OPfontColor(OPvec4Create(r, g, b, 1));

		// If it's a category it doesn't get pushed to the right
		if (isActiveCategory) {
			OPfontRender(exampleSelector.Examples[i].name,
				OPvec2Create(75, start + 40 * pos));
		}
		else {
			// If it's the root menu we don't offset to the right
			// If it isn't the root menu, then we push it to right
			// to help indicate that it's a sub-menu
			OPint isNotRootMenu = (exampleSelector.CurrentHierarchy != -1) ? 1 : 0;
			OPfontRender(exampleSelector.Examples[i].name,
				OPvec2Create((OPfloat)(75 + 40 * isNotRootMenu),
					start + 40 * (pos + isNotRootMenu)));
			pos++;
		}
	}

	OPfontRenderEnd();

	OPrenderPresent();

	//OPlog("rendered example");
}

OPint ExampleSelectorExit(OPgameState* next) {
	//OPfontManagerDestroy(exampleSelector.FontManager);
	//OPeffectBind(NULL);
	//OPcmanDelete("Ubuntu.opf");
	//OPtexture2DDestroy(exampleSelector.Background);
	//OPfontSystemShutdownEffects();
	return 0;
}


OPgameState GS_EXAMPLE_SELECTOR = {
	ExampleSelectorEnter,
	ExampleSelectorUpdate,
	ExampleSelectorRender,
	ExampleSelectorExit
};
