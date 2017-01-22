#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"

#include "./Human/include/Rendering/OPMvertex.h"

#include "./Pipeline/include/SpineLoader.h"


#ifndef SPINE_MESH_VERTEX_COUNT_MAX
#define SPINE_MESH_VERTEX_COUNT_MAX 1000
#endif

// Data for this Game State Example
class SpineExample : public OPgameState {
	OPmesh* Mesh;			// The Mesh to render
	OPeffect Effect;		// The Effect used to render the Mesh
	OPcam Camera;			// The Camera to use in the Effect to render the Mesh
	ui32 Rotation;			// The amount to rotate the Mesh
	OPvec3 LightDirection;	// Where the Light Source is coming from

	Spine* spine;
	f32* worldVertices;
	OPmodel* quad;



	void Init(OPgameState* last) {

		// Load up the mesh into the Content Manager
		// If the model was already loaded by a previous Game State
		// it'll continue on without reloading it
		OPCMAN.Load("cube.opm");
		OPCMAN.Load("spineboy.atlas");


		// The OPCMAN.Load call ensures that this mesh has been loaded
		// The OPCMAN.Get call returns a pointer to the resource (an OPmesh)
		// that's contained in the Content Manager
		Mesh = (OPmesh*)OPCMAN.Get("cube.opm");

		// Sets up the camera as a perpsective camera for rendering
		Camera.SetPerspective(
			OPvec3Create(0.0f, 0.0f, 45.0f),
			OPvec3Create(0.0f, 0.0f, 0.0f),
			OPVEC3_UP,
			0.1f,
			1000.0f,
			45.0f,
			OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (f32)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height
		);

		// A default light direction used in the effect
		LightDirection = OPVEC3_UP;

		// This can be controlled in the update loop if it varies
		// Since this is a simple example we'll ensure that it's set
		// to turn the Depth Buffer on and continue
		OPrenderDepth(1);

		spine = (Spine*)OPCMAN.Get("spineboy.atlas");

		SpineSetMix(spine, "walk", "jump", 0.2f);
		SpineSetMix(spine, "jump", "run", 0.2f);
		SpineSetMix(spine, "run", "walk", 0.2f);
		SpineSetMix(spine, "walk", "shoot", 1.0f);
		SpineBuildMixingState(spine);


		SpineSetAnim(spine, 0, "idle", true);
		//SpineAddAnim(spine, 0, "jump", false, 3);
		//SpineAddAnim(spine, 0, "walk", true, 2);
		//SpineAddAnim(spine, 1, "shoot", false, 5);
		//SpineAddAnim(spine, 0, "death", false, 5);

		worldVertices = (f32*)OPalloc(sizeof(f32)* SPINE_MESH_VERTEX_COUNT_MAX);

		quad = OPquadCreate();

		// The effect that will be used to render the mesh
		// The renderGenEffect is a simplified utility method
		// which requires the attributes are given in a set order
		// Position (vec3), then Normal (vec3)
		// For more granular control use OPeffectCreate
		Effect.Init("SimpleTextured.vert", "SimpleTextured.frag");

		SpineInitialize();
	}

	OPint Update(OPtimer* time) {

		////////////////////////
		// Update
		////////////////////////

		if (OPKEYBOARD.WasPressed(OPkeyboardKey::SPACE)) {
			SpineSetAnim(spine, 0, "jump", true);
		}

		SpineUpdate(spine, time);


		// The application root is set to update the Keyboard, Mouse and GamePads
		// If you need more granular control for when these update, please modify
		// this application's main.cpp
		if (OPKEYBOARD.IsDown(OPkeyboardKey::SPACE)) { Rotation++; }

		// Generates an OPmat4 (Matrix 4x4) which is rotated on the Y axis
		OPmat4 world = OPmat4RotY(Rotation / 100.0f);
		OPmat4Scl(&world, 0.025f, 0.025f, 0.025f);

		////////////////////////
		// Render
		////////////////////////
		OPrenderClear(0, 0, 0);



		SpineRender(spine, &world, &Effect, &Camera);



		// A helper utility which binds the Mesh, Effect and the World, View and Projection Matrices
		// For more granular control please take a look at the Textured Example
		//OPbindMeshEffectWorldCam(Mesh, &Effect, &world, &Camera);

		// Sets the vLightDirection uniform on the Effect that is currently bound (modelExample->Effect)
		//OPeffectParamVec3("vLightDirection", &LightDirection);

		// Renders to the screen the currently bound Mesh (modelExample->Mesh)
		//OPmeshRender();

		// Swaps the back buffer
		OPrenderPresent();

		// Tells the engine to continue running
		// Returning true will tell the engine to terminate
		return false;

	}
	void Render(OPfloat delta) {

	}
	// The OPifex Engine will call this itself when you call OPgameStateChange
	OPint Exit(OPgameState* next) {
		// Clean up phase for the Game State
		Effect.Destroy();
		return 0;
	}
};

#ifndef OPIFEX_OPTION_SPINE
OPint GS_EXAMPLE_SPINE_AVAILABLE = 0;
#else
OPint GS_EXAMPLE_SPINE_AVAILABLE = 1;
#endif
// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
SpineExample _GS_EXAMPLE_SPINE;
OPgameState* GS_EXAMPLE_SPINE = &_GS_EXAMPLE_SPINE;