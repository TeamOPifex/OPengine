#include "./ExampleSelectorState.h"

#ifdef OPIFEX_OPTION_PHYSX

#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Performance/include/OPphysX.h"
#include "./Performance/include/OPphysXScene.h"
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
} PhysicsExample;

PhysicsExample* physicsExample;

ui32 _contact = 0;
ui32 _trigger = 0;

void OnTrigger(OPphysXTrigger trigger) {
	OPlog("User Defined Trigger %d", _trigger++);
}
void OnContact(OPphysXContact contact) {
	OPlog("User Defined Contact %d", _contact++);
	for(OPuint p = 0; p < contact.count; p++) {
		const PxContactPair pair = contact.contactPairs[p];
		for(OPuint i = 0; i < physicsExample->boxCount; i++) {
			if(pair.shapes[1] == physicsExample->boxes[i].shape) {
				physicsExample->boxes[i].dead = 1;
			}
		}
	}
}

void ExamplePhysicsEnter(OPgameState* last) {

	physicsExample = (PhysicsExample*)OPalloc(sizeof(PhysicsExample));

	OPphysXInit();
	physicsExample->scene = OPphysXSceneCreate(OPvec3Create(0.0f, -9.8f, 0.0f), OnTrigger, OnContact);

	OPCMAN.Load("PuzzleBlock.opm");
	OPCMAN.Load("PuzzleSphere.opm");
	OPCMAN.Load("Common/Texture.frag");
	OPCMAN.Load("Common/Texture3D.vert");
	OPCMAN.Load("TetrisBroken.png");
	OPCMAN.Load("TetrisOrange.png");
	OPCMAN.Load("TetrisBlue.png");
	OPCMAN.Load("TetrisGreen.png");

	physicsExample->texture = (OPtexture*)OPCMAN.Get("TetrisBroken.png");
	physicsExample->texturePlayer = (OPtexture*)OPCMAN.Get("TetrisOrange.png");
	physicsExample->textureSphere = (OPtexture*)OPCMAN.Get("TetrisBlue.png");
	physicsExample->textureStatic = (OPtexture*)OPCMAN.Get("TetrisGreen.png");

	physicsExample->Mesh = (OPmesh*)OPCMAN.Get("PuzzleBlock.opm");
	physicsExample->MeshSphere = (OPmesh*)OPCMAN.Get("PuzzleSphere.opm");

	OPshaderAttribute attribs[] = {
		{ "aPosition", OPshaderElementType::FLOAT, 3 },
		{ "aNormal", OPshaderElementType::FLOAT, 3 },
		{ "aUV", OPshaderElementType::FLOAT, 2 }
	};

	physicsExample->Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPCMAN.Get("Common/Texture3D.vert");
	OPshader* frag = (OPshader*)OPCMAN.Get("Common/Texture.frag");
	physicsExample->Effect->Init(vert, frag);

	physicsExample->SphereEffect = (OPeffect*)OPalloc(sizeof(OPeffect));
	physicsExample->SphereEffect->Init(vert, frag);

	physicsExample->Camera = (OPcam*)OPalloc(sizeof(OPcam));
	physicsExample->Camera->SetPerspective(
		OPvec3Create(0, 20, 50),
		OPvec3Create(0, 1, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		1000.0f,
		45.0f,
		OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (f32)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height
		);

	OPphysXMaterial* material = OPphysXCreateMaterial(0.8, 0.8, 0.6);

	physicsExample->boxCount = 200;
	physicsExample->boxes = (Dynamic*)OPallocZero(sizeof(Dynamic)* physicsExample->boxCount);
	for (ui32 i = 0; i < physicsExample->boxCount; i++) {
		f32 r = OPrandom();
		f32 r2 = 0.5;// OPrandom();
		f32 r3 = OPrandom();
		f32 size = (0.05f + (OPrandom() * 2));
		physicsExample->boxes[i].physics = OPphysXSceneCreateDynamic(physicsExample->scene, OPvec3Create(-20 + (40 * r), 5 + 200 * r3, -20 + (40 * r2)));
		physicsExample->boxes[i].shape = OPphysXAddBoxShape(physicsExample->boxes[i].physics, material, OPvec3Create(size, size, size));
		//OPphysXSetAsTrigger(OPphysXAddBoxShape(physicsExample->boxes[i].physics, material, OPvec3Create(size, size, size)));
		OPphysXSetFilter(physicsExample->boxes[i].physics, 1, 2);
		OPphysXSceneAddActor(physicsExample->scene, physicsExample->boxes[i].physics);
		physicsExample->boxes[i].size = size;
	}

	physicsExample->boxStaticCount = 10;
	physicsExample->boxesStatic = (Static*)OPallocZero(sizeof(Static)*physicsExample->boxStaticCount);
	for (ui32 i = 0; i < physicsExample->boxStaticCount; i++) {
		f32 r2 = 0.5;
		f32 size = 1;// 0.05f + (OPrandom() * 2);
		physicsExample->boxesStatic[i].physics = OPphysXSceneCreateStatic(physicsExample->scene, OPvec3Create(((-10.0) + (i * 2)), i * 2, -20 + (40 * r2)));
		physicsExample->boxesStatic[i].shape = OPphysXAddBoxShape(physicsExample->boxesStatic[i].physics, material, OPvec3Create(size, size, size));
		OPphysXSceneAddActor(physicsExample->scene, physicsExample->boxesStatic[i].physics);
		physicsExample->boxesStatic[i].size = size;
	}

	physicsExample->sphereCount = 5;
	physicsExample->spheres = (Dynamic*)OPallocZero(sizeof(Dynamic)* physicsExample->sphereCount);
	physicsExample->spheres[0].physics = OPphysXSceneCreateDynamic(physicsExample->scene, OPVEC3_ZERO);
	OPphysXAddSphereShape(physicsExample->spheres[0].physics, material, 1);
	OPphysXSetMass(physicsExample->spheres[0].physics, 1);
	OPphysXSceneAddActor(physicsExample->scene, physicsExample->spheres[0].physics);
	OPphysXSetFilter(physicsExample->spheres[0].physics, 2, 1);
	physicsExample->spheres[0].size = 1;
	for (ui32 i = 1; i < physicsExample->sphereCount; i++) {
		f32 r = OPrandom();
		f32 r2 = OPrandom();
		f32 r3 = OPrandom();
		f32 size = 0.1f + (OPrandom() * 2);
		physicsExample->spheres[i].physics = OPphysXSceneCreateDynamic(physicsExample->scene, OPvec3Create(-20 + (40 * r), 3 * r3, -20 + (40 * r2)));
		physicsExample->spheres[i].shape = OPphysXAddSphereShape(physicsExample->spheres[i].physics, material, size);
		OPphysXSetMass(physicsExample->spheres[i].physics, size);
		OPphysXSceneAddActor(physicsExample->scene, physicsExample->spheres[i].physics);
		physicsExample->spheres[i].size = size;
	}

	PxTransform transform = PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	physicsExample->physicsPlane = OPphysXSceneCreateStatic(physicsExample->scene, transform);
	OPphysXAddPlaneShape(physicsExample->physicsPlane, material);
	OPphysXSceneAddActor(physicsExample->scene, physicsExample->physicsPlane);

}

OPint ExamplePhysicsUpdate(OPtimer* time) {

	for(OPuint i = 0; i < physicsExample->boxCount; i++) {
		if(physicsExample->boxes[i].dead == 1) {
			OPphysXSceneRemove(physicsExample->scene, physicsExample->boxes[i].physics);
			physicsExample->boxes[i].dead++;
		}
	}

	if (OPkeyboardWasPressed(OPkeyboardKey::SPACE)) {
		OPphysXAddForce(physicsExample->spheres[0].physics, OPvec3Create(0, 500 * physicsExample->spheres[0].size, 0));
	}
	f32 rate = 500 * physicsExample->spheres[0].size;
	f32 rate2 = 50 * physicsExample->spheres[0].size;
	//if (OPkeyboardWasPressed(OPkeyboardKey::A)) {
	//	OPphysicsSetLinearVelocity(spheres[0].physics, -rate / 100, 0, 0);
	//}
	//if (OPkeyboardWasPressed(OPkeyboardKey::D)) {
	//	OPphysicsSetLinearVelocity(spheres[0].physics, rate / 100, 0, 0);
	//}
	//if (OPkeyboardWasPressed(OPkeyboardKey::W)) {
	//	OPphysicsSetLinearVelocity(spheres[0].physics, 0, 0, -rate / 100);
	//}
	//if (OPkeyboardWasPressed(OPkeyboardKey::S)) {
	//	OPphysicsSetLinearVelocity(spheres[0].physics, 0, 0, rate / 100);
	//}

	if (OPkeyboardIsDown(OPkeyboardKey::A)) {
		OPphysXAddTorque(physicsExample->spheres[0].physics, OPvec3Create(0.0f, 0.0f, rate));
		OPphysXAddForce(physicsExample->spheres[0].physics, OPvec3Create(-rate2, 0.0f, 0.0f));
	}
	if (OPkeyboardIsDown(OPkeyboardKey::D)) {
		OPphysXAddTorque(physicsExample->spheres[0].physics, OPvec3Create(0.0f, 0.0f, -rate));
		OPphysXAddForce(physicsExample->spheres[0].physics, OPvec3Create(rate2, 0.0f, 0.0f));
	}
	if (OPkeyboardIsDown(OPkeyboardKey::W)) {
		OPphysXAddTorque(physicsExample->spheres[0].physics, OPvec3Create(-rate, 0, 0));
		OPphysXAddForce(physicsExample->spheres[0].physics, OPvec3Create(0.0f, 0.0f, -rate2));
	}
	if (OPkeyboardIsDown(OPkeyboardKey::S)) {
		OPphysXAddTorque(physicsExample->spheres[0].physics, OPvec3Create(rate, 0.0f, 0.0f));
		OPphysXAddForce(physicsExample->spheres[0].physics, OPvec3Create(0.0f, 0.0f, rate2));
	}

	if (OPkeyboardIsDown(OPkeyboardKey::UP)) { physicsExample->Camera->pos.y += 0.2f; }
	if (OPkeyboardIsDown(OPkeyboardKey::DOWN)) { physicsExample->Camera->pos.y -= 0.2f; }
	if (OPkeyboardIsDown(OPkeyboardKey::LEFT)) { physicsExample->Camera->pos.x -= 0.2f; }
	if (OPkeyboardIsDown(OPkeyboardKey::RIGHT)) { physicsExample->Camera->pos.x += 0.2f; }

	physicsExample->Camera->Update();


	OPphysXSceneUpdate(physicsExample->scene, time);

	OPrenderDepth(true);
	OPrenderClear(0.1f, 0.1f, 0.1f);

	OPmat4 world = OPMAT4_IDENTITY;

	physicsExample->Mesh->Bind();
	physicsExample->Effect->Bind();


	OPeffectSet("uProj", &physicsExample->Camera->proj);
	OPeffectSet("uView", &physicsExample->Camera->view);

	OPvec3 light = OPvec3Create(0, 1, 0);
	OPeffectSet("uLightDirection", &light);
	OPeffectSet("uColorTexture", physicsExample->texture, 0);
	OPmat4 scale = OPMAT4_IDENTITY;
	OPmat4 scratch = OPMAT4_IDENTITY;
	for (ui32 i = 0; i < physicsExample->boxCount; i++) {
		if(physicsExample->boxes[i].dead) continue;
		scale = OPmat4Scl(physicsExample->boxes[i].size * 2, physicsExample->boxes[i].size * 2, physicsExample->boxes[i].size * 2);
		OPphysXGetTransform((OPphysXRigidActor*)physicsExample->boxes[i].physics, &scratch);
		world = scratch * scale;
		OPeffectSet("uWorld", &world);
		OPmeshRender();
	}
	OPeffectSet("uColorTexture", physicsExample->textureStatic, 0);
	for (ui32 i = 0; i < physicsExample->boxStaticCount; i++) {
		if(physicsExample->boxesStatic[i].dead) continue;
		scale = OPmat4Scl(physicsExample->boxesStatic[i].size * 2, physicsExample->boxesStatic[i].size * 2, physicsExample->boxesStatic[i].size * 2);
		OPphysXGetTransform((OPphysXRigidActor*)physicsExample->boxesStatic[i].physics, &scratch);
		world = scratch * scale;
		OPeffectSet("uWorld", &world);
		OPmeshRender();
	}

	physicsExample->MeshSphere->Bind();
	physicsExample->SphereEffect->Bind();
	OPeffectSet("uProj", &physicsExample->Camera->proj);
	OPeffectSet("uView", &physicsExample->Camera->view);
	OPeffectSet("uLightDirection", &light);

	OPeffectSet("uColorTexture", physicsExample->texturePlayer, 0);
	scale = OPmat4Scl(physicsExample->spheres[0].size * 2, physicsExample->spheres[0].size * 2, physicsExample->spheres[0].size * 2);
	OPphysXGetTransform((OPphysXRigidActor*)physicsExample->spheres[0].physics, &scratch);
	world = scratch * scale;
	OPeffectSet("uWorld", &world);
	OPmeshRender();

	OPeffectSet("uColorTexture", physicsExample->textureSphere, 0);
	for (ui32 i = 1; i < physicsExample->sphereCount; i++) {
		scale = OPmat4Scl(physicsExample->spheres[i].size * 2, physicsExample->spheres[i].size * 2, physicsExample->spheres[i].size * 2);
		OPphysXGetTransform((OPphysXRigidActor*)physicsExample->spheres[i].physics, &scratch);
		world = scratch * scale;
		OPeffectSet("uWorld", 1, &world);
		OPmeshRender();
	}

	OPrenderPresent();
	return false;
}

void ExamplePhysicsRender(OPfloat delta) {
    
}

OPint ExamplePhysicsExit(OPgameState* next) {
	OPphysXSceneDestroy(physicsExample->scene);
	OPphysXShutdown();
	OPfree(physicsExample);
	return 0;
}

OPint GS_EXAMPLE_PHYSICS_AVAILABLE = 1;
OPgameState GS_EXAMPLE_PHYSICS = {
	ExamplePhysicsEnter,
	ExamplePhysicsUpdate,
	ExamplePhysicsRender,
	ExamplePhysicsExit
};
#else
OPint GS_EXAMPLE_PHYSICS_AVAILABLE = 0;
OPgameState GS_EXAMPLE_PHYSICS = {
	NULL,
	NULL,
	NULL,
	NULL
};
#endif
