#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Human/include/Rendering/Primitives/OPsphere.h"
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

		OPsphere::Color = OPvec3(0, 0, 1);
		Model = OPsphere::Create(2, ((ui32)OPattributes::POSITION | (ui32)OPattributes::COLOR | (ui32)OPattributes::NORMAL));

		
		Effect.Init("ColoredModel.vertspv", "ColoredModel.fragspv");

		Camera.SetPerspective(OPVEC3_ONE * 2.0, OPVEC3_ZERO);

		LightDirection = OPVEC3_UP;

		Material.Init(&Effect);
		Material.AddParam("MatrixBlock", "MatrixBlock.uWorld", &World, 0);
		Material.AddParam("MatrixBlock", "MatrixBlock.uView", &Camera.view, 0);
		Material.AddParam("MatrixBlock", "MatrixBlock.uProj", &Camera.proj, 0);
		Material.AddParam("vLightDirection", &LightDirection);

		OPrenderDepth(1);
		OPrenderCull(0);
	}

	OPint Update(OPtimer* time) {

		if (OPKEYBOARD.IsDown(OPkeyboardKey::SPACE)) { Rotation++; }

		// Generates an OPmat4 (Matrix 4x4) which is rotated on the Y axis
		World.SetRotY(Rotation / 100.0f)->Scl(0.25f, 0.25f, 0.25f);
		World.Translate(1, 0, 0);

		return false;

	}

	void Render(OPfloat delta) {
		OPrenderClear(0.4f, 0.4f, 0.4f);

		Material.Bind();

		for (ui32 i = 0; i < Model->meshCount; i++) {
			Model->meshes[i].Bind();
			Model->meshes[i].Draw();
		}

		// Model->Draw(&World, &Material, &Camera);
		OPrenderPresent();
	}

	// The OPifex Engine will call this itself when you call OPgameStateChange
	OPint Exit(OPgameState* next) {
		// Clean up phase for the Game State
		Effect.Destroy();
		Model->Destroy();
		OPfree(Model);
		Material.Destroy();
		return 0;
	}
};



// This is for the Example Selector only
OPint GS_EXAMPLE_MATERIAL_AVAILABLE = 1;

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
MaterialExample _GS_EXAMPLE_MATERIAL;
OPgameState* GS_EXAMPLE_MATERIAL = &_GS_EXAMPLE_MATERIAL;