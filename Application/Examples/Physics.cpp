#include "./Examples/Physics.h"
#include "./Human/include/Systems/RenderSystem.h"
#include "./Performance/include/Physics.h"

#include "./Data/include/ContentManager.h"

OPgameState GS_EXAMPLE_PHYSICS = {
	ExamplePhysicsEnter,
	ExamplePhysicsUpdate,
	ExamplePhysicsExit
};

OPmesh* physicsMesh;
OPeffect* physicsEffect = NULL;
OPcam* physicsCamera;
void* box;
void* plane;

void ExamplePhysicsEnter(OPgameState* last) {
	OPcmanLoad("PuzzleBlock.opm");
	OPcmanLoad("SimpleModel.frag");
	OPcmanLoad("SimpleModel.vert");

	physicsMesh = (OPmesh*)OPcmanGet("PuzzleBlock.opm");

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 }
	};

	physicsEffect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("SimpleModel.vert");
	OPshader* frag = (OPshader*)OPcmanGet("SimpleModel.frag");
	*physicsEffect = OPrenderCreateEffectStride(
		*vert,
		*frag,
		attribs,
		2,
		"Model Effect",
		physicsMesh->VertexSize
		);

	physicsCamera = (OPcam*)OPalloc(sizeof(OPcam));
	*physicsCamera = OPcamProj(
		OPvec3One * 2.0,
		OPvec3Create(0, 1, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);

	OPphysicsInitialize();

	box = OPphysicsBoxCreate();
	plane = OPphysicsPlaneCreate();


	OPmat4 matrix;
	OPphysicsGetTransform(box, &matrix);
}

int ExamplePhysicsUpdate(OPtimer* time) {

	OPphysicsStep();

	OPrenderDepth(1);
	OPrenderClear(0, 0, 0);

	OPmat4 world;
	OPphysicsGetTransform(box, &world);

	OPrenderBindMesh(physicsMesh);
	OPrenderBindEffect(physicsEffect);

	OPmat4 view, proj;	

	OPcamGetView((*physicsCamera), &view);
	OPcamGetProj((*physicsCamera), &proj);

	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPrenderParamVec3("vLightDirection", 1, &light);

	OPrenderMesh();

	OPrenderPresent();
	return false;
}

void ExamplePhysicsExit(OPgameState* next) {
	OPphysicsDestroy();
}