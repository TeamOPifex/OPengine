#include "../ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPtextureCube.h"


// Data for this Game State Example
typedef struct {
    OPmesh Mesh;			// The Mesh to render
    OPeffect Effect;		// The Effect used to render the Mesh
    OPcam Camera;			// The Camera to use in the Effect to render the Mesh
    ui32 Rotation;			// The amount to rotate the Mesh
    OPtextureCube CubeMap;
    OPsphericalCube SphericalCube;
} CubeMapExample;

CubeMapExample cubeMapExample;

void ExampleCubeMapEnter(OPgameState* last) {

    cubeMapExample.Mesh = OPcubeCreate(OPvec3Create(1,0,0));
    cubeMapExample.SphericalCube = OPsphericalCubeCreate(10);

    cubeMapExample.Effect = OPeffectGen(
            "CubeMap.vert",
            "CubeMap.frag",
            OPATTR_POSITION,
            "Cube Map Effect",
            cubeMapExample.SphericalCube.sides[0].VertexSize);

    cubeMapExample.Camera = OPcamPersp(
            OPVEC3_ONE,
            OPVEC3_ZERO,
            OPVEC3_UP,
            0.1f,
            1000.0f,
            45.0f,
            OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
    );

    OPimagePNG faces[6];
    faces[0] = OPimagePNGLoadData("Textures/TetrisBroken.png");
    faces[1] = OPimagePNGLoadData("Textures/TetrisGreen.png");
    faces[2] = OPimagePNGLoadData("Textures/TetrisGray.png");
    faces[3] = OPimagePNGLoadData("Textures/TetrisOrange.png");
    faces[4] = OPimagePNGLoadData("Textures/TetrisRed.png");
    faces[5] = OPimagePNGLoadData("Textures/TetrisFlawed.png");
    OPtextureCubeInit(&cubeMapExample.CubeMap, faces);

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

    if (OPkeyboardIsDown(OPKEY_SPACE)) { cubeMapExample.Rotation++; }

    // Generates an OPmat4 (Matrix 4x4) which is rotated on the Y axis
    OPmat4 world = OPmat4RotY(cubeMapExample.Rotation / 100.0);
    OPmat4Scl(&world, 0.25f, 0.25f, 0.25f);


    ////////////////////////
    // Render
    ////////////////////////
    OPrenderClear(0.4, 0.4, 0.4);

//    OPbindMeshEffectWorldCam(&cubeMapExample.Mesh, &cubeMapExample.Effect, &world, &cubeMapExample.Camera);
//
//    OPmeshRender();

    for (OPint i = 0; i < 6; i++) {
        // A helper utility which binds the Mesh, Effect and the World, View and Projection Matrices
        // For more granular control please take a look at the Textured Example
        OPbindMeshEffectWorldCam(&cubeMapExample.SphericalCube.sides[i], &cubeMapExample.Effect, &world, &cubeMapExample.Camera);

        OPtextureCubeClearActive();
        OPeffectParam("uColorTexture", &cubeMapExample.CubeMap);

        // Renders to the screen the currently bound Mesh (sphericalCubeExample->Mesh)
        OPmeshRender();
    }

    OPrenderPresent();

    // Tells the engine to continue running
    // Returning true will tell the engine to terminate
    return false;

}

// The OPifex Engine will call this itself when you call OPgameStateChange
OPint ExampleCubeMapExit(OPgameState* next) {
    // Clean up phase for the Game State
    OPeffectUnload(&cubeMapExample.Effect);
    return 0;
}

// This is for the Example Selector only
OPint GS_EXAMPLE_CUBE_MAP_AVAILABLE = 1;

// This is the Game State for this ModelExample
// Each entry is a function pointer for Initialize, Update, Destroy
OPgameState GS_EXAMPLE_CUBE_MAP = {
        ExampleCubeMapEnter,
        ExampleCubeMapUpdate,
        ExampleCubeMapExit
};