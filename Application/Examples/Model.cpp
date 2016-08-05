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
	OPvec3 LightDirection;	// Where the Light Source is coming from
} ModelExample;

ModelExample* modelExample;

void ExampleModelEnter(OPgameState* last) {

	// Allocating a memory block for this example
	// Typically not the favored approach for this, but shows
    // how to allocate a block of memory.
	modelExample = (ModelExample*)OPallocZero(sizeof(ModelExample));

	// Load up the mesh into the Content Manager
	// If the model was already loaded by a previous Game State
	// it'll continue on without reloading it.
	// OPcamnLoadGet calls OPcmanLoad and then returns OPcmanGet
	// The OPcmanLoad call ensures that this mesh has been loaded
	// The OPcmanGet call returns a pointer to the resource (an OPmesh)
	// that's contained in the Content Manager
	//modelExample->Mesh = OPcubeCreate(OPvec3Create(1,0,0));
	//OPmesh* mesh = (OPmesh*)OPcmanLoadGet("Box.obj");
	modelExample->Mesh = *(OPmesh*)OPcmanLoadGet("output.opm");

	// The effect that will be used to render the mesh
	// The renderGenEffect is a simplified utility method
	// which requires the attributes are given in a set order
	// Position (vec3), then Normal (vec3)
	// For more granular control use OPeffectCreate
	modelExample->Effect.Init("ColoredModel.vert", "ColoredModel.frag");
	modelExample->Effect.AddUniform("uWorld");
	modelExample->Effect.AddUniform("uView");
	modelExample->Effect.AddUniform("uProj");

	// Sets up the camera as a perpsective camera for rendering
	modelExample->Camera.SetPerspective(
		OPVEC3_ONE * 2.0,
		OPVEC3_UP,
		OPVEC3_UP,
		0.1f,
		1000.0f,
		45.0f,
		OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (f32)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height
		);

	// A default light direction used in the effect
	modelExample->LightDirection = OPVEC3_UP;

	// This can be controlled in the update loop if it varies
	// Since this is a simple example we'll ensure that it's set
	// to turn the Depth Buffer on and continue
	OPrenderDepth(1);
	OPrenderCull(0);
}

OPint ExampleModelUpdate(OPtimer* time) {

	
	////////////////////////
	// Update
	////////////////////////

	// The application root is set to update the Keyboard, Mouse and GamePads
	// If you need more granular control for when these update, please modify
	// this application's main.cpp
	if (OPkeyboardIsDown(OPkeyboardKey::SPACE)) { modelExample->Rotation += time->Elapsed; }


	// Tells the engine to continue running
	// Returning true will tell the engine to terminate
	return false;
}

void ExampleModelRender(OPfloat delta) {

	OPrenderDepth(1);
	OPrenderCull(0);
	
	// Generates an OPmat4 (Matrix 4x4) which is rotated on the Y axis
	OPmat4 world = OPmat4RotY(modelExample->Rotation / 100.0f);
	OPmat4Scl(&world, 0.25f, 0.25f, 0.25f);


	////////////////////////
	// Render
	////////////////////////
	OPrenderClear(0.4f, 0.4f, 0.4f);

	// A helper utility which binds the Mesh, Effect and the World, View and Projection Matrices
	// For more granular control please take a look at the Textured Example
	OPbindMeshEffectWorldCam(&modelExample->Mesh, &modelExample->Effect, &world, &modelExample->Camera);

	// Sets the vLightDirection uniform on the Effect that is currently bound (modelExample->Effect)
	//OPeffectSet("vLightDirection", &modelExample->LightDirection);

	// Renders to the screen the currently bound Mesh (modelExample->Mesh)
	OPmeshRender();


	// 	OPmeshBind(&modelExample->Mesh);
	// 	OPeffectBind(&modelExample->Effect);


	// 	OPeffectParamMat4("uWorld", &world);
	// 	OPeffectParamMat4("uProj", &modelExample->Camera.proj);
	// 	OPeffectParamMat4("uView", &modelExample->Camera.view);

	// 	OPvec3 light = OPvec3Create(0, 1, 0);
	// 	OPeffectParamVec3("vLightDirection", &light);

	// 	OPmeshRender();
	
	
	// Swaps the back buffer
	OPrenderPresent();
}

// The OPifex Engine will call this itself when you call OPgameStateChange
OPint ExampleModelExit(OPgameState* next) {
	// Clean up phase for the Game State
	modelExample->Effect.Destroy();
	OPfree(modelExample);
	return 0;
}

// This is for the Example Selector only
OPint GS_EXAMPLE_MODEL_AVAILABLE = 1;

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_MODEL = {
	ExampleModelEnter,
	ExampleModelUpdate,
	ExampleModelRender,
	ExampleModelExit
};
