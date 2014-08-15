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
OPmesh* physicsMeshSphere;
OPeffect* physicsEffect = NULL;
OPeffect* physicsSphereEffect = NULL;
OPcam* physicsCamera;
void* box;
void** boxes;
ui32 boxCount;
void** spheres;
ui32 sphereCount;
void* plane;
OPtexture* texture;

void ExamplePhysicsEnter(OPgameState* last) {
	OPcmanLoad("PuzzleBlock.opm");
	OPcmanLoad("Sphere.opm");
	OPcmanLoad("TexturedModel.frag");
	OPcmanLoad("TexturedModel.vert");
	OPcmanLoad("TetrisBroken.png");

	texture = (OPtexture*)OPcmanGet("TetrisBroken.png");

	physicsMesh = (OPmesh*)OPcmanGet("PuzzleBlock.opm");
	physicsMeshSphere = (OPmesh*)OPcmanGet("Sphere.opm");

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 }
	};

	physicsEffect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("TexturedModel.vert");
	OPshader* frag = (OPshader*)OPcmanGet("TexturedModel.frag");
	*physicsEffect = OPrenderCreateEffectStride(
		*vert,
		*frag,
		attribs,
		3,
		"Model Effect",
		physicsMesh->VertexSize
		);

	physicsSphereEffect = (OPeffect*)OPalloc(sizeof(OPeffect));
	*physicsSphereEffect = OPrenderCreateEffectStride(
		*vert,
		*frag,
		attribs,
		3,
		"Model Effect",
		physicsMeshSphere->VertexSize
		);

	physicsCamera = (OPcam*)OPalloc(sizeof(OPcam));
	*physicsCamera = OPcamProj(
		OPvec3One * 30.0,
		OPvec3Create(0, 1, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);

	OPphysicsInitialize();

	boxCount = 750;
	boxes = (void**)OPalloc(sizeof(void*)* boxCount);
	for (ui32 i = 0; i < boxCount; i++) {
		f32 r = OPrandom();
		f32 r2 = OPrandom();
		f32 r3 = OPrandom();
		boxes[i] = OPphysicsBoxCreate(-20 + (40 * r), 200 * r3, -20 + (40 * r2));
	}
	sphereCount = 750;
	spheres = (void**)OPalloc(sizeof(void*)* sphereCount);
	for (ui32 i = 0; i < sphereCount; i++) {
		f32 r = OPrandom();
		f32 r2 = OPrandom();
		f32 r3 = OPrandom();
		spheres[i] = OPphysicsSphereCreate(-20 + (40 * r), 200 * r3, -20 + (40 * r2));
	}
	plane = OPphysicsPlaneCreate();

}

int ExamplePhysicsUpdate(OPtimer* time) {

	OPphysicsStep();

	OPrenderDepth(1);
	OPrenderClear(0.1, 0.1, 0.1);

	OPmat4 world;

	OPrenderBindMesh(physicsMesh);
	OPrenderBindEffect(physicsEffect);

	OPtextureClearActive();
	ui32 tex = OPtextureBind(texture);

	OPmat4 view, proj;	

	OPcamGetView((*physicsCamera), &view);
	OPcamGetProj((*physicsCamera), &proj);

	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPrenderParamVec3("uLightDirection", 1, &light);
	OPrenderParami("uColorTexture", tex);

	for (ui32 i = 0; i < boxCount; i++) {
		OPphysicsGetTransform(boxes[i], &world);
		OPrenderParamMat4v("uWorld", 1, &world);
		OPrenderMesh();
	}

	OPrenderBindMesh(physicsMeshSphere);
	OPrenderBindEffect(physicsSphereEffect);
	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);
	OPrenderParamVec3("uLightDirection", 1, &light);
	OPrenderParami("uColorTexture", tex);
	for (ui32 i = 0; i < sphereCount; i++) {
		OPphysicsGetTransform(spheres[i], &world);
		OPrenderParamMat4v("uWorld", 1, &world);
		OPrenderMesh();
	}

	OPrenderPresent();
	return false;
}

void ExamplePhysicsExit(OPgameState* next) {
	OPphysicsDestroy();
}