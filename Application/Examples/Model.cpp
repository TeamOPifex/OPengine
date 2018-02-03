#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Human/include/Rendering/Primitives/OPsphere.h"
#include "./Data/include/OPcman.h"


#ifdef ADDON_socketio
#include "OPsocketGamePadSystem.h"
#endif


// Data for this Game State Example
class ModelExample : public OPgameState {
public:
	OPmodel* Mesh;			// The Mesh to render
	OPeffect Effect;		// The Effect used to render the Mesh
	OPcamFreeFlight Camera;			// The Camera to use in the Effect to render the Mesh
	d64 Rotation;			// The amount to rotate the Mesh
	d64 Y;
	OPvec3 LightDirection;	// Where the Light Source is coming from


	void Init(OPgameState* last) {


		// The effect that will be used to render the mesh
		// The renderGenEffect is a simplified utility method
		// which requires the attributes are given in a set order
		// Position (vec3), then Normal (vec3)
		// For more granular control use OPeffectCreate
		Effect.Init("ColoredModel.vert", "ColoredModel.frag");

		// Load up the mesh into the Content Manager
		// If the model was already loaded by a previous Game State
		// it'll continue on without reloading it.
		// OPcamnLoadGet calls OPCMAN.Load and then returns OPCMAN.Get
		// The OPCMAN.Load call ensures that this mesh has been loaded
		// The OPCMAN.Get call returns a pointer to the resource (an OPmesh)
		// that's contained in the Content Manager
		//modelExample->Mesh = OPcubeCreate(OPvec3Create(1,0,0));
		//OPmesh* mesh = (OPmesh*)OPCMAN.LoadGet("Box.obj");
		//Mesh = *(OPmodel*)OPCMAN.LoadGet("output.opm");
		OPsphere::Color = OPvec3(0, 0, 1);
		Mesh = OPsphere::Create(2, ((ui32)OPattributes::POSITION | (ui32)OPattributes::COLOR | (ui32)OPattributes::NORMAL));

		// Sets up the camera as a perpsective camera for rendering
		Camera.Init();// .SetPerspective(OPVEC3_ONE * 2.0, OPVEC3_ZERO);

		// A default light direction used in the effect
		LightDirection = OPVEC3_UP;

		// This can be controlled in the update loop if it varies
		// Since this is a simple example we'll ensure that it's set
		// to turn the Depth Buffer on and continue
		OPrenderDepth(1);
		OPrenderCull(0);
	}

	OPint Update(OPtimer* time) {


		////////////////////////
		// Update
		////////////////////////

		Camera.Update(time);

		// The application root is set to update the Keyboard, Mouse and GamePads
		// If you need more granular control for when these update, please modify
		// this application's main.cpp
		//if (OPKEYBOARD.IsDown(OPkeyboardKey::SPACE)) { 
			Rotation += time->Elapsed;
		// }
#ifdef ADDON_socketio
		OPSOCKETGAMEPADS.Update(time);
		Rotation += OPSOCKETGAMEPADS[0]->LeftThumbX() * 10;
		Y -= OPSOCKETGAMEPADS[0]->LeftThumbY() / 10.0f;
#endif

		// Tells the engine to continue running
		// Returning true will tell the engine to terminate
		return false;
	}

	void Render(OPfloat delta) {

		OPrenderDepth(1);
		OPrenderCull(0);

		// Generates an OPmat4 (Matrix 4x4) which is rotated on the Y axis
		OPmat4 world = OPMAT4_IDENTITY;
		//world.RotY(Rotation / 100.0f)->Scl(0.25f, 0.25f, 0.25f);
		//world.Translate(1, Y, 0);


		////////////////////////
		// Render
		////////////////////////
#ifdef ADDON_socketio
		if (OPSOCKETGAMEPADS[0]->IsDown(OPgamePadButton::A)) {
			OPrenderClear(0.4f, 0.0f, 0.0f);
		}
		else {
			OPrenderClear(0.4f, 0.4f + OPSOCKETGAMEPADS[0]->LeftThumbY(), 0.4f);
		}
#else
		OPrenderClear(0.4f, 0.0f, 0.0f);
#endif

		// A helper utility which binds the Mesh, Effect and the World, View and Projection Matrices
		// For more granular control please take a look at the Textured Example
		// OPbindMeshEffectWorldCam(Mesh, &Effect, &world, &Camera);


		//MatrixBlock
		Effect.Bind();
		OPshaderUniformBuffer* ubo = Effect.GetUniformBuffer("MatrixBlock");
		Effect.Set(ubo, "uWorld", &world);
		Effect.Set(ubo, "uProj", &Camera.Camera.proj);
		Effect.Set(ubo, "uView", &Camera.Camera.view);
		OPRENDERER_ACTIVE->ShaderUniformBuffer.Bind(ubo);
		Mesh->Bind();

		// Sets the vLightDirection uniform on the Effect that is currently bound (modelExample->Effect)
		//OPeffectSet("vLightDirection", &modelExample->LightDirection);

		// Renders to the screen the currently bound Mesh (modelExample->Mesh)
		OPrenderDrawBufferIndexed(0);

		// Swaps the back buffer
		OPrenderPresent();
	}

	// The OPifex Engine will call this itself when you call OPgameStateChange
	OPint Exit(OPgameState* next) {
		// Clean up phase for the Game State
		Effect.Destroy();
		Mesh->Destroy();
		OPfree(Mesh);
		return 0;
	}
};



// This is for the Example Selector only
OPint GS_EXAMPLE_MODEL_AVAILABLE = 1;

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
ModelExample _GS_EXAMPLE_MODEL;
OPgameState* GS_EXAMPLE_MODEL = &_GS_EXAMPLE_MODEL;