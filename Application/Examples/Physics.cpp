#include "./Examples/Physics.h"
#include "./Human/include/Systems/RenderSystem.h"
#include "./Performance/include/Physics.h"
#include "./Human/include/Input/Input.h"

#include "./Data/include/ContentManager.h"

OPgameState GS_EXAMPLE_PHYSICS = {
	ExamplePhysicsEnter,
	ExamplePhysicsUpdate,
	ExamplePhysicsExit
};

typedef struct {
	OPphysicsDynamic* physics;
	f32 size;
} Dynamic;
typedef struct {
	OPphysicsStatic* physics;
	f32 size;
} Static;

OPmesh* physicsMesh;
OPmesh* physicsMeshSphere;
OPeffect* physicsEffect = NULL;
OPeffect* physicsSphereEffect = NULL;
OPcam* physicsCamera;
void* box;
Dynamic* boxes;
ui32 boxCount;
Static* boxesStatic;
ui32 boxStaticCount;
Dynamic* spheres;
ui32 sphereCount;
void* physicsPlane;
OPtexture* texture;
OPtexture* textureStatic;
OPtexture* texturePlayer;
OPtexture* textureSphere;
OPphysicsScene* scene;



void ExamplePhysicsEnter(OPgameState* last) {
	OPcmanLoad("PuzzleBlock.opm");
	OPcmanLoad("PuzzleSphere.opm");
	OPcmanLoad("TexturedModel.frag");
	OPcmanLoad("TexturedModel.vert");
	OPcmanLoad("TetrisBroken.png");
	OPcmanLoad("TetrisOrange.png");
	OPcmanLoad("TetrisBlue.png");
	OPcmanLoad("TetrisGreen.png");

	texture = (OPtexture*)OPcmanGet("TetrisBroken.png");
	texturePlayer = (OPtexture*)OPcmanGet("TetrisOrange.png");
	textureSphere = (OPtexture*)OPcmanGet("TetrisBlue.png");
	textureStatic = (OPtexture*)OPcmanGet("TetrisGreen.png");

	physicsMesh = (OPmesh*)OPcmanGet("PuzzleBlock.opm");
	physicsMeshSphere = (OPmesh*)OPcmanGet("PuzzleSphere.opm");

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
		OPvec3Create(0, 40, 20),
		OPvec3Create(0, 1, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);

	OPphysicsInit();
	scene = OPphysicsCreateScene();

	boxCount = 5;
	boxes = (Dynamic*)OPalloc(sizeof(Dynamic)* boxCount);
	for (ui32 i = 0; i < boxCount; i++) {
		f32 r = OPrandom();
		f32 r2 = 0.5;// OPrandom();
		f32 r3 = OPrandom();
		f32 size = 0.05f + (OPrandom() * 2);
		boxes[i].physics = OPphysicsCreateBoxDynamic(scene, -20 + (40 * r), 5 + 200 * r3, -20 + (40 * r2), size, size, size);
		boxes[i].size = size;
	}

	boxStaticCount = 10;
	boxesStatic = (Static*)OPalloc(sizeof(Static)* boxStaticCount);
	for (ui32 i = 0; i < boxStaticCount; i++) {
		f32 r = OPrandom();
		f32 r2 = 0.5;// OPrandom();
		f32 r3 = OPrandom();
		f32 size = 1;// 0.05f + (OPrandom() * 2);
		boxesStatic[i].physics = OPphysicsCreateBoxStatic(scene, ((-10.0) + (i * 2)), i * 2, -20 + (40 * r2), size, size, size);
		boxesStatic[i].size = size;
	}

	sphereCount = 5;
	spheres = (Dynamic*)OPalloc(sizeof(Dynamic)* sphereCount);
	spheres[0].physics = OPphysicsCreateSphereDynamic(scene, 0, 0, 0, 1);
	spheres[0].size = 1;
	for (ui32 i = 1; i < sphereCount; i++) {
		f32 r = OPrandom();
		f32 r2 = OPrandom();
		f32 r3 = OPrandom();
		f32 size = 0.1f + (OPrandom() * 2);
		spheres[i].physics = OPphysicsCreateSphereDynamic(scene, -20 + (40 * r), 3 * r3, -20 + (40 * r2), size);
		spheres[i].size = size;
	}
	physicsPlane = OPphysicsCreatePlane(scene);

}

int ExamplePhysicsUpdate(OPtimer* time) {

	OPkeyboardUpdate();
	if (OPkeyboardWasPressed(OPKEY_SPACE)) {
		OPphysicsAddForce(spheres[0].physics, 0, 1500 * spheres[0].size, 0);
	}
	f32 rate = 500 * spheres[0].size;
	f32 rate2 = 50 * spheres[0].size;
	//if (OPkeyboardWasPressed(OPKEY_A)) {
	//	OPphysicsSetLinearVelocity(spheres[0].physics, -rate / 100, 0, 0);
	//}
	//if (OPkeyboardWasPressed(OPKEY_D)) {
	//	OPphysicsSetLinearVelocity(spheres[0].physics, rate / 100, 0, 0);
	//}
	//if (OPkeyboardWasPressed(OPKEY_W)) {
	//	OPphysicsSetLinearVelocity(spheres[0].physics, 0, 0, -rate / 100);
	//}
	//if (OPkeyboardWasPressed(OPKEY_S)) {
	//	OPphysicsSetLinearVelocity(spheres[0].physics, 0, 0, rate / 100);
	//}

	if (OPkeyboardIsDown(OPKEY_A)) {
		OPphysicsAddTorque(spheres[0].physics, 0, 0, rate);
		OPphysicsAddForce(spheres[0].physics, -rate2, 0, 0);
	}
	if (OPkeyboardIsDown(OPKEY_D)) {
		OPphysicsAddTorque(spheres[0].physics, 0, 0, -rate);
		OPphysicsAddForce(spheres[0].physics, rate2, 0, 0);
	}
	if (OPkeyboardIsDown(OPKEY_W)) {
		OPphysicsAddTorque(spheres[0].physics, -rate, 0, 0);
		OPphysicsAddForce(spheres[0].physics, 0, 0, -rate2);
	}
	if (OPkeyboardIsDown(OPKEY_S)) {
		OPphysicsAddTorque(spheres[0].physics, rate, 0, 0);
		OPphysicsAddForce(spheres[0].physics, 0, 0, rate2);
	}

	OPphysicsStep(scene, time->Elapsed);

	OPrenderDepth(1);
	OPrenderClear(0.1, 0.1, 0.1);

	OPmat4 world;

	OPrenderBindMesh(physicsMesh);
	OPrenderBindEffect(physicsEffect);

	OPtextureClearActive();
	ui32 tex = OPtextureBind(texture);
	ui32 tex2 = OPtextureBind(texturePlayer);
	ui32 tex3 = OPtextureBind(textureSphere);
	ui32 tex4 = OPtextureBind(textureStatic);

	OPmat4 view, proj;	

	OPcamGetView((*physicsCamera), &view);
	OPcamGetProj((*physicsCamera), &proj);

	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPrenderParamVec3("uLightDirection", 1, &light);
	OPrenderParami("uColorTexture", tex);
	OPmat4 scale;
	OPmat4 scratch;
	for (ui32 i = 0; i < boxCount; i++) {
		OPmat4buildScl(&scale, boxes[i].size * 2, boxes[i].size * 2, boxes[i].size * 2);
		OPphysicsGetTransform((OPphysicsActor*)boxes[i].physics, &scratch);
		OPmat4mul(&world, &scale, &scratch);
		OPrenderParamMat4v("uWorld", 1, &world);
		OPrenderMesh();
	}
	OPrenderParami("uColorTexture", tex4);
	for (ui32 i = 0; i < boxStaticCount; i++) {
		OPmat4buildScl(&scale, boxesStatic[i].size * 2, boxesStatic[i].size * 2, boxesStatic[i].size * 2);
		OPphysicsGetTransform((OPphysicsActor*)boxesStatic[i].physics, &scratch);
		OPmat4mul(&world, &scale, &scratch);
		OPrenderParamMat4v("uWorld", 1, &world);
		OPrenderMesh();
	}

	OPrenderBindMesh(physicsMeshSphere);
	OPrenderBindEffect(physicsSphereEffect);
	OPrenderParamMat4v("uProj", 1, &proj);
	OPrenderParamMat4v("uView", 1, &view);
	OPrenderParamVec3("uLightDirection", 1, &light);

	OPrenderParami("uColorTexture", tex2);
	OPmat4buildScl(&scale, spheres[0].size * 2, spheres[0].size * 2, spheres[0].size * 2);
	OPphysicsGetTransform((OPphysicsActor*)spheres[0].physics, &scratch);
	OPmat4mul(&world, &scale, &scratch);
	OPrenderParamMat4v("uWorld", 1, &world);
	OPrenderMesh();

	OPrenderParami("uColorTexture", tex3);
	for (ui32 i = 1; i < sphereCount; i++) {
		OPmat4buildScl(&scale, spheres[i].size * 2, spheres[i].size * 2, spheres[i].size * 2);
		OPphysicsGetTransform((OPphysicsActor*)spheres[i].physics, &scratch);
		OPmat4mul(&world, &scale, &scratch);
		OPrenderParamMat4v("uWorld", 1, &world);
		OPrenderMesh();
	}

	OPrenderPresent();
	return false;
}

void ExamplePhysicsExit(OPgameState* next) {
	OPphysicsDestroy(scene);
}