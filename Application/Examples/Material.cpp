#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"


// Data for this Game State Example
class MaterialExample : public OPgameState {
	OPmodel* Model;
	OPmat4 World;
	OPeffect Effect;		// The Effect used to render the Mesh
	OPmaterial Material;
	OPcam Camera;			// The Camera to use in the Effect to render the Mesh
	ui32 Rotation;			// The amount to rotate the Mesh
	OPvec3 LightDirection;	// Where the Light Source is coming from


	void Init(OPgameState* last) {

		Model = (OPmodel*)OPCMAN.LoadGet("output.opm");

		Effect.Init("ColoredModel.vert", "ColoredModel.frag");

		Camera.SetPerspective(
			OPVEC3_ONE * 2.0,
			OPVEC3_UP,
			OPVEC3_UP,
			0.1f,
			1000.0f,
			45.0f,
			OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (f32)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height
		);

		LightDirection = OPVEC3_UP;

		Material.Init(&Effect);
		Material.AddParam("vLightDirection", &LightDirection);

		OPrenderDepth(1);
		OPrenderCull(0);
	}

	OPint Update(OPtimer* time) {

		if (OPKEYBOARD.IsDown(OPkeyboardKey::SPACE)) { Rotation++; }

		// Generates an OPmat4 (Matrix 4x4) which is rotated on the Y axis
		World = OPmat4RotY(Rotation / 100.0f);
		OPmat4Scl(&World, 0.25f, 0.25f, 0.25f);

		return false;

	}

	void Render(OPfloat delta) {
		OPrenderClear(0.4f, 0.4f, 0.4f);
		Model->Draw(&World, &Material, &Camera);
		OPrenderPresent();
	}

	// The OPifex Engine will call this itself when you call OPgameStateChange
	OPint Exit(OPgameState* next) {
		// Clean up phase for the Game State
		Effect.Destroy();
		return 0;
	}
};



// This is for the Example Selector only
OPint GS_EXAMPLE_MATERIAL_AVAILABLE = 1;

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
MaterialExample _GS_EXAMPLE_MATERIAL;
OPgameState* GS_EXAMPLE_MATERIAL = &_GS_EXAMPLE_MATERIAL;