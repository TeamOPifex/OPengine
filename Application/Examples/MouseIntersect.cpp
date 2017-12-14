#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"


struct MouseIntersectVert {
	OPvec3 pos;
	OPvec3 col;
};


// Data for this Game State Example
class MouseIntersectExample : public OPgameState {
	OPmodel* Mesh;			// The Mesh to render
	OPmodel* Mesh2;			// The Mesh to render
	OPeffect Effect;		// The Effect used to render the Mesh
	OPcam Camera;			// The Camera to use in the Effect to render the Mesh
	ui32 Rotation;			// The amount to rotate the Mesh
	OPvec3 LightDirection;	// Where the Light Source is coming from
	OPfontManager* FontManager;


	void Init(OPgameState* last) {

		// Load up the mesh into the Content Manager
		// If the model was already loaded by a previous Game State
		// it'll continue on without reloading it.
		// OPcamnLoadGet calls OPCMAN.Load and then returns OPCMAN.Get
		// The OPCMAN.Load call ensures that this mesh has been loaded
		// The OPCMAN.Get call returns a pointer to the resource (an OPmesh)
		// that's contained in the Content Manager
		//Mesh = OPcubeCreate(OPvec3Create(1,0,0));

		// OPmeshBuilder* builder = OPmeshBuilderCreate(sizeof(OPfloat) * 6);
		//
		// MouseIntersectVert one = { -1, 1, 1, 1, 0, 0 };
		// MouseIntersectVert two = { 1, 1, 1, 1, 0, 0 };
		// MouseIntersectVert three = { 1, -1, 1, 1, 0, 0 };
		// MouseIntersectVert four = { -1, -1, 1, 1, 0, 0 };
		// OPmeshBuilderAdd(builder, &one, &two, &three, &four);
		//
		// Mesh = OPmeshBuilderGen(builder);
		// OPmeshBuilderDestroy(builder);

		Mesh = OPcubeCreate(OPvec3Create(1, 0, 0));
		Mesh2 = OPcubeCreate(OPvec3Create(0.8f, 0.8f, 0.8f));

		// The effect that will be used to render the mesh
		// The renderGenEffect is a simplified utility method
		// which requires the attributes are given in a set order
		// Position (vec3), then Normal (vec3)
		// For more granular control use OPeffectCreate
		Effect.Init("ColoredModel.vert", "ColoredModel.frag");

		// Sets up the camera as a perpsective camera for rendering
		Camera.SetPerspective(
			OPvec3Create(10, 10, 10),
			OPVEC3_ZERO,
			OPVEC3_UP,
			0.1f,
			100.0f,
			45.0f,
			(OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (OPfloat)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height
		);

		// A default light direction used in the effect
		LightDirection = OPVEC3_UP;



		OPCMAN.Load("Ubuntu.opf");

		OPfontSystemLoadEffects();

		FontManager = OPfontManager::Create("Ubuntu.opf", NULL, 0);


		// This can be controlled in the update loop if it varies
		// Since this is a simple example we'll ensure that it's set
		// to turn the Depth Buffer on and continue
		OPrenderDepth(1);
	}

	OPint Update(OPtimer* time) {

		////////////////////////
		// Update
		////////////////////////

		// The application root is set to update the Keyboard, Mouse and GamePads
		// If you need more granular control for when these update, please modify
		// this application's main.cpp
		if (OPKEYBOARD.IsDown(OPkeyboardKey::SPACE)) { Rotation++; }

		Camera.pos.x -= OPKEYBOARD.IsDown(OPkeyboardKey::A) * time->Elapsed * 0.01f;
		Camera.pos.x += OPKEYBOARD.IsDown(OPkeyboardKey::D) * time->Elapsed * 0.01f;
		Camera.target.x -= OPKEYBOARD.IsDown(OPkeyboardKey::A) * time->Elapsed * 0.01f;
		Camera.target.x += OPKEYBOARD.IsDown(OPkeyboardKey::D) * time->Elapsed * 0.01f;
		Camera.Update();
		Camera.UpdateView();


		OPray3D ray = { OPvec3(0, 0, 0), OPvec3(0, 0, 0) };
		OPvec3 positionHit = OPvec3(0, 0, 0);
		OPint intersecting = 0;
		if (OPMOUSE.IsDown(OPmouseButton::LBUTTON)) {

			ray = Camera.Unproject(
				OPMOUSE.X(),
				OPMOUSE.Y()
			);


			{
				OPboundingBox3D box = OPboundingBox3D(
					OPvec3Create(-0.5, -0.5, -0.5),
					OPvec3Create(0.5, 0.5, 0.5)
				);
				if (box.Intersects(ray)) {
					intersecting = 1;
				}
			}

			{
				OPboundingBox3D box = OPboundingBox3D(
					OPvec3Create(-0.5 - 5.0, -0.5, -0.5),
					OPvec3Create(0.5 - 5.0, 0.5, 0.5)
				);

				if (box.Intersects(ray)) {
					intersecting = 2;
				}
			}

			{
				OPplane3D plane = {
					OPvec3Create(0,0,0),
					OPvec3Create(0,1,0)
				};
				if (OPplane3DIntersects(plane, ray, &positionHit)) {
					OPvec3Log("Intersect", positionHit);
				}
			}

		}


		OPvec3 posRay = ray.position;
		posRay += ray.direction * OPvec3Len(ray.position);


		OPmat4 world = OPmat4Translate(OPvec3Create(0, 0, 0));
		OPmat4 world2 = OPmat4Translate(OPvec3Create(-5, 0, 0));
		OPmat4 world3 = OPmat4Translate(positionHit);
		world3 = OPmat4Scl(world3, 0.125, 0.125, 0.125);

		////////////////////////
		// Render
		////////////////////////
		if (intersecting == 1) {
			OPrenderClear(0, 0, 1.0f);
		}
		else if (intersecting == 2) {
			OPrenderClear(0, 1.0f, 0.0);
		}
		else {
			OPrenderClear(0.4f, 0.4f, 0.4f);
		}

		// A helper utility which binds the Mesh, Effect and the World, View and Projection Matrices
		// For more granular control please take a look at the Textured Example
		OPbindMeshEffectWorldCam(Mesh, &Effect, &world, &Camera);

		// Sets the vLightDirection uniform on the Effect that is currently bound (modelExample->Effect)
		OPeffectSet("vLightDirection", &LightDirection);

		// Renders to the screen the currently bound Mesh (modelExample->Mesh)
		OPrenderDrawBufferIndexed(0);

		OPbindMeshEffectWorldCam(
			Mesh,
			&Effect,
			&world2,
			&Camera);
		OPeffectSet("vLightDirection", &LightDirection);
		OPrenderDrawBufferIndexed(0);

		if (OPMOUSE.IsDown(OPmouseButton::LBUTTON)) {
			OPbindMeshEffectWorldCam(
				Mesh2,
				&Effect,
				&world3,
				&Camera);
			OPeffectSet("vLightDirection", &LightDirection);
			OPrenderDrawBufferIndexed(0);
		}

		OPfontRenderBegin(FontManager);
		OPfontColor(OPvec4(1.0, 1.0, 1.0, 1));
		FontManager->scale = 0.75;
		OPchar buffer[256];
#ifdef OPIFEX_WINDOWS
		sprintf_s(buffer, 256, "%d, %d", OPMOUSE.X(), OPMOUSE.Y());
#else
		sprintf(buffer, "%d, %d", OPMOUSE.X(), OPMOUSE.Y());
#endif
		OPfontRender(buffer, OPvec2(50, 60));

#ifdef OPIFEX_WINDOWS
		sprintf_s(buffer, 256, "%f, %f, %f", ray.direction.x, ray.direction.y, ray.direction.z);
#else
		sprintf(buffer, "%f, %f, %f", ray.direction.x, ray.direction.y, ray.direction.z);
#endif
		OPfontRender(buffer, OPvec2(50, 120));

		OPfontRenderEnd();

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


// This is for the Example Selector only
OPint GS_EXAMPLE_MOUSE_INTERSECT_AVAILABLE = 1;

// This is the Game State for this Mouse Intersect
// Each entry is a function pointer for Initialize, Update, Destroy
MouseIntersectExample _GS_EXAMPLE_MOUSE_INTERSECT;
OPgameState* GS_EXAMPLE_MOUSE_INTERSECT = &_GS_EXAMPLE_MOUSE_INTERSECT;