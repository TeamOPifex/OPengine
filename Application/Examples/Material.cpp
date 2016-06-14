#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"


// Data for this Game State Example
typedef struct {
	OPmodel Model;
	OPeffect Effect;		// The Effect used to render the Mesh
	OPmaterial Material;
	OPcam Camera;			// The Camera to use in the Effect to render the Mesh
	ui32 Rotation;			// The amount to rotate the Mesh
	OPvec3 LightDirection;	// Where the Light Source is coming from
} MaterialExample;

MaterialExample materialExample;

void ExampleMaterialEnter(OPgameState* last) {

	materialExample.Model.mesh = (OPmesh*)OPcmanLoadGet("output.opm");

	materialExample.Effect = OPeffectGen(
		"ColoredModel.vert",
		"ColoredModel.frag",
		OPATTR_POSITION | OPATTR_COLOR,
		"Model Effect",
		materialExample.Model.mesh->vertexLayout.stride);

	materialExample.Camera = OPcamPersp(
		OPVEC3_ONE * 2.0,
		OPVEC3_UP,
		OPVEC3_UP,
		0.1f,
		1000.0f,
		45.0f,
		OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
		);

	materialExample.LightDirection = OPVEC3_UP;

    OPmaterialInit(&materialExample.Material, &materialExample.Effect);
    OPmaterialAddParam(&materialExample.Material, "vLightDirection", &materialExample.LightDirection);

	OPrenderDepth(1);
	OPrenderCull(0);
}

OPint ExampleMaterialUpdate(OPtimer* time) {

	if (OPkeyboardIsDown(OPKEY_SPACE)) { materialExample.Rotation++; }

	// Generates an OPmat4 (Matrix 4x4) which is rotated on the Y axis
	materialExample.Model.world = OPmat4RotY(materialExample.Rotation / 100.0f);
	OPmat4Scl(&materialExample.Model.world, 0.25f, 0.25f, 0.25f);

	return false;

}

void ExampleMaterialRender(OPfloat delta) {
	OPrenderClear(0.4f, 0.4f, 0.4f);
	materialExample.Model.Draw(&materialExample.Material, &materialExample.Camera);
	OPrenderPresent();
}

// The OPifex Engine will call this itself when you call OPgameStateChange
OPint ExampleMaterialExit(OPgameState* next) {
	// Clean up phase for the Game State
	OPeffectUnload(&materialExample.Effect);
	return 0;
}

// This is for the Example Selector only
OPint GS_EXAMPLE_MATERIAL_AVAILABLE = 1;

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_MATERIAL = {
	ExampleMaterialEnter,
	ExampleMaterialUpdate,
	ExampleMaterialRender,
	ExampleMaterialExit
};
