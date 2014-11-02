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

typedef struct {
	OPmesh* Mesh;
	OPmesh* MeshSphere;
	OPeffect* Effect;
	OPeffect* SphereEffect;
	OPcam* Camera;
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
} PhysicsExample;

PhysicsExample* physicsExample;


void ExamplePhysicsEnter(OPgameState* last) {

	physicsExample = (PhysicsExample*)OPalloc(sizeof(PhysicsExample));

	OPphysicsInit();
	physicsExample->scene = OPphysicsCreateScene();

	OPcmanLoad("PuzzleBlock.opm");
	OPcmanLoad("PuzzleSphere.opm");
	OPcmanLoad("TexturedModel.frag");
	OPcmanLoad("TexturedModel.vert");
	OPcmanLoad("TetrisBroken.png");
	OPcmanLoad("TetrisOrange.png");
	OPcmanLoad("TetrisBlue.png");
	OPcmanLoad("TetrisGreen.png");

	physicsExample->texture = (OPtexture*)OPcmanGet("TetrisBroken.png");
	physicsExample->texturePlayer = (OPtexture*)OPcmanGet("TetrisOrange.png");
	physicsExample->textureSphere = (OPtexture*)OPcmanGet("TetrisBlue.png");
	physicsExample->textureStatic = (OPtexture*)OPcmanGet("TetrisGreen.png");

	physicsExample->Mesh = (OPmesh*)OPcmanGet("PuzzleBlock.opm");
	physicsExample->MeshSphere = (OPmesh*)OPcmanGet("PuzzleSphere.opm");

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 }
	};

	physicsExample->Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("TexturedModel.vert");
	OPshader* frag = (OPshader*)OPcmanGet("TexturedModel.frag");
	*physicsExample->Effect = OPrenderCreateEffectStride(
		*vert,
		*frag,
		attribs,
		3,
		"Model Effect",
		physicsExample->Mesh->VertexSize
		);

	physicsExample->SphereEffect = (OPeffect*)OPalloc(sizeof(OPeffect));
	*physicsExample->SphereEffect = OPrenderCreateEffectStride(
		*vert,
		*frag,
		attribs,
		3,
		"Model Effect",
		physicsExample->MeshSphere->VertexSize
		);

	physicsExample->Camera = (OPcam*)OPalloc(sizeof(OPcam));
	*physicsExample->Camera = OPcamProj(
		OPvec3Create(0, 40, 20),
		OPvec3Create(0, 1, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPrenderWidth / (f32)OPrenderHeight
		);

	physicsExample->boxCount = 2000;
	physicsExample->boxes = (Dynamic*)OPalloc(sizeof(Dynamic)* physicsExample->boxCount);
	for (ui32 i = 0; i < physicsExample->boxCount; i++) {
		f32 r = OPrandom();
		f32 r2 = 0.5;// OPrandom();
		f32 r3 = OPrandom();
		f32 size = 0.05f + (OPrandom() * 2);
		physicsExample->boxes[i].physics = OPphysicsCreateBoxDynamic(physicsExample->scene, -20 + (40 * r), 5 + 200 * r3, -20 + (40 * r2), size, size, size);
		physicsExample->boxes[i].size = size;
	}

	physicsExample->boxStaticCount = 10;
	physicsExample->boxesStatic = (Static*)OPalloc(sizeof(Static)*physicsExample->boxStaticCount);
	for (ui32 i = 0; i < physicsExample->boxStaticCount; i++) {
		f32 r = OPrandom();
		f32 r2 = 0.5;// OPrandom();
		f32 r3 = OPrandom();
		f32 size = 1;// 0.05f + (OPrandom() * 2);
		physicsExample->boxesStatic[i].physics = OPphysicsCreateBoxStatic(physicsExample->scene, ((-10.0) + (i * 2)), i * 2, -20 + (40 * r2), size, size, size);
		physicsExample->boxesStatic[i].size = size;
	}

	physicsExample->sphereCount = 5;
	physicsExample->spheres = (Dynamic*)OPalloc(sizeof(Dynamic)* physicsExample->sphereCount);
	physicsExample->spheres[0].physics = OPphysicsCreateSphereDynamic(physicsExample->scene, 0, 0, 0, 1);
	physicsExample->spheres[0].size = 1;
	for (ui32 i = 1; i < physicsExample->sphereCount; i++) {
		f32 r = OPrandom();
		f32 r2 = OPrandom();
		f32 r3 = OPrandom();
		f32 size = 0.1f + (OPrandom() * 2);
		physicsExample->spheres[i].physics = OPphysicsCreateSphereDynamic(physicsExample->scene, -20 + (40 * r), 3 * r3, -20 + (40 * r2), size);
		physicsExample->spheres[i].size = size;
	}
	physicsExample->physicsPlane = OPphysicsCreatePlane(physicsExample->scene);

}

OPint ExamplePhysicsUpdate(OPtimer* time) {

	if (OPkeyboardWasPressed(OPKEY_SPACE)) {
		OPphysicsAddForce(physicsExample->spheres[0].physics, 0, 1500 * physicsExample->spheres[0].size, 0);
	}
	f32 rate = 500 * physicsExample->spheres[0].size;
	f32 rate2 = 50 * physicsExample->spheres[0].size;
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
		OPphysicsAddTorque(physicsExample->spheres[0].physics, 0, 0, rate);
		OPphysicsAddForce(physicsExample->spheres[0].physics, -rate2, 0, 0);
	}
	if (OPkeyboardIsDown(OPKEY_D)) {
		OPphysicsAddTorque(physicsExample->spheres[0].physics, 0, 0, -rate);
		OPphysicsAddForce(physicsExample->spheres[0].physics, rate2, 0, 0);
	}
	if (OPkeyboardIsDown(OPKEY_W)) {
		OPphysicsAddTorque(physicsExample->spheres[0].physics, -rate, 0, 0);
		OPphysicsAddForce(physicsExample->spheres[0].physics, 0, 0, -rate2);
	}
	if (OPkeyboardIsDown(OPKEY_S)) {
		OPphysicsAddTorque(physicsExample->spheres[0].physics, rate, 0, 0);
		OPphysicsAddForce(physicsExample->spheres[0].physics, 0, 0, rate2);
	}

	OPphysicsStep(physicsExample->scene, time->Elapsed);

	OPrenderDepth(1);
	OPrenderClear(0.1, 0.1, 0.1);

	OPmat4 world;

	OPrenderBindMesh(physicsExample->Mesh);
	OPrenderBindEffect(physicsExample->Effect);

	OPtextureClearActive();
	ui32 tex = OPtextureBind(physicsExample->texture);
	ui32 tex2 = OPtextureBind(physicsExample->texturePlayer);
	ui32 tex3 = OPtextureBind(physicsExample->textureSphere);
	ui32 tex4 = OPtextureBind(physicsExample->textureStatic);

	OPmat4 view, proj;	

	OPcamGetView((*physicsExample->Camera), &view);
	OPcamGetProj((*physicsExample->Camera), &proj);

	OPrenderParamMat4("uProj", &proj);
	OPrenderParamMat4("uView", &view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPrenderParamVec3("uLightDirection", &light);
	OPrenderParami("uColorTexture", tex);
	OPmat4 scale;
	OPmat4 scratch;
	for (ui32 i = 0; i < physicsExample->boxCount; i++) {
		OPmat4buildScl(&scale, physicsExample->boxes[i].size * 2, physicsExample->boxes[i].size * 2, physicsExample->boxes[i].size * 2);
		OPphysicsGetTransform((OPphysicsActor*)physicsExample->boxes[i].physics, &scratch);
		OPmat4mul(&world, &scale, &scratch);
		OPrenderParamMat4("uWorld", &world);
		OPrenderMesh();
	}
	OPrenderParami("uColorTexture", tex4);
	for (ui32 i = 0; i < physicsExample->boxStaticCount; i++) {
		OPmat4buildScl(&scale, physicsExample->boxesStatic[i].size * 2, physicsExample->boxesStatic[i].size * 2, physicsExample->boxesStatic[i].size * 2);
		OPphysicsGetTransform((OPphysicsActor*)physicsExample->boxesStatic[i].physics, &scratch);
		OPmat4mul(&world, &scale, &scratch);
		OPrenderParamMat4("uWorld", &world);
		OPrenderMesh();
	}

	OPrenderBindMesh(physicsExample->MeshSphere);
	OPrenderBindEffect(physicsExample->SphereEffect);
	OPrenderParamMat4("uProj", &proj);
	OPrenderParamMat4("uView", &view);
	OPrenderParamVec3("uLightDirection", &light);

	OPrenderParami("uColorTexture", tex2);
	OPmat4buildScl(&scale, physicsExample->spheres[0].size * 2, physicsExample->spheres[0].size * 2, physicsExample->spheres[0].size * 2);
	OPphysicsGetTransform((OPphysicsActor*)physicsExample->spheres[0].physics, &scratch);
	OPmat4mul(&world, &scale, &scratch);
	OPrenderParamMat4("uWorld", &world);
	OPrenderMesh();

	OPrenderParami("uColorTexture", tex3);
	for (ui32 i = 1; i < physicsExample->sphereCount; i++) {
		OPmat4buildScl(&scale, physicsExample->spheres[i].size * 2, physicsExample->spheres[i].size * 2, physicsExample->spheres[i].size * 2);
		OPphysicsGetTransform((OPphysicsActor*)physicsExample->spheres[i].physics, &scratch);
		OPmat4mul(&world, &scale, &scratch);
		OPrenderParamMat4v("uWorld", 1, &world);
		OPrenderMesh();
	}

	OPrenderPresent();
	return false;
}

void ExamplePhysicsExit(OPgameState* next) {
	OPphysicsDestroy(physicsExample->scene);

	OPfree(physicsExample);
}