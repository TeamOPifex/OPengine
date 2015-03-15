#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Performance/include/OPphysics.h"
#include "./Human/include/Systems/OPinputSystem.h"

#include "./Data/include/OPcman.h"


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
	OPcmanLoad("Common/Texture.frag");
	OPcmanLoad("Common/Texture3D.vert");
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
	OPshader* vert = (OPshader*)OPcmanGet("Common/Texture3D.vert");
	OPshader* frag = (OPshader*)OPcmanGet("Common/Texture.frag");
	*physicsExample->Effect = OPeffectCreate(
		*vert,
		*frag,
		attribs,
		3,
		"Model Effect",
		physicsExample->Mesh->VertexSize
		);

	physicsExample->SphereEffect = (OPeffect*)OPalloc(sizeof(OPeffect));
	*physicsExample->SphereEffect = OPeffectCreate(
		*vert,
		*frag,
		attribs,
		3,
		"Model Effect",
		physicsExample->MeshSphere->VertexSize
		);

	physicsExample->Camera = (OPcam*)OPalloc(sizeof(OPcam));
	*physicsExample->Camera = OPcamPersp(
		OPvec3Create(0, 20, 30),
		OPvec3Create(0, 1, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
		);

	physicsExample->boxCount = 200;
	physicsExample->boxes = (Dynamic*)OPalloc(sizeof(Dynamic)* physicsExample->boxCount);
	for (ui32 i = 0; i < physicsExample->boxCount; i++) {
		f32 r = OPrandom();
		f32 r2 = 0.5;// OPrandom();
		f32 r3 = OPrandom();
		f32 size = (0.05f + (OPrandom() * 2));
		physicsExample->boxes[i].physics = OPphysicsCreateBoxDynamic(physicsExample->scene, -20 + (40 * r), 5 + 200 * r3, -20 + (40 * r2), size, size, size);
		physicsExample->boxes[i].size = size;
	}

	physicsExample->boxStaticCount = 10;
	physicsExample->boxesStatic = (Static*)OPalloc(sizeof(Static)*physicsExample->boxStaticCount);
	for (ui32 i = 0; i < physicsExample->boxStaticCount; i++) {
		f32 r2 = 0.5;
		f32 size = 1;// 0.05f + (OPrandom() * 2);
		physicsExample->boxesStatic[i].physics = OPphysicsCreateBoxStatic(physicsExample->scene, ((-10.0) + (i * 2)), i * 2, -20 + (40 * r2), size, size, size);
		physicsExample->boxesStatic[i].size = size;
	}

	physicsExample->sphereCount = 5;
	physicsExample->spheres = (Dynamic*)OPalloc(sizeof(Dynamic)* physicsExample->sphereCount);
	physicsExample->spheres[0].physics = OPphysicsCreateSphereDynamic(physicsExample->scene, 0, 0, 0, 0.5);
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
		OPphysicsAddForce(physicsExample->spheres[0].physics, 0, 500 * physicsExample->spheres[0].size, 0);
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

	if (OPkeyboardIsDown(OPKEY_UP)) { physicsExample->Camera->_pos.y += 0.2; }
	if (OPkeyboardIsDown(OPKEY_DOWN)) { physicsExample->Camera->_pos.y -= 0.2; }
	if (OPkeyboardIsDown(OPKEY_LEFT)) { physicsExample->Camera->_pos.x -= 0.2; }
	if (OPkeyboardIsDown(OPKEY_RIGHT)) { physicsExample->Camera->_pos.x += 0.2; }

	physicsExample->Camera->_viewStale = 1;
	OPcamUpdateView((*physicsExample->Camera));


	OPphysicsStep(physicsExample->scene, time->Elapsed);

	OPrenderDepth(1);
	OPrenderClear(0.1, 0.1, 0.1);

	OPmat4 world = OPMAT4IDENTITY;

	OPmeshBind(physicsExample->Mesh);
	OPeffectBind(physicsExample->Effect);

	OPtextureClearActive();
	ui32 tex = OPtextureBind(physicsExample->texture);
	ui32 tex2 = OPtextureBind(physicsExample->texturePlayer);
	ui32 tex3 = OPtextureBind(physicsExample->textureSphere);
	ui32 tex4 = OPtextureBind(physicsExample->textureStatic);

	OPmat4 view, proj;	

	OPcamGetView((*physicsExample->Camera), &view);
	OPcamGetProj((*physicsExample->Camera), &proj);

	OPeffectParamMat4("uProj", &proj);
	OPeffectParamMat4("uView", &view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPeffectParamVec3("uLightDirection", &light);
	OPeffectParami("uColorTexture", tex);
	OPmat4 scale = OPMAT4IDENTITY;
	OPmat4 scratch = OPMAT4IDENTITY;
	for (ui32 i = 0; i < physicsExample->boxCount; i++) {
		scale = OPmat4Scl(physicsExample->boxes[i].size * 2, physicsExample->boxes[i].size * 2, physicsExample->boxes[i].size * 2);
		OPphysicsGetTransform((OPphysicsActor*)physicsExample->boxes[i].physics, &scratch);
		world = scratch * scale;
		OPeffectParamMat4("uWorld", &world);
		OPmeshRender();
	}
	OPeffectParami("uColorTexture", tex4);
	for (ui32 i = 0; i < physicsExample->boxStaticCount; i++) {
		scale = OPmat4Scl(physicsExample->boxesStatic[i].size * 2, physicsExample->boxesStatic[i].size * 2, physicsExample->boxesStatic[i].size * 2);
		OPphysicsGetTransform((OPphysicsActor*)physicsExample->boxesStatic[i].physics, &scratch);
		world = scratch * scale;
		OPeffectParamMat4("uWorld", &world);
		OPmeshRender();
	}

	OPmeshBind(physicsExample->MeshSphere);
	OPeffectBind(physicsExample->SphereEffect);
	OPeffectParamMat4("uProj", &proj);
	OPeffectParamMat4("uView", &view);
	OPeffectParamVec3("uLightDirection", &light);

	OPeffectParami("uColorTexture", tex2);
	scale = OPmat4Scl(physicsExample->spheres[0].size * 2, physicsExample->spheres[0].size * 2, physicsExample->spheres[0].size * 2);
	OPphysicsGetTransform((OPphysicsActor*)physicsExample->spheres[0].physics, &scratch);
	world = scratch * scale;
	OPeffectParamMat4("uWorld", &world);
	OPmeshRender();

	OPeffectParami("uColorTexture", tex3);
	for (ui32 i = 1; i < physicsExample->sphereCount; i++) {
		scale = OPmat4Scl(physicsExample->spheres[i].size * 2, physicsExample->spheres[i].size * 2, physicsExample->spheres[i].size * 2);
		OPphysicsGetTransform((OPphysicsActor*)physicsExample->spheres[i].physics, &scratch);
		world = scratch * scale;
		OPeffectParamMat4v("uWorld", 1, &world);
		OPmeshRender();
	}

	OPrenderPresent();
	return false;
}

OPint ExamplePhysicsExit(OPgameState* next) {
	OPphysicsDestroy(physicsExample->scene);

	OPfree(physicsExample);
	return 0;
}

OPgameState GS_EXAMPLE_PHYSICS = {
	ExamplePhysicsEnter,
	ExamplePhysicsUpdate,
	ExamplePhysicsExit
};