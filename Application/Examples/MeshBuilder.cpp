#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"


// Data for this Game State Example
typedef struct {
	OPmesh Mesh;			// The Mesh to render
	OPeffect Effect;		// The Effect used to render the Mesh
	OPcam Camera;			// The Camera to use in the Effect to render the Mesh
	ui32 Rotation;			// The amount to rotate the Mesh
} MeshBuilderExample;

MeshBuilderExample meshBuilderExample;

struct MeshBuilderVert {
	OPvec3 pos;
	OPvec3 col;
};

void ExampleMeshBuilderEnter(OPgameState* last) {
	OPvertexLayoutBuilder vertexLayoutBuilder = OPvertexLayoutBuilder((ui32)OPattributes::POSITION | (ui32)OPattributes::COLOR);
	OPmeshBuilder builder = OPmeshBuilder(vertexLayoutBuilder.Build());

	MeshBuilderVert one = { OPvec3(-1, 1, 0), OPvec3(1, 0, 0) };
	MeshBuilderVert two = { OPvec3(1, 1, 0), OPvec3(1, 0, 0) };
	MeshBuilderVert three = { OPvec3(1, -1, 0), OPvec3(1, 0, 0) };
	MeshBuilderVert four = { OPvec3(-1, -1, 0), OPvec3(1, 0, 0) };
	builder.Add(&one, &two, &three, &four);

	meshBuilderExample.Mesh = builder.BuildAndDestroy();

	meshBuilderExample.Effect.Init("ColoredModel.vert", "ColoredModel.frag");

	meshBuilderExample.Camera.SetPerspective(OPVEC3_ONE * 2.0, OPVEC3_UP);

	OPrenderDepth(1);
	OPrenderCull(0);
}

OPint ExampleMeshBuilderUpdate(OPtimer* time) {

	if (OPKEYBOARD.IsDown(OPkeyboardKey::SPACE)) { meshBuilderExample.Rotation++; }
	OPmat4 world = OPmat4RotY(meshBuilderExample.Rotation / 100.0f);

	OPrenderClear(0.4f, 0.4f, 0.4f);
	OPbindMeshEffectWorldCam(&meshBuilderExample.Mesh, &meshBuilderExample.Effect, &world, &meshBuilderExample.Camera);

	OPmeshRender();
	OPrenderPresent();

	return 0;
}
void ExampleMeshBuilderRender(OPfloat delta) {

}

// The OPifex Engine will call this itself when you call OPgameStateChange
OPint ExampleMeshBuilderExit(OPgameState* next) {
	// Clean up phase for the Game State
	meshBuilderExample.Effect.Destroy();
	return 0;
}

OPint GS_EXAMPLE_MESH_BUILDER_AVAILABLE = 1;
// This is the Game State for this MeshBuilderExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_MESH_BUILDER = {
	ExampleMeshBuilderEnter,
	ExampleMeshBuilderUpdate,
	ExampleMeshBuilderRender,
	ExampleMeshBuilderExit
};
