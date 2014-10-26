#include "./Examples/Myo.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/InputSystem.h"
#include "./Human/include/Systems/RenderSystem.h"
#include "./Data/include/ContentManager.h"

// This is the Game State for this MyoExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_MYO = {
	ExampleMyoEnter,
	ExampleMyoUpdate,
	ExampleMyoExit
};

// Data for this Game State Example
typedef struct {
	OPmesh* Mesh;			// The Mesh to render
	OPeffect Effect;		// The Effect used to render the Mesh
	OPcam Camera;			// The Camera to use in the Effect to render the Mesh
	f32 Rotation;			// The amount to rotate the Mesh
	OPvec3 LightDirection;	// Where the Light Source is coming from
} MyoExample;
MyoExample* myoExample;

void ExampleMyoEnter(OPgameState* last) {

	// Load up the mesh into the Content Manager
	// If the myo was already loaded by a previous Game State
	// it'll continue on without reloading it
	OPcmanLoad("cube.opm");

	// Allocating a memory block for this example
	myoExample = (MyoExample*)OPallocZero(sizeof(MyoExample));

	// The OPcmanLoad call ensures that this mesh has been loaded
	// The OPcmanGet call returns a pointer to the resource (an OPmesh)
	// that's contained in the Content Manager
	myoExample->Mesh = (OPmesh*)OPcmanGet("cube.opm");

	// The effect that will be used to render the mesh
	// The renderGenEffect is a simplified utility method
	// which requires the attributes are given in a set order
	// Position (vec3), then Normal (vec3)
	// For more granular control use OPrenderCreateEffect
	myoExample->Effect = OPrenderGenEffect(
		"SimpleModel.vert",
		"SimpleModel.frag",
		OPATTR_POSITION | OPATTR_NORMAL,
		"Model Effect",
		myoExample->Mesh->VertexSize);

	// Sets up the camera as a perpsective camera for rendering
	myoExample->Camera = OPcamProj(
		OPvec3One * 2.0,
		OPvec3Up,
		OPvec3Up,
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);

	// A default light direction used in the effect
	myoExample->LightDirection = OPvec3Up;

	// This can be controlled in the update loop if it varies
	// Since this is a simple example we'll ensure that it's set
	// to turn the Depth Buffer on and continue
	OPrenderDepth(1);

	OPmyoConnect();
}

OPint ExampleMyoUpdate(OPtimer* time) {

	////////////////////////
	// Update
	////////////////////////

	OPmyoUpdate(time);
	OPglError("OpenGL Error");

	// The application root is set to update the Keyboard, Mouse and GamePads
	// If you need more granular control for when these update, please modify
	// this application's main.cpp
	if (OPkeyboardIsDown(OPKEY_SPACE)) { myoExample->Rotation++; }

	if (OPmyoPoseIs(Myo_Pose_Fist)) { myoExample->Rotation+=10; }
	if (OPmyoPoseNow(Myo_Pose_Fist)) {
		OPmyoSetVibration(Myo_Vibration_Medium);
	}

	myoExample->Rotation += OPmyoRollChange() * 100 * OPpi * 2;

	// Generates an OPmat4 (Matrix 4x4) which is rotated on the Y axis
	OPmat4 world = OPmat4createRotY(myoExample->Rotation / 100.0f);
	OPmat4scl(&world, 0.25f, 0.25f, 0.25f);

	////////////////////////
	// Render
	////////////////////////
	OPrenderClear(0.4, 0.4, 0.4);

	// A helper utility which binds the Mesh, Effect and the World, View and Projection Matrices
	// For more granular control please take a look at the Textured Example
	OPbindMeshEffectWorldCam(myoExample->Mesh, &myoExample->Effect, &world, &myoExample->Camera);
	
	// Sets the vLightDirection uniform on the Effect that is currently bound (myoExample->Effect)
	OPrenderParamVec3("vLightDirection", &myoExample->LightDirection);

	// Renders to the screen the currently bound Mesh (myoExample->Mesh)
	OPrenderMesh();

	// Swaps the back buffer
	OPrenderPresent();

	// Tells the engine to continue running
	// Returning true will tell the engine to terminate
	return false;

}

// The OPifex Engine will call this itself when you call OPgameStateChange
void ExampleMyoExit(OPgameState* next) {
	// Clean up phase for the Game State
	OPrenderUnloadEffect(&myoExample->Effect);	
	OPfree(myoExample);
}
