#include "./Examples/Model.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/InputSystem.h"
#include "./Human/include/Systems/RenderSystem.h"
#include "./Data/include/ContentManager.h"

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_MODEL = {
	ExampleModelEnter,
	ExampleModelUpdate,
	ExampleModelExit
};

// Data for this Game State Example
typedef struct {
	OPmesh* Mesh;			// The Mesh to render
	OPeffect Effect;		// The Effect used to render the Mesh
	OPcam Camera;			// The Camera to use in the Effect to render the Mesh
	ui32 Rotation;			// The amount to rotate the Mesh
	OPvec3 LightDirection;	// Where the Light Source is coming from
} ModelExample;
ModelExample* modelExample;

void ExampleModelEnter(OPgameState* last) {

	// Load up the mesh into the Content Manager
	// If the model was already loaded by a previous Game State
	// it'll continue on without reloading it
	OPcmanLoad("cube.opm");

	// Allocating a memory block for this example
	modelExample = (ModelExample*)OPallocZero(sizeof(ModelExample));

	// The OPcmanLoad call ensures that this mesh has been loaded
	// The OPcmanGet call returns a pointer to the resource (an OPmesh)
	// that's contained in the Content Manager
	modelExample->Mesh = (OPmesh*)OPcmanGet("cube.opm");

	// The effect that will be used to render the mesh
	// The renderGenEffect is a simplified utility method
	// which requires the attributes are given in a set order
	// Position (vec3), then Normal (vec3)
	// For more granular control use OPrenderCreateEffect
	modelExample->Effect = OPrenderGenEffect(
		"SimpleModel.vert",
		"SimpleModel.frag",
		OPATTR_POSITION | OPATTR_NORMAL,
		"Model Effect",
		modelExample->Mesh->VertexSize);

	// Sets up the camera as a perpsective camera for rendering
	modelExample->Camera = OPcamProj(
		OPvec3One * 2.0,
		OPvec3Up,
		OPvec3Up,
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);

	// A default light direction used in the effect
	modelExample->LightDirection = OPvec3Up;

	// This can be controlled in the update loop if it varies
	// Since this is a simple example we'll ensure that it's set
	// to turn the Depth Buffer on and continue
	OPrenderDepth(1);
}

int ExampleModelUpdate(OPtimer* time) {

	////////////////////////
	// Update
	////////////////////////

	// The application root is set to update the Keyboard, Mouse and GamePads
	// If you need more granular control for when these update, please modify
	// this application's main.cpp
	if (OPkeyboardIsDown(OPKEY_SPACE)) { modelExample->Rotation++; }

	// Generates an OPmat4 (Matrix 4x4) which is rotated on the Y axis
	OPmat4 world = OPmat4createRotY(modelExample->Rotation / 100.0);
	OPmat4scl(&world, 0.025f, 0.025f, 0.025f);

	////////////////////////
	// Render
	////////////////////////
	OPrenderClear(0, 0, 0);

	// A helper utility which binds the Mesh, Effect and the World, View and Projection Matrices
	// For more granular control please take a look at the Textured Example
	OPbindMeshEffectWorldCam(modelExample->Mesh, &modelExample->Effect, &world, &modelExample->Camera);
	
	// Sets the vLightDirection uniform on the Effect that is currently bound (modelExample->Effect)
	OPrenderParamVec3("vLightDirection", &modelExample->LightDirection);

	// Renders to the screen the currently bound Mesh (modelExample->Mesh)
	OPrenderMesh();

	// Swaps the back buffer
	OPrenderPresent();

	// Tells the engine to continue running
	// Returning true will tell the engine to terminate
	return false;

}

// The OPifex Engine will call this itself when you call OPgameStateChange
void ExampleModelExit(OPgameState* next) {
	// Clean up phase for the Game State
	OPrenderUnloadEffect(&modelExample->Effect);	
	OPfree(modelExample);
}
