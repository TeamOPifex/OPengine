#include "./ExampleSelectorState.h"

#ifdef OPIFEX_OPTION_PHYSX
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Performance/include/OPphysX.h"
#include "./Performance/include/OPphysXScene.h"
#include "./Performance/include/OPphysXController.h"
#include "./Human/include/Systems/OPinputSystem.h"

#include "./Data/include/OPcman.h"


typedef struct {
	PxRigidDynamic* physics;
	PxShape* shape;
	f32 size;
	OPuint dead;
} Dynamic;

typedef struct {
	PxRigidStatic* physics;
	PxShape* shape;
	f32 size;
	OPuint dead;
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
	PxRigidStatic* physicsPlane;
	OPtexture* texture;
	OPtexture* textureStatic;
	OPtexture* texturePlayer;
	OPtexture* textureSphere;
	OPphysXScene* scene;
	OPphysXControllerManager* manager;
	OPphysXController* controller;
} PhysicsCharacterExample;

PhysicsCharacterExample* physicsCharacterExample;

ui32 _contactCharacter = 0;
ui32 _triggerCharacter = 0;

void CharacterOnTrigger(OPphysXTrigger trigger) {
	OPlog("User Defined Trigger %d", _triggerCharacter++);
}
void CharacterOnContact(OPphysXContact contact) {
	OPlog("User Defined Contact %d", _contactCharacter++);
	for(OPuint p = 0; p < contact.count; p++) {
		const PxContactPair pair = contact.contactPairs[p];
		for(OPuint i = 0; i < physicsCharacterExample->boxCount; i++) {
			if(pair.shapes[1] == physicsCharacterExample->boxes[i].shape) {
				physicsCharacterExample->boxes[i].dead = 1;
			}
		}
	}
}

void CharacterOnShapeHit(PxControllerShapeHit hit) {
	OPlog("Hit");
	for(OPuint i = 0; i < physicsCharacterExample->boxCount; i++) {
		if(hit.shape == physicsCharacterExample->boxes[i].shape) {
			physicsCharacterExample->boxes[i].dead = 1;
		}
	}
}

void ExamplePhysicsCharacterEnter(OPgameState* last) {

	physicsCharacterExample = (PhysicsCharacterExample*)OPalloc(sizeof(PhysicsCharacterExample));

	OPphysXInit();
	physicsCharacterExample->scene = OPphysXSceneCreate(OPvec3Create(0, -9.8, 0), CharacterOnTrigger, CharacterOnContact);

	OPcmanLoad("PuzzleBlock.opm");
	OPcmanLoad("PuzzleSphere.opm");
	OPcmanLoad("Common/Texture.frag");
	OPcmanLoad("Common/Texture3D.vert");
	OPcmanLoad("TetrisBroken.png");
	OPcmanLoad("TetrisOrange.png");
	OPcmanLoad("TetrisBlue.png");
	OPcmanLoad("TetrisGreen.png");

	physicsCharacterExample->texture = (OPtexture*)OPcmanGet("TetrisBroken.png");
	physicsCharacterExample->texturePlayer = (OPtexture*)OPcmanGet("TetrisOrange.png");
	physicsCharacterExample->textureSphere = (OPtexture*)OPcmanGet("TetrisBlue.png");
	physicsCharacterExample->textureStatic = (OPtexture*)OPcmanGet("TetrisGreen.png");

	physicsCharacterExample->Mesh = (OPmesh*)OPcmanGet("PuzzleBlock.opm");
	physicsCharacterExample->MeshSphere = (OPmesh*)OPcmanGet("PuzzleSphere.opm");

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 }
	};

	physicsCharacterExample->Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("Common/Texture3D.vert");
	OPshader* frag = (OPshader*)OPcmanGet("Common/Texture.frag");
	*physicsCharacterExample->Effect = OPeffectCreate(
		*vert,
		*frag,
		attribs,
		3,
		"Model Effect",
		physicsCharacterExample->Mesh->VertexSize
		);

	physicsCharacterExample->SphereEffect = (OPeffect*)OPalloc(sizeof(OPeffect));
	*physicsCharacterExample->SphereEffect = OPeffectCreate(
		*vert,
		*frag,
		attribs,
		3,
		"Model Effect",
		physicsCharacterExample->MeshSphere->VertexSize
		);

	physicsCharacterExample->Camera = (OPcam*)OPalloc(sizeof(OPcam));
	*physicsCharacterExample->Camera = OPcamPersp(
		OPvec3Create(0, 20, 50),
		OPvec3Create(0, 1, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
		);

	OPphysXMaterial* material = OPphysXCreateMaterial(0.8, 0.8, 0.6);

	physicsCharacterExample->boxCount = 200;
	physicsCharacterExample->boxes = (Dynamic*)OPallocZero(sizeof(Dynamic)* physicsCharacterExample->boxCount);
	for (ui32 i = 0; i < physicsCharacterExample->boxCount; i++) {
		f32 r = OPrandom();
		f32 r2 = 0.5;// OPrandom();
		f32 r3 = OPrandom();
		f32 size = (0.05f + (OPrandom() * 2));
		physicsCharacterExample->boxes[i].physics = OPphysXSceneCreateDynamic(physicsCharacterExample->scene, OPvec3Create(-20 + (40 * r), 5 + 200 * r3, -20 + (40 * r2)));
		physicsCharacterExample->boxes[i].shape = OPphysXAddBoxShape(physicsCharacterExample->boxes[i].physics, material, OPvec3Create(size, size, size));
		//OPphysXSetAsTrigger(OPphysXAddBoxShape(physicsCharacterExample->boxes[i].physics, material, OPvec3Create(size, size, size)));
		OPphysXSetFilter(physicsCharacterExample->boxes[i].physics, 1, 2);
		OPphysXSceneAddActor(physicsCharacterExample->scene, physicsCharacterExample->boxes[i].physics);
		physicsCharacterExample->boxes[i].size = size;
	}

	physicsCharacterExample->boxStaticCount = 100;
	physicsCharacterExample->boxesStatic = (Static*)OPallocZero(sizeof(Static)*physicsCharacterExample->boxStaticCount);
	for (ui32 i = 0; i < physicsCharacterExample->boxStaticCount; i++) {
		f32 r2 = 0.5;
		f32 size = 1;// 0.05f + (OPrandom() * 2);
		physicsCharacterExample->boxesStatic[i].physics = OPphysXSceneCreateStatic(physicsCharacterExample->scene, OPvec3Create(-200 + OPrandom() * 400, 0, -200 + OPrandom() * 400));
		physicsCharacterExample->boxesStatic[i].shape = OPphysXAddBoxShape(physicsCharacterExample->boxesStatic[i].physics, material, OPvec3Create(size, size, size));
		OPphysXSceneAddActor(physicsCharacterExample->scene, physicsCharacterExample->boxesStatic[i].physics);
		physicsCharacterExample->boxesStatic[i].size = size;
	}

	physicsCharacterExample->sphereCount = 5;
	physicsCharacterExample->spheres = (Dynamic*)OPallocZero(sizeof(Dynamic)* physicsCharacterExample->sphereCount);
	physicsCharacterExample->spheres[0].physics = OPphysXSceneCreateDynamic(physicsCharacterExample->scene, OPVEC3_ZERO);
	OPphysXAddSphereShape(physicsCharacterExample->spheres[0].physics, material, 1);
	OPphysXSetMass(physicsCharacterExample->spheres[0].physics, 1);
	OPphysXSceneAddActor(physicsCharacterExample->scene, physicsCharacterExample->spheres[0].physics);
	OPphysXSetFilter(physicsCharacterExample->spheres[0].physics, 2, 1);
	physicsCharacterExample->spheres[0].size = 1;
	for (ui32 i = 1; i < physicsCharacterExample->sphereCount; i++) {
		f32 r = OPrandom();
		f32 r2 = OPrandom();
		f32 r3 = OPrandom();
		f32 size = 0.1f + (OPrandom() * 2);
		physicsCharacterExample->spheres[i].physics = OPphysXSceneCreateDynamic(physicsCharacterExample->scene, OPvec3Create(-20 + (40 * r), 3 * r3, -20 + (40 * r2)));
		physicsCharacterExample->spheres[i].shape = OPphysXAddSphereShape(physicsCharacterExample->spheres[i].physics, material, size);
		OPphysXSetMass(physicsCharacterExample->spheres[i].physics, size);
		OPphysXSceneAddActor(physicsCharacterExample->scene, physicsCharacterExample->spheres[i].physics);
		physicsCharacterExample->spheres[i].size = size;
	}

	OPfloat pSize = 50;
	OPfloat pHeight = 10;
	
	OPvec3 verts[8] = {
		{-pSize, -pHeight, -pSize },
		{0, pHeight, -pSize },
		{0, pHeight, pSize },
		{-pSize, -pHeight, pSize },

		{0, pHeight, -pSize },
		{pSize, -pHeight, -pSize },
		{pSize, -pHeight, pSize },
		{0, pHeight, pSize }
	};

	ui32 indices[12] = {
		2, 1, 0,
		3, 2, 0,
		6, 5, 4,
		7, 6, 4
	};

	PxTriangleMesh* mesh = OPphysXCreateTriangleMesh(8, (PxVec3*)verts, 4, (PxU32*)indices);
	physicsCharacterExample->physicsPlane = OPphysXSceneCreateStatic(physicsCharacterExample->scene, OPVEC3_ZERO);
	OPphysXAddTriangleMeshShape(physicsCharacterExample->physicsPlane, material, mesh);
	OPphysXSceneAddActor(physicsCharacterExample->scene, physicsCharacterExample->physicsPlane);

	physicsCharacterExample->manager = OPphysXControllerCreateManager(physicsCharacterExample->scene);
	physicsCharacterExample->controller = OPphysXControllerCreate(physicsCharacterExample->manager, material, 0.5, 0.5, CharacterOnShapeHit, NULL, NULL);
}

OPint ExamplePhysicsCharacterUpdate(OPtimer* time) {

	for(OPuint i = 0; i < physicsCharacterExample->boxCount; i++) {
		if(physicsCharacterExample->boxes[i].dead == 1) {
			OPphysXSceneRemove(physicsCharacterExample->scene, physicsCharacterExample->boxes[i].physics);
			physicsCharacterExample->boxes[i].dead++;
		}
	}

	OPvec3 disp = OPvec3Create(0, -0.98 * 0.25, 0);

	OPvec2 leftThumb = OPgamePadLeftThumb(OPgamePadGet(OPGAMEPAD_ONE));
	disp.x += leftThumb.x * 0.5f;
	disp.z -= leftThumb.y * 0.5f;

	if (OPkeyboardIsDown(OPKEY_D)) {
		disp.x = 0.5f;
	}
	if (OPkeyboardIsDown(OPKEY_A)) {
		disp.x = -0.5f;
	}
	if (OPkeyboardIsDown(OPKEY_W)) {
		disp.z = -0.5f;
	}
	if (OPkeyboardIsDown(OPKEY_S)) {
		disp.z = 0.5f;
	}
	if (OPkeyboardWasPressed(OPKEY_SPACE)) {
		disp.y = 5.0;
	}

	OPphysXControllerMove(physicsCharacterExample->controller, disp, time);
	OPvec3 pos = OPphysXControllerGetPos(physicsCharacterExample->controller);

	f32 rate = 500 * physicsCharacterExample->spheres[0].size;
	f32 rate2 = 50 * physicsCharacterExample->spheres[0].size;

	if (OPkeyboardIsDown(OPKEY_UP)) { physicsCharacterExample->Camera->_pos.y += 0.2; }
	if (OPkeyboardIsDown(OPKEY_DOWN)) { physicsCharacterExample->Camera->_pos.y -= 0.2; }
	if (OPkeyboardIsDown(OPKEY_LEFT)) { physicsCharacterExample->Camera->_pos.x -= 0.2; }
	if (OPkeyboardIsDown(OPKEY_RIGHT)) { physicsCharacterExample->Camera->_pos.x += 0.2; }

	physicsCharacterExample->Camera->_viewStale = 1;
	OPcamUpdateView((*physicsCharacterExample->Camera));


	OPphysXSceneUpdate(physicsCharacterExample->scene, time);

	OPrenderDepth(1);
	OPrenderClear(0.1, 0.1, 0.1);

	OPmat4 world = OPMAT4_IDENTITY;

	OPmeshBind(physicsCharacterExample->Mesh);
	OPeffectBind(physicsCharacterExample->Effect);

	OPtextureClearActive();
	ui32 tex = OPtextureBind(physicsCharacterExample->texture);
	ui32 tex2 = OPtextureBind(physicsCharacterExample->texturePlayer);
	ui32 tex3 = OPtextureBind(physicsCharacterExample->textureSphere);
	ui32 tex4 = OPtextureBind(physicsCharacterExample->textureStatic);

	OPmat4 view, proj;	

	OPcamGetView((*physicsCharacterExample->Camera), &view);
	OPcamGetProj((*physicsCharacterExample->Camera), &proj);

	OPeffectParamMat4("uProj", &proj);
	OPeffectParamMat4("uView", &view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPeffectParamVec3("uLightDirection", &light);
	OPeffectParami("uColorTexture", tex);
	OPmat4 scale = OPMAT4_IDENTITY;
	OPmat4 scratch = OPMAT4_IDENTITY;
	for (ui32 i = 0; i < physicsCharacterExample->boxCount; i++) {
		if(physicsCharacterExample->boxes[i].dead) continue;
		scale = OPmat4Scl(physicsCharacterExample->boxes[i].size * 2, physicsCharacterExample->boxes[i].size * 2, physicsCharacterExample->boxes[i].size * 2);
		OPphysXGetTransform((OPphysXRigidActor*)physicsCharacterExample->boxes[i].physics, &scratch);
		world = scratch * scale;
		OPeffectParamMat4("uWorld", &world);
		OPmeshRender();
	}
	OPeffectParami("uColorTexture", tex4);
	for (ui32 i = 0; i < physicsCharacterExample->boxStaticCount; i++) {
		if(physicsCharacterExample->boxesStatic[i].dead) continue;
		scale = OPmat4Scl(physicsCharacterExample->boxesStatic[i].size * 2, physicsCharacterExample->boxesStatic[i].size * 2, physicsCharacterExample->boxesStatic[i].size * 2);
		OPphysXGetTransform((OPphysXRigidActor*)physicsCharacterExample->boxesStatic[i].physics, &scratch);
		world = scratch * scale;
		OPeffectParamMat4("uWorld", &world);
		OPmeshRender();
	}

	OPmeshBind(physicsCharacterExample->MeshSphere);
	OPeffectBind(physicsCharacterExample->SphereEffect);
	OPeffectParamMat4("uProj", &proj);
	OPeffectParamMat4("uView", &view);
	OPeffectParamVec3("uLightDirection", &light);

	OPeffectParami("uColorTexture", tex2);
	scale = OPmat4Scl(physicsCharacterExample->spheres[0].size * 2, physicsCharacterExample->spheres[0].size * 2, physicsCharacterExample->spheres[0].size * 2);
	OPphysXGetTransform((OPphysXRigidActor*)physicsCharacterExample->spheres[0].physics, &scratch);
	world = scratch * scale;
	OPeffectParamMat4("uWorld", &world);
	OPmeshRender();

	{
		world = OPmat4Translate(pos.x, pos.y, pos.z) * OPmat4Scl(1);
		OPeffectParamMat4("uWorld", &world);
		OPmeshRender();
	}

	OPeffectParami("uColorTexture", tex3);
	for (ui32 i = 1; i < physicsCharacterExample->sphereCount; i++) {
		scale = OPmat4Scl(physicsCharacterExample->spheres[i].size * 2, physicsCharacterExample->spheres[i].size * 2, physicsCharacterExample->spheres[i].size * 2);
		OPphysXGetTransform((OPphysXRigidActor*)physicsCharacterExample->spheres[i].physics, &scratch);
		world = scratch * scale;
		OPeffectParamMat4v("uWorld", 1, &world);
		OPmeshRender();
	}


	OPrenderPresent();
	return false;
}

OPint ExamplePhysicsCharacterExit(OPgameState* next) {
	OPphysXSceneDestroy(physicsCharacterExample->scene);
	OPphysXShutdown();
	OPfree(physicsCharacterExample);
	return 0;
}

OPint GS_EXAMPLE_PHYSICSCHARACTER_AVAILABLE = 1;

OPgameState GS_EXAMPLE_PHYSICSCHARACTER = {
	ExamplePhysicsCharacterEnter,
	ExamplePhysicsCharacterUpdate,
	ExamplePhysicsCharacterExit
};
#else
OPint GS_EXAMPLE_PHYSICSCHARACTER_AVAILABLE = 0;

OPgameState GS_EXAMPLE_PHYSICSCHARACTER = {
	NULL,
	NULL,
	NULL
};
#endif