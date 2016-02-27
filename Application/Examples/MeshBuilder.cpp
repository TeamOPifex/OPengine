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

	OPmeshBuilder* builder = OPmeshBuilderCreate(sizeof(OPfloat) * 6);

	MeshBuilderVert one = { -1, 1, 0, 1, 0, 0 };
	MeshBuilderVert two = { 1, 1, 0, 1, 0, 0 };
	MeshBuilderVert three = { 1, -1, 0, 1, 0, 0 };
	MeshBuilderVert four = { -1, -1, 0, 1, 0, 0 };
	OPmeshBuilderAdd(builder, &one, &two, &three, &four);

	meshBuilderExample.Mesh = OPmeshBuilderGen(builder);
	OPmeshBuilderDestroy(builder);

	meshBuilderExample.Effect = OPeffectGen(
		"ColoredModel.vert",
		"ColoredModel.frag",
		OPATTR_POSITION | OPATTR_COLOR,
		"MeshBuilder Effect",
		meshBuilderExample.Mesh.vertexLayout.stride);

	meshBuilderExample.Camera = OPcamPersp(
		OPVEC3_ONE * 2.0,
		OPVEC3_UP,
		OPVEC3_UP,
		0.1f,
		1000.0f,
		45.0f,
		OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
		);

	OPrenderDepth(1);
	OPrenderCull(0);
}

OPint ExampleMeshBuilderUpdate(OPtimer* time) {

	OPlog("Update");
	if (OPkeyboardIsDown(OPKEY_SPACE)) { meshBuilderExample.Rotation++; }
	OPlog("Update 1");
	OPmat4 world = OPmat4RotY(meshBuilderExample.Rotation / 100.0f);
	OPlog("Update 2");

	OPrenderClear(0.4f, 0.4f, 0.4f);
	OPlog("Update 3");
	OPbindMeshEffectWorldCam(&meshBuilderExample.Mesh, &meshBuilderExample.Effect, &world, &meshBuilderExample.Camera);

	OPlog("Update 4");
	OPmeshRender();
	OPlog("Update 5");
	OPrenderPresent();
	OPlog("Return");

	return 0;
}
void ExampleMeshBuilderRender(OPfloat delta) {

}

// The OPifex Engine will call this itself when you call OPgameStateChange
OPint ExampleMeshBuilderExit(OPgameState* next) {
	// Clean up phase for the Game State
	OPeffectUnload(&meshBuilderExample.Effect);
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
