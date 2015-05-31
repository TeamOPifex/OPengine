#include "./ExampleSelectorState.h"

#ifdef OPIFEX_OPTION_PHYSICS
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Performance/include/OPphysics.h"
#include "./Human/include/Systems/OPinputSystem.h"

#include "./Data/include/OPcman.h"
#include "characterkinematic/PxControllerManager.h"

typedef struct {
	OPphysicsDynamic* physics;
	f32 size;
} Dynamic;

typedef struct {
	OPphysicsStatic* physics;
	f32 size;
	OPint dead;
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
	OPphysicsStatic* physicsPlane;
	OPtexture* texture;
	OPtexture* textureStatic;
	OPtexture* texturePlayer;
	OPtexture* textureSphere;
	OPphysicsScene* scene;
	PxControllerManager* controllerManager;
	PxController* controller;
} PhysicsCharacterExample;

PhysicsCharacterExample physicsCharacterExample;

void ExamplePhysicsCharacterOnTrigger(OPphysicsTrigger pairs) {
	for(OPuint i = 0; i < pairs.count; i++) {
		if(pairs.triggerPairs[i].triggerActor == physicsCharacterExample.spheres[0].physics->actor &&
			pairs.triggerPairs[i].otherActor != physicsCharacterExample.physicsPlane->actor) {
			if(physicsCharacterExample.boxStaticCount == 0) continue;
			if(pairs.triggerPairs[i].otherActor == physicsCharacterExample.boxesStatic[0].physics->actor) {
				physicsCharacterExample.boxesStatic[0].dead = 1;
				OPlog("User Defined Trigger Box");
			}
		}
	}
}
#endif

void ExamplePhysicsCharacterEnter(OPgameState* last) {
#ifdef OPIFEX_OPTION_PHYSICS

	OPphysicsInit();
	physicsCharacterExample.scene = OPphysicsCreateScene(ExamplePhysicsCharacterOnTrigger);

	OPcmanLoad("PuzzleBlock.opm");
	OPcmanLoad("PuzzleSphere.opm");
	OPcmanLoad("Common/Texture.frag");
	OPcmanLoad("Common/Texture3D.vert");
	OPcmanLoad("TetrisBroken.png");
	OPcmanLoad("TetrisOrange.png");
	OPcmanLoad("TetrisBlue.png");
	OPcmanLoad("TetrisGreen.png");

	physicsCharacterExample.texture = (OPtexture*)OPcmanGet("TetrisBroken.png");
	physicsCharacterExample.texturePlayer = (OPtexture*)OPcmanGet("TetrisOrange.png");
	physicsCharacterExample.textureSphere = (OPtexture*)OPcmanGet("TetrisBlue.png");
	physicsCharacterExample.textureStatic = (OPtexture*)OPcmanGet("TetrisGreen.png");

	physicsCharacterExample.Mesh = (OPmesh*)OPcmanGet("PuzzleBlock.opm");
	physicsCharacterExample.MeshSphere = (OPmesh*)OPcmanGet("PuzzleSphere.opm");

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 }
	};

	physicsCharacterExample.Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("Common/Texture3D.vert");
	OPshader* frag = (OPshader*)OPcmanGet("Common/Texture.frag");
	*physicsCharacterExample.Effect = OPeffectCreate(
		*vert,
		*frag,
		attribs,
		3,
		"Model Effect",
		physicsCharacterExample.Mesh->VertexSize
		);

	physicsCharacterExample.SphereEffect = (OPeffect*)OPalloc(sizeof(OPeffect));
	*physicsCharacterExample.SphereEffect = OPeffectCreate(
		*vert,
		*frag,
		attribs,
		3,
		"Model Effect",
		physicsCharacterExample.MeshSphere->VertexSize
		);

	physicsCharacterExample.Camera = (OPcam*)OPalloc(sizeof(OPcam));
	*physicsCharacterExample.Camera = OPcamPersp(
		OPvec3Create(0, 20, 30),
		OPvec3Create(0, 1, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
		);

	physicsCharacterExample.boxCount = 1;
	physicsCharacterExample.boxes = (Dynamic*)OPallocZero(sizeof(Dynamic)* physicsCharacterExample.boxCount);
	for (ui32 i = 0; i < physicsCharacterExample.boxCount; i++) {
		f32 r = OPrandom();
		f32 r2 = 0.5;// OPrandom();
		f32 r3 = OPrandom();
		f32 size = (0.05f + (OPrandom() * 2));
		physicsCharacterExample.boxes[i].physics = OPphysicsCreateBoxDynamic(physicsCharacterExample.scene, -20 + (40 * r), 5 + 200 * r3, -20 + (40 * r2), size, size, size);
		physicsCharacterExample.boxes[i].size = size;
	}

	physicsCharacterExample.boxStaticCount = 1;
	physicsCharacterExample.boxesStatic = (Static*)OPallocZero(sizeof(Static)*physicsCharacterExample.boxStaticCount);
	for (ui32 i = 0; i < physicsCharacterExample.boxStaticCount; i++) {
		f32 r2 = 0.5;
		f32 size = 1;// 0.05f + (OPrandom() * 2);
		physicsCharacterExample.boxesStatic[i].physics = OPphysicsCreateBoxStatic(physicsCharacterExample.scene, ((-10.0) + (i * 2)), i * 2, -20 + (40 * r2), size, size, size);
		physicsCharacterExample.boxesStatic[i].size = size;
	}

	physicsCharacterExample.sphereCount = 1;
	physicsCharacterExample.spheres = (Dynamic*)OPallocZero(sizeof(Dynamic)* physicsCharacterExample.sphereCount);
	physicsCharacterExample.spheres[0].physics = OPphysicsCreateSphereDynamic(physicsCharacterExample.scene, 0, 0, 0, 0.5);
	physicsCharacterExample.spheres[0].size = 1;
	for (ui32 i = 1; i < physicsCharacterExample.sphereCount; i++) {
		f32 r = OPrandom();
		f32 r2 = OPrandom();
		f32 r3 = OPrandom();
		f32 size = 0.1f + (OPrandom() * 2);
		physicsCharacterExample.spheres[i].physics = OPphysicsCreateSphereDynamic(physicsCharacterExample.scene, -20 + (40 * r), 3 * r3, -20 + (40 * r2), size);
		physicsCharacterExample.spheres[i].size = size;
	}
	physicsCharacterExample.physicsPlane = OPphysicsCreatePlane(physicsCharacterExample.scene);

	physicsCharacterExample.controllerManager = PxCreateControllerManager(*(PxScene*)physicsCharacterExample.scene->scene);
	
	PxCapsuleControllerDesc desc = PxCapsuleControllerDesc();
	desc.height = 1.0f;
	desc.radius = 0.5f;
	desc.material = gPhysicsSDK->createMaterial(0.8f, 0.8f, 0.6f);
	physicsCharacterExample.controller = physicsCharacterExample.controllerManager->createController(desc);
#endif
}

OPint ExamplePhysicsCharacterUpdate(OPtimer* time) {
#ifdef OPIFEX_OPTION_PHYSICS

	PxVec3 disp = PxVec3(0,-0.1,0);

	if (OPkeyboardIsDown(OPKEY_RIGHT)) {
		disp.x = 1;
	}
	if (OPkeyboardIsDown(OPKEY_LEFT)) {
		disp.x = -1;
	}

	PxControllerFilters filters = PxControllerFilters(0);

	physicsCharacterExample.controller->move(disp, 0.01f, time->Elapsed, filters);
	PxExtendedVec3 pos = physicsCharacterExample.controller->getPosition();
	OPlog("Position %f, %f, %f", pos.x, pos.y, pos.z);

	for(OPuint i = 0; i < physicsCharacterExample.boxStaticCount; i++) {
		if(physicsCharacterExample.boxesStatic[i].dead) {
			physicsCharacterExample.boxStaticCount--;
			OPphysicsRemoveStatic(physicsCharacterExample.scene, physicsCharacterExample.boxesStatic[i].physics);
		}
	}

	if (OPkeyboardWasPressed(OPKEY_SPACE)) {
		OPphysicsAddForce(physicsCharacterExample.spheres[0].physics, 0, 500 * physicsCharacterExample.spheres[0].size, 0);
	}
	f32 rate = 500 * physicsCharacterExample.spheres[0].size;
	f32 rate2 = 50 * physicsCharacterExample.spheres[0].size;
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
		OPphysicsAddTorque(physicsCharacterExample.spheres[0].physics, 0, 0, rate);
		OPphysicsAddForce(physicsCharacterExample.spheres[0].physics, -rate2, 0, 0);
	}
	if (OPkeyboardIsDown(OPKEY_D)) {
		OPphysicsAddTorque(physicsCharacterExample.spheres[0].physics, 0, 0, -rate);
		OPphysicsAddForce(physicsCharacterExample.spheres[0].physics, rate2, 0, 0);
	}
	if (OPkeyboardIsDown(OPKEY_W)) {
		OPphysicsAddTorque(physicsCharacterExample.spheres[0].physics, -rate, 0, 0);
		OPphysicsAddForce(physicsCharacterExample.spheres[0].physics, 0, 0, -rate2);
	}
	if (OPkeyboardIsDown(OPKEY_S)) {
		OPphysicsAddTorque(physicsCharacterExample.spheres[0].physics, rate, 0, 0);
		OPphysicsAddForce(physicsCharacterExample.spheres[0].physics, 0, 0, rate2);
	}

	if (OPkeyboardIsDown(OPKEY_UP)) { physicsCharacterExample.Camera->_pos.y += 0.2; }
	if (OPkeyboardIsDown(OPKEY_DOWN)) { physicsCharacterExample.Camera->_pos.y -= 0.2; }
	if (OPkeyboardIsDown(OPKEY_LEFT)) { physicsCharacterExample.Camera->_pos.x -= 0.2; }
	if (OPkeyboardIsDown(OPKEY_RIGHT)) { physicsCharacterExample.Camera->_pos.x += 0.2; }

	physicsCharacterExample.Camera->_viewStale = 1;
	OPcamUpdateView((*physicsCharacterExample.Camera));


	OPphysicsStep(physicsCharacterExample.scene, time->Elapsed);

	OPrenderDepth(1);
	OPrenderClear(0.1, 0.1, 0.1);

	OPmat4 world = OPMAT4IDENTITY;

	OPmeshBind(physicsCharacterExample.Mesh);
	OPeffectBind(physicsCharacterExample.Effect);

	OPtextureClearActive();
	ui32 tex = OPtextureBind(physicsCharacterExample.texture);
	ui32 tex2 = OPtextureBind(physicsCharacterExample.texturePlayer);
	ui32 tex3 = OPtextureBind(physicsCharacterExample.textureSphere);
	ui32 tex4 = OPtextureBind(physicsCharacterExample.textureStatic);

	OPmat4 view, proj;	

	OPcamGetView((*physicsCharacterExample.Camera), &view);
	OPcamGetProj((*physicsCharacterExample.Camera), &proj);

	OPeffectParamMat4("uProj", &proj);
	OPeffectParamMat4("uView", &view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPeffectParamVec3("uLightDirection", &light);
	OPeffectParami("uColorTexture", tex);
	OPmat4 scale = OPMAT4IDENTITY;
	OPmat4 scratch = OPMAT4IDENTITY;
	for (ui32 i = 0; i < physicsCharacterExample.boxCount; i++) {
		scale = OPmat4Scl(physicsCharacterExample.boxes[i].size * 2, physicsCharacterExample.boxes[i].size * 2, physicsCharacterExample.boxes[i].size * 2);
		OPphysicsGetTransform((OPphysicsActor*)physicsCharacterExample.boxes[i].physics, &scratch);
		world = scratch * scale;
		OPeffectParamMat4("uWorld", &world);
		OPmeshRender();
	}
	OPeffectParami("uColorTexture", tex4);
	for (ui32 i = 0; i < physicsCharacterExample.boxStaticCount; i++) {
		scale = OPmat4Scl(physicsCharacterExample.boxesStatic[i].size * 2, physicsCharacterExample.boxesStatic[i].size * 2, physicsCharacterExample.boxesStatic[i].size * 2);
		OPphysicsGetTransform((OPphysicsActor*)physicsCharacterExample.boxesStatic[i].physics, &scratch);
		world = scratch * scale;
		OPeffectParamMat4("uWorld", &world);
		OPmeshRender();
	}

	{
		world = OPmat4Translate(pos.x, pos.y, pos.z) * OPmat4Scl(2);
		OPeffectParamMat4("uWorld", &world);
		OPmeshRender();
	}

	OPmeshBind(physicsCharacterExample.MeshSphere);
	OPeffectBind(physicsCharacterExample.SphereEffect);
	OPeffectParamMat4("uProj", &proj);
	OPeffectParamMat4("uView", &view);
	OPeffectParamVec3("uLightDirection", &light);

	OPeffectParami("uColorTexture", tex2);
	scale = OPmat4Scl(physicsCharacterExample.spheres[0].size * 2, physicsCharacterExample.spheres[0].size * 2, physicsCharacterExample.spheres[0].size * 2);
	OPphysicsGetTransform((OPphysicsActor*)physicsCharacterExample.spheres[0].physics, &scratch);
	world = scratch * scale;
	OPeffectParamMat4("uWorld", &world);
	OPmeshRender();

	OPeffectParami("uColorTexture", tex3);
	for (ui32 i = 1; i < physicsCharacterExample.sphereCount; i++) {
		scale = OPmat4Scl(physicsCharacterExample.spheres[i].size * 2, physicsCharacterExample.spheres[i].size * 2, physicsCharacterExample.spheres[i].size * 2);
		OPphysicsGetTransform((OPphysicsActor*)physicsCharacterExample.spheres[i].physics, &scratch);
		world = scratch * scale;
		OPeffectParamMat4v("uWorld", 1, &world);
		OPmeshRender();
	}

	OPrenderPresent();
#endif
	return false;
}

OPint ExamplePhysicsCharacterExit(OPgameState* next) {
#ifdef OPIFEX_OPTION_PHYSICS
	OPphysicsDestroy(physicsCharacterExample.scene);
#endif
	return 0;
}

#ifndef OPIFEX_OPTION_PHYSICS
OPint GS_EXAMPLE_PHYSICSCHARACTER_AVAILABLE = 0;
#else
OPint GS_EXAMPLE_PHYSICSCHARACTER_AVAILABLE = 1;
#endif

OPgameState GS_EXAMPLE_PHYSICSCHARACTER = {
	ExamplePhysicsCharacterEnter,
	ExamplePhysicsCharacterUpdate,
	ExamplePhysicsCharacterExit
};