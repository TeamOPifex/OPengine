#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"

#include "./Human/include/Utilities/ImagePNG.h"
#include "./Human/include/Rendering/OPMvertex.h"

#include "./Pipeline/include/SpineLoader.h"

void ExampleSpineEnter(OPgameState* last);
OPint ExampleSpineUpdate(OPtimer* time);
void ExampleSpineExit(OPgameState* next);

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_SPINE = {
	ExampleSpineEnter,
	ExampleSpineUpdate,
	ExampleSpineExit
};

// Data for this Game State Example
typedef struct {
	OPmesh* Mesh;			// The Mesh to render
	OPeffect Effect;		// The Effect used to render the Mesh
	OPcam Camera;			// The Camera to use in the Effect to render the Mesh
	ui32 Rotation;			// The amount to rotate the Mesh
	OPvec3 LightDirection;	// Where the Light Source is coming from

	Spine* spine;
	f32* worldVertices;
	OPrenderBuffer* buffer;
	OPmesh quad;
} SpineExample;
SpineExample* spineExample;

#ifndef SPINE_MESH_VERTEX_COUNT_MAX
#define SPINE_MESH_VERTEX_COUNT_MAX 1000
#endif

void ExampleSpineEnter(OPgameState* last) {

	// Load up the mesh into the Content Manager
	// If the model was already loaded by a previous Game State
	// it'll continue on without reloading it
	OPcmanLoad("cube.opm");
	OPcmanLoad("spineboy.atlas");

	// Allocating a memory block for this example
	spineExample = (SpineExample*)OPallocZero(sizeof(SpineExample));

	// The OPcmanLoad call ensures that this mesh has been loaded
	// The OPcmanGet call returns a pointer to the resource (an OPmesh)
	// that's contained in the Content Manager
	spineExample->Mesh = (OPmesh*)OPcmanGet("cube.opm");

	// Sets up the camera as a perpsective camera for rendering
	spineExample->Camera = OPcamPersp(
		OPvec3Create(0.0f, 0.0f, 45.0f),
		OPvec3Create(0.0f, 0.0f, 0.0f),
		OPVEC3_UP,
		0.1f,
		1000.0f,
		45.0f,
		OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
		);

	// A default light direction used in the effect
	spineExample->LightDirection = OPVEC3_UP;

	// This can be controlled in the update loop if it varies
	// Since this is a simple example we'll ensure that it's set
	// to turn the Depth Buffer on and continue
	OPrenderDepth(1);

	spineExample->spine = (Spine*)OPcmanGet("spineboy.atlas");

	SpineSetMix(spineExample->spine, "walk", "jump", 0.2f);
	SpineSetMix(spineExample->spine, "jump", "run", 0.2f);
	SpineSetMix(spineExample->spine, "run", "walk", 0.2f);
	SpineSetMix(spineExample->spine, "walk", "shoot", 1.0f);
	SpineBuildMixingState(spineExample->spine);


	SpineSetAnim(spineExample->spine, 0, "idle", true);
	SpineAddAnim(spineExample->spine, 0, "jump", false, 3);
	SpineAddAnim(spineExample->spine, 0, "walk", true, 2);
	SpineAddAnim(spineExample->spine, 1, "shoot", false, 5);
	SpineAddAnim(spineExample->spine, 0, "death", false, 5);

	spineExample->worldVertices = (f32*)OPalloc(sizeof(f32)* SPINE_MESH_VERTEX_COUNT_MAX);

	spineExample->quad = OPquadCreate();

	// The effect that will be used to render the mesh
	// The renderGenEffect is a simplified utility method
	// which requires the attributes are given in a set order
	// Position (vec3), then Normal (vec3)
	// For more granular control use OPrenderCreateEffect
	spineExample->Effect = OPrenderGenEffect(
		"SimpleTextured.vert",
		"SimpleTextured.frag",
		OPATTR_POSITION | OPATTR_UV,
		"Model Effect",
		spineExample->quad.VertexSize);

	SpineInitialize();
}

OPint ExampleSpineUpdate(OPtimer* time) {

	////////////////////////
	// Update
	////////////////////////

	SpineUpdate(spineExample->spine, time);


	// The application root is set to update the Keyboard, Mouse and GamePads
	// If you need more granular control for when these update, please modify
	// this application's main.cpp
	if (OPkeyboardIsDown(OPKEY_SPACE)) { spineExample->Rotation++; }

	// Generates an OPmat4 (Matrix 4x4) which is rotated on the Y axis
	OPmat4 world = OPmat4RotY(spineExample->Rotation / 100.0);
	OPmat4Scl(&world, 0.025f, 0.025f, 0.025f);

	////////////////////////
	// Render
	////////////////////////
	OPrenderClear(0, 0, 0);



	SpineRender(spineExample->spine, &world, &spineExample->Effect, &spineExample->Camera);



	// A helper utility which binds the Mesh, Effect and the World, View and Projection Matrices
	// For more granular control please take a look at the Textured Example
	//OPbindMeshEffectWorldCam(spineExample->Mesh, &spineExample->Effect, &world, &spineExample->Camera);

	// Sets the vLightDirection uniform on the Effect that is currently bound (modelExample->Effect)
	//OPrenderParamVec3("vLightDirection", &spineExample->LightDirection);

	// Renders to the screen the currently bound Mesh (modelExample->Mesh)
	//OPrenderMesh();

	// Swaps the back buffer
	OPrenderPresent();

	// Tells the engine to continue running
	// Returning true will tell the engine to terminate
	return false;

}

// The OPifex Engine will call this itself when you call OPgameStateChange
void ExampleSpineExit(OPgameState* next) {
	// Clean up phase for the Game State
	OPrenderUnloadEffect(&spineExample->Effect);
	OPfree(spineExample);
}
