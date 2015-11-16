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
		mouseIntersectExample.Mesh.VertexSize);

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

bool intersection(OPboundingBox3D b, OPray3D ray) {
    double tmin = -INFINITY, tmax = INFINITY;

    for (int i = 0; i < 3; ++i) {
        if (ray.direction[i] != 0.0) {
            double t1 = (b.min[i] - ray.position[i])/ray.direction[i];
            double t2 = (b.max[i] - ray.position[i])/ray.direction[i];

            tmin = OPMAX(tmin, OPMIN(t1, t2));
            tmax = OPMIN(tmax, OPMAX(t1, t2));
        } else if (ray.position[i] <= b.min[i] || ray.position[i] >= b.max[i]) {
            return false;
        }
    }

    return tmax > tmin && tmax > 0.0;
}

OPint ExampleMouseIntersectUpdate(OPtimer* time) {

	////////////////////////
	// Update
	////////////////////////

	// The application root is set to update the Keyboard, Mouse and GamePads
	// If you need more granular control for when these update, please modify
	// this application's main.cpp
	if (OPkeyboardIsDown(OPKEY_SPACE)) { mouseIntersectExample.Rotation++; }

	mouseIntersectExample.Camera._pos.x -= OPkeyboardIsDown(OPKEY_A) * time->Elapsed * 0.01f;
	mouseIntersectExample.Camera._pos.x += OPkeyboardIsDown(OPKEY_D) * time->Elapsed * 0.01f;
	mouseIntersectExample.Camera._targ.x -= OPkeyboardIsDown(OPKEY_A) * time->Elapsed * 0.01f;
	mouseIntersectExample.Camera._targ.x += OPkeyboardIsDown(OPKEY_D) * time->Elapsed * 0.01f;
	mouseIntersectExample.Camera._viewStale = 1;
	OPcamUpdateView(mouseIntersectExample.Camera);

	// Generates an OPmat4 (Matrix 4x4) which is rotated on the Y axis
	OPmat4 world = OPmat4RotY(mouseIntersectExample.Rotation / 100.0);
	OPmat4 world2 = OPmat4Translate(OPvec3Create(-5, 0, 0));
	//OPmat4Scl(&world, 0.25f, 0.25f, 0.25f);

	OPvec3 ray_wor = { 0, 0, 0};
	OPvec3 ray_Origon = { 0, 0, 0};
	OPint intersecting = 0;
	if(OPmouseIsDown(OPMOUSE_LBUTTON)) {
		// Step 0
		i32 mouseX = OPmousePositionX();
		i32 mouseY = OPmousePositionY();

		// Step 1
		OPfloat x = (((OPfloat)mouseX / (OPfloat)OPRENDER_SCALED_WIDTH) - 0.5f) * 2.0f;
		OPfloat y = -(((OPfloat)mouseY / (OPfloat)OPRENDER_SCALED_HEIGHT) - 0.5f) * 2.0f;
		OPvec4 rayStart_nds = OPvec4Create(x, y, -1.0, 1.0);
		OPvec4 rayEnd_nds = OPvec4Create(x, y, 0.0f, 1.0);


		OPvec4Log("Ray Start NDS", rayStart_nds);
		OPvec4Log("Ray End NDS", rayEnd_nds);
		OPlog("Screen %d x %d", OPRENDER_SCALED_WIDTH, OPRENDER_SCALED_HEIGHT);

		OPmat4 inverseProjectionMatrix;
		OPmat4Inverse(&inverseProjectionMatrix, mouseIntersectExample.Camera.Proj);
		OPmat4 inverseViewMatrix;
		OPmat4Inverse(&inverseViewMatrix, mouseIntersectExample.Camera.View);

		OPmat4Log("View", mouseIntersectExample.Camera.View);
		OPmat4Log("Inverse", inverseViewMatrix);


		OPvec4 rayStartCamera = OPmat4Transform(rayStart_nds, inverseProjectionMatrix);
		rayStartCamera /= rayStartCamera.w;
		OPvec4 rayStartWorld = OPmat4Transform(rayStartCamera, inverseViewMatrix);
		rayStartWorld /= rayStartWorld.w;
		OPvec4 rayEndCamera = OPmat4Transform(rayEnd_nds, inverseProjectionMatrix);
		rayEndCamera /= rayEndCamera.w;
		OPvec4 rayEndWorld = OPmat4Transform(rayEndCamera, inverseViewMatrix);
		rayEndWorld /= rayEndWorld.w;

		OPvec4 rayDirWorld = rayEndWorld - rayStartWorld;
		//rayDirWorld = OPvec4Norm(rayDirWorld);


		ray_wor = OPvec3Norm(OPvec3Create(rayDirWorld.x, rayDirWorld.y, rayDirWorld.z));

		OPvec3Log("World", ray_wor);
		OPvec4Log("Origin", rayStartWorld);
		ray_Origon = OPvec3Create(rayStartWorld.x, rayStartWorld.y, rayStartWorld.z);

		// // Step 2
		// OPvec4 ray_clip = OPvec4Create(rayStart_nds.x, rayStart_nds.y, -1.0, 1.0);
		//
		// // Step 3
		// OPmat4Log("Proj", mouseIntersectExample.Camera.Proj);
		// OPmat4Log("Inverse", inverseProjectionMatrix);
		// OPvec4 ray_eye = OPmat4Transform(ray_clip, inverseProjectionMatrix);
		// ray_eye = OPvec4Create(ray_eye.x, ray_eye.y, -1.0, 0.0);
		//
		// // Step 4
		// OPvec4 ray_wor4 = OPmat4Transform(ray_eye, inverseViewMatrix);
		// ray_wor = OPvec3Create(ray_wor4.x, ray_wor4.y, ray_wor4.z);
		// ray_wor = OPvec3Norm(ray_wor);


		{
			OPboundingBox3D box = OPboundingBox3DCreate(
				OPvec3Create(-0.5, -0.5, -0.5),
				OPvec3Create(0.5, 0.5, 0.5)
			);
			OPray3D ray = {
				ray_Origon,
				ray_wor
			};

			if(intersection(box, ray)) {
				intersecting = 1;
			}
		}

		{
			OPboundingBox3D box = OPboundingBox3DCreate(
				OPvec3Create(-0.5 - 5.0, -0.5, -0.5),
				OPvec3Create(0.5 - 5.0, 0.5, 0.5)
			);
			OPray3D ray = {
				ray_Origon,
				ray_wor
			};

			if(intersection(box, ray)) {
				intersecting = 2;
			}
		}

	}


	OPvec3 posRay = ray_Origon;//mouseIntersectExample.Camera._pos;
	posRay += ray_wor * OPvec3Len(ray_Origon);

	// OPfloat cont = posRay.z / 10.0f;
	// posRay.z = 0;


	OPmat4 world3 = OPmat4Translate(posRay);
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

	sprintf(buffer, "%f, %f, %f", ray_wor.x, ray_wor.y, ray_wor.z);
	OPfontRender(buffer, OPvec2Create(50, 120));

	OPfontRenderEnd();

	// Swaps the back buffer
	OPrenderPresent();

	// Tells the engine to continue running
	// Returning true will tell the engine to terminate
	return false;

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
	ExampleMouseIntersectExit
};
