#include "../ExampleSelectorState.h"

// Data for this Game State Example
typedef struct {
    OPmesh Mesh;			// The Mesh to render
    OPeffect Effect;		// The Effect used to render the Mesh
    OPcam Camera;			// The Camera to use in the Effect to render the Mesh
    ui32 Rotation;			// The amount to rotate the Mesh
    OPtextureCubeOLD CubeMap;
    OPsphericalCube SphericalCube;
} CubeMapExample;
CubeMapExample cubeMapExample;

void ExampleCubeMapEnter(OPgameState* last) {

    OPimage faces[6];
    faces[0] = OPimagePNGLoadData("Textures/subtle-irongrip2.png");
    faces[1] = OPimagePNGLoadData("Textures/subtle-irongrip2.png");
    faces[2] = OPimagePNGLoadData("Textures/subtle-irongrip2.png");
    faces[3] = OPimagePNGLoadData("Textures/subtle-irongrip2.png");
    faces[4] = OPimagePNGLoadData("Textures/subtle-irongrip2.png");
    faces[5] = OPimagePNGLoadData("Textures/subtle-irongrip2.png");
    OPtextureCubeInit(&cubeMapExample.CubeMap, faces);

    OPimage faces2[6];
    faces2[0] = OPimagePNGLoadData("Textures/cubeTex.png");
    faces2[1] = OPimagePNGLoadData("Textures/cubeTex.png");
    faces2[2] = OPimagePNGLoadData("Textures/cubeTex.png");
    faces2[3] = OPimagePNGLoadData("Textures/cubeTex.png");
    faces2[4] = OPimagePNGLoadData("Textures/cubeTex.png");
    faces2[5] = OPimagePNGLoadData("Textures/cubeTex.png");

    cubeMapExample.Mesh = OPcubeCreate(OPvec3Create(1,0,0));
    cubeMapExample.SphericalCube = OPsphericalCubeCreate(faces2);

    cubeMapExample.Effect.Init("CubeMap.vert", "CubeMap.frag");

    cubeMapExample.Camera.SetPerspective(
        OPVEC3_ONE, OPVEC3_ZERO, OPVEC3_UP,
        0.1f, 1000.0f, 45.0f,
        OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
    );

    // This can be controlled in the update loop if it varies
    // Since this is a simple example we'll ensure that it's set
    // to turn the Depth Buffer on and continue
    OPrenderDepth(1);
    OPrenderCull(0);
}

OPint ExampleCubeMapUpdate(OPtimer* time) {

    ////////////////////////
    // Update
    ////////////////////////

    if (OPkeyboardIsDown(OPkeyboardKey::SPACE)) { cubeMapExample.Rotation++; }



    return false;

}

void ExampleCubeMapRender(OPfloat delta) {

	// Generates an OPmat4 (Matrix 4x4) which is rotated on the Y axis
	OPmat4 world = OPmat4RotY(cubeMapExample.Rotation / 100.0f);
	//OPmat4Scl(&world, 0.5f, 0.5f, 0.5f);

	////////////////////////
	// Render
	////////////////////////
	OPrenderClear(0.4f, 0.4f, 0.4f);

	for (OPint i = 0; i < 6; i++) {
		// A helper utility which binds the Mesh, Effect and the World, View and Projection Matrices
		// For more granular control please take a look at the Textured Example
		OPbindMeshEffectWorldCam(&cubeMapExample.SphericalCube.sides[i], &cubeMapExample.Effect, &world, &cubeMapExample.Camera);

		OPtextureCubeClearActive();
		//OPeffectSet("uColorTexture", &cubeMapExample.CubeMap);

		// Renders to the screen the currently bound Mesh (sphericalCubeExample->Mesh)
		OPmeshRender();
	}

	OPrenderPresent();
}

OPint ExampleCubeMapExit(OPgameState* next) {
    cubeMapExample.Effect.Destroy();
    return 0;
}

OPint GS_EXAMPLE_CUBE_MAP_AVAILABLE = 1;
OPgameState GS_EXAMPLE_CUBE_MAP = {
        ExampleCubeMapEnter,
        ExampleCubeMapUpdate,
	ExampleCubeMapRender,
        ExampleCubeMapExit
};
