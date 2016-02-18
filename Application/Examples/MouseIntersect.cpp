#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"


// Data for this Game State Example
typedef struct {
	OPmesh Mesh;			// The Mesh to render
	OPmesh Mesh2;			// The Mesh to render
	OPeffect Effect;		// The Effect used to render the Mesh
	OPcam Camera;			// The Camera to use in the Effect to render the Mesh
	ui32 Rotation;			// The amount to rotate the Mesh
	OPvec3 LightDirection;	// Where the Light Source is coming from
	OPfontManager* FontManager;
} MouseIntersectExample;

MouseIntersectExample mouseIntersectExample;

struct MouseIntersectVert {
	OPvec3 pos;
	OPvec3 col;
};

void ExampleMouseIntersectEnter(OPgameState* last) {

	// Load up the mesh into the Content Manager
	// If the model was already loaded by a previous Game State
	// it'll continue on without reloading it.
	// OPcamnLoadGet calls OPcmanLoad and then returns OPcmanGet
	// The OPcmanLoad call ensures that this mesh has been loaded
	// The OPcmanGet call returns a pointer to the resource (an OPmesh)
	// that's contained in the Content Manager
	//mouseIntersectExample.Mesh = OPcubeCreate(OPvec3Create(1,0,0));

	// OPmeshBuilder* builder = OPmeshBuilderCreate(sizeof(OPfloat) * 6);
	//
	// MouseIntersectVert one = { -1, 1, 1, 1, 0, 0 };
	// MouseIntersectVert two = { 1, 1, 1, 1, 0, 0 };
	// MouseIntersectVert three = { 1, -1, 1, 1, 0, 0 };
	// MouseIntersectVert four = { -1, -1, 1, 1, 0, 0 };
	// OPmeshBuilderAdd(builder, &one, &two, &three, &four);
	//
	// mouseIntersectExample.Mesh = OPmeshBuilderGen(builder);
	// OPmeshBuilderDestroy(builder);

	mouseIntersectExample.Mesh = OPcubeCreate(OPvec3Create(1,0,0));
	mouseIntersectExample.Mesh2 = OPcubeCreate(OPvec3Create(0.8,0.8,0.8));

	// The effect that will be used to render the mesh
	// The renderGenEffect is a simplified utility method
	// which requires the attributes are given in a set order
	// Position (vec3), then Normal (vec3)
	// For more granular control use OPeffectCreate
	mouseIntersectExample.Effect = OPeffectGen(
		"ColoredModel.vert",
		"ColoredModel.frag",
		OPATTR_POSITION | OPATTR_COLOR,
		"Model Effect",
		mouseIntersectExample.Mesh.vertexLayout.stride);

	// Sets up the camera as a perpsective camera for rendering
	mouseIntersectExample.Camera = OPcamPersp(
		OPvec3Create(10, 10, 10),
		OPVEC3_ZERO,
		OPVEC3_UP,
		0.1f,
		100.0f,
		45.0f,
		(OPfloat)OPRENDER_WIDTH / (OPfloat)OPRENDER_HEIGHT
		);

	// A default light direction used in the effect
	mouseIntersectExample.LightDirection = OPVEC3_UP;



	OPcmanLoad("Ubuntu.opf");

	OPfontSystemLoadEffects();

    mouseIntersectExample.FontManager = OPfontManagerSetup("Ubuntu.opf", NULL, 0);


	// This can be controlled in the update loop if it varies
	// Since this is a simple example we'll ensure that it's set
	// to turn the Depth Buffer on and continue
	OPrenderDepth(1);
}

OPint ExampleMouseIntersectUpdate(OPtimer* time) {

	////////////////////////
	// Update
	////////////////////////

	// The application root is set to update the Keyboard, Mouse and GamePads
	// If you need more granular control for when these update, please modify
	// this application's main.cpp
	if (OPkeyboardIsDown(OPKEY_SPACE)) { mouseIntersectExample.Rotation++; }

	mouseIntersectExample.Camera.pos.x -= OPkeyboardIsDown(OPKEY_A) * time->Elapsed * 0.01f;
	mouseIntersectExample.Camera.pos.x += OPkeyboardIsDown(OPKEY_D) * time->Elapsed * 0.01f;
	mouseIntersectExample.Camera.target.x -= OPkeyboardIsDown(OPKEY_A) * time->Elapsed * 0.01f;
	mouseIntersectExample.Camera.target.x += OPkeyboardIsDown(OPKEY_D) * time->Elapsed * 0.01f;
	mouseIntersectExample.Camera.Update();
	OPcamUpdateView(&mouseIntersectExample.Camera);


	OPray3D ray = { 0, 0, 0, 0, 0, 0};
	OPvec3 positionHit = { 0, 0, 0};
	OPint intersecting = 0;
	if(OPmouseIsDown(OPMOUSE_LBUTTON)) {

		ray = OPcamUnproject(
			&mouseIntersectExample.Camera,
			OPmousePositionX(),
			OPmousePositionY()
		);


		{
			OPboundingBox3D box = OPboundingBox3DCreate(
				OPvec3Create(-0.5, -0.5, -0.5),
				OPvec3Create(0.5, 0.5, 0.5)
			);
			if(OPboundingBox3DRay3D(box, ray)) {
				intersecting = 1;
			}
		}

		{
			OPboundingBox3D box = OPboundingBox3DCreate(
				OPvec3Create(-0.5 - 5.0, -0.5, -0.5),
				OPvec3Create(0.5 - 5.0, 0.5, 0.5)
			);

			if(OPboundingBox3DRay3D(box, ray)) {
				intersecting = 2;
			}
		}

		{
			OPplane3D plane = {
				OPvec3Create(0,0,0),
				OPvec3Create(0,1,0)
			};
			if(OPplane3DIntersects(plane, ray, &positionHit)) {
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
	if(intersecting == 1) {
		OPrenderClear(0, 0, 1.0);
	} else if(intersecting == 2) {
		OPrenderClear(0, 1.0, 0.0);
	} else {
		OPrenderClear(0.4, 0.4, 0.4);
	}

	// A helper utility which binds the Mesh, Effect and the World, View and Projection Matrices
	// For more granular control please take a look at the Textured Example
	OPbindMeshEffectWorldCam(&mouseIntersectExample.Mesh, &mouseIntersectExample.Effect, &world, &mouseIntersectExample.Camera);

	// Sets the vLightDirection uniform on the Effect that is currently bound (modelExample->Effect)
	OPeffectParamVec3("vLightDirection", &mouseIntersectExample.LightDirection);

	// Renders to the screen the currently bound Mesh (modelExample->Mesh)
	OPmeshRender();

	OPbindMeshEffectWorldCam(
		&mouseIntersectExample.Mesh,
		&mouseIntersectExample.Effect,
		&world2,
		&mouseIntersectExample.Camera);
	OPeffectParamVec3("vLightDirection", &mouseIntersectExample.LightDirection);
	OPmeshRender();

	if(OPmouseIsDown(OPMOUSE_LBUTTON)) {
		OPbindMeshEffectWorldCam(
			&mouseIntersectExample.Mesh2,
			&mouseIntersectExample.Effect,
			&world3,
			&mouseIntersectExample.Camera);
		OPeffectParamVec3("vLightDirection", &mouseIntersectExample.LightDirection);
		OPmeshRender();
	}

	OPfontRenderBegin(mouseIntersectExample.FontManager);
	OPfontColor(OPvec4Create(1.0, 1.0, 1.0, 1));
   	mouseIntersectExample.FontManager->scale = 0.75;
	i8 buffer[256];
	sprintf(buffer, "%d, %d", OPmousePositionX(), OPmousePositionY());
	OPfontRender(buffer, OPvec2Create(50, 60));

	sprintf(buffer, "%f, %f, %f", ray.direction.x, ray.direction.y, ray.direction.z);
	OPfontRender(buffer, OPvec2Create(50, 120));

	OPfontRenderEnd();

	// Swaps the back buffer
	OPrenderPresent();

	// Tells the engine to continue running
	// Returning true will tell the engine to terminate
	return false;

}
void ExampleMouseIntersectRender(OPfloat delta) {

}

// The OPifex Engine will call this itself when you call OPgameStateChange
OPint ExampleMouseIntersectExit(OPgameState* next) {
	// Clean up phase for the Game State
	OPeffectUnload(&mouseIntersectExample.Effect);
	return 0;
}

// This is for the Example Selector only
OPint GS_EXAMPLE_MOUSE_INTERSECT_AVAILABLE = 1;

// This is the Game State for this Mouse Intersect
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_MOUSE_INTERSECT = {
	ExampleMouseIntersectEnter,
	ExampleMouseIntersectUpdate,
	ExampleMouseIntersectRender,
	ExampleMouseIntersectExit
};
