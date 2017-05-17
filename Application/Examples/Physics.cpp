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

ui32 _contact = 0;
ui32 _trigger = 0;

void OnTrigger(OPphysXTrigger trigger) {
	OPlog("User Defined Trigger %d", _trigger++);
}
void OnContact(OPphysXContact contact);

class PhysicsExample : public OPgameState {
public:
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


	void Init(OPgameState* last) {


		OPphysXInit();
		scene = OPphysXSceneCreate(OPvec3Create(0.0f, -9.8f, 0.0f), OnTrigger, OnContact);

		OPCMAN.Load("PuzzleBlock.opm");
		OPCMAN.Load("PuzzleSphere.opm");
		OPCMAN.Load("Common/Texture.frag");
		OPCMAN.Load("Common/Texture3D.vert");
		OPCMAN.Load("TetrisBroken.png");
		OPCMAN.Load("TetrisOrange.png");
		OPCMAN.Load("TetrisBlue.png");
		OPCMAN.Load("TetrisGreen.png");

		texture = (OPtexture*)OPCMAN.Get("TetrisBroken.png");
		texturePlayer = (OPtexture*)OPCMAN.Get("TetrisOrange.png");
		textureSphere = (OPtexture*)OPCMAN.Get("TetrisBlue.png");
		textureStatic = (OPtexture*)OPCMAN.Get("TetrisGreen.png");

		Mesh = (OPmesh*)OPCMAN.Get("PuzzleBlock.opm");
		MeshSphere = (OPmesh*)OPCMAN.Get("PuzzleSphere.opm");

		OPshaderAttribute attribs[] = {
			{ "aPosition", OPshaderElementType::FLOAT, 3 },
			{ "aNormal", OPshaderElementType::FLOAT, 3 },
			{ "aUV", OPshaderElementType::FLOAT, 2 }
		};

		Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
		OPshader* vert = (OPshader*)OPCMAN.Get("Common/Texture3D.vert");
		OPshader* frag = (OPshader*)OPCMAN.Get("Common/Texture.frag");
		Effect->Init(vert, frag);

		SphereEffect = (OPeffect*)OPalloc(sizeof(OPeffect));
		SphereEffect->Init(vert, frag);

		Camera = (OPcam*)OPalloc(sizeof(OPcam));
		Camera->SetPerspective(
			OPvec3Create(0, 20, 50),
			OPvec3Create(0, 1, 0),
			OPvec3Create(0, 1, 0),
			0.1f,
			1000.0f,
			45.0f,
			OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (f32)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height
		);

		OPphysXMaterial* material = OPphysXCreateMaterial(0.8f, 0.8f, 0.6f);

		boxCount = 200;
		boxes = (Dynamic*)OPallocZero(sizeof(Dynamic)* boxCount);
		for (ui32 i = 0; i < boxCount; i++) {
			f32 r = OPrandom();
			f32 r2 = 0.5;// OPrandom();
			f32 r3 = OPrandom();
			f32 size = (0.05f + (OPrandom() * 2));
			boxes[i].physics = OPphysXSceneCreateDynamic(scene, OPvec3Create(-20 + (40 * r), 5 + 200 * r3, -20 + (40 * r2)));
			boxes[i].shape = OPphysXAddBoxShape(boxes[i].physics, material, OPvec3Create(size, size, size));
			//OPphysXSetAsTrigger(OPphysXAddBoxShape(boxes[i].physics, material, OPvec3Create(size, size, size)));
			OPphysXSetFilter(boxes[i].physics, 1, 2);
			OPphysXSceneAddActor(scene, boxes[i].physics);
			boxes[i].size = size;
		}

		boxStaticCount = 10;
		boxesStatic = (Static*)OPallocZero(sizeof(Static)*boxStaticCount);
		for (ui32 i = 0; i < boxStaticCount; i++) {
			f32 r2 = 0.5;
			f32 size = 1;// 0.05f + (OPrandom() * 2);
			boxesStatic[i].physics = OPphysXSceneCreateStatic(scene, OPvec3Create(((-10.0f) + (i * 2)), i * 2.0f, -20.0f + (40.0f * r2)));
			boxesStatic[i].shape = OPphysXAddBoxShape(boxesStatic[i].physics, material, OPvec3Create(size, size, size));
			OPphysXSceneAddActor(scene, boxesStatic[i].physics);
			boxesStatic[i].size = size;
		}

		sphereCount = 5;
		spheres = (Dynamic*)OPallocZero(sizeof(Dynamic)* sphereCount);
		spheres[0].physics = OPphysXSceneCreateDynamic(scene, OPVEC3_ZERO);
		OPphysXAddSphereShape(spheres[0].physics, material, 1);
		OPphysXSetMass(spheres[0].physics, 1);
		OPphysXSceneAddActor(scene, spheres[0].physics);
		OPphysXSetFilter(spheres[0].physics, 2, 1);
		spheres[0].size = 1;
		for (ui32 i = 1; i < sphereCount; i++) {
			f32 r = OPrandom();
			f32 r2 = OPrandom();
			f32 r3 = OPrandom();
			f32 size = 0.1f + (OPrandom() * 2);
			spheres[i].physics = OPphysXSceneCreateDynamic(scene, OPvec3Create(-20 + (40 * r), 3 * r3, -20 + (40 * r2)));
			spheres[i].shape = OPphysXAddSphereShape(spheres[i].physics, material, size);
			OPphysXSetMass(spheres[i].physics, size);
			OPphysXSceneAddActor(scene, spheres[i].physics);
			spheres[i].size = size;
		}

		PxTransform transform = PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
		physicsPlane = OPphysXSceneCreateStatic(scene, transform);
		OPphysXAddPlaneShape(physicsPlane, material);
		OPphysXSceneAddActor(scene, physicsPlane);

	}

	OPint Update(OPtimer* time) {

		for (OPuint i = 0; i < boxCount; i++) {
			if (boxes[i].dead == 1) {
				OPphysXSceneRemove(scene, boxes[i].physics);
				boxes[i].dead++;
			}
		}

		if (OPKEYBOARD.WasPressed(OPkeyboardKey::SPACE)) {
			OPphysXAddForce(spheres[0].physics, OPvec3Create(0, 500 * spheres[0].size, 0));
		}
		f32 rate = 500 * spheres[0].size;
		f32 rate2 = 50 * spheres[0].size;
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

		if (OPKEYBOARD.IsDown(OPkeyboardKey::A)) {
			OPphysXAddTorque(spheres[0].physics, OPvec3Create(0.0f, 0.0f, rate));
			OPphysXAddForce(spheres[0].physics, OPvec3Create(-rate2, 0.0f, 0.0f));
		}
		if (OPKEYBOARD.IsDown(OPkeyboardKey::D)) {
			OPphysXAddTorque(spheres[0].physics, OPvec3Create(0.0f, 0.0f, -rate));
			OPphysXAddForce(spheres[0].physics, OPvec3Create(rate2, 0.0f, 0.0f));
		}
		if (OPKEYBOARD.IsDown(OPkeyboardKey::W)) {
			OPphysXAddTorque(spheres[0].physics, OPvec3Create(-rate, 0, 0));
			OPphysXAddForce(spheres[0].physics, OPvec3Create(0.0f, 0.0f, -rate2));
		}
		if (OPKEYBOARD.IsDown(OPkeyboardKey::S)) {
			OPphysXAddTorque(spheres[0].physics, OPvec3Create(rate, 0.0f, 0.0f));
			OPphysXAddForce(spheres[0].physics, OPvec3Create(0.0f, 0.0f, rate2));
		}

		if (OPKEYBOARD.IsDown(OPkeyboardKey::UP)) { Camera->pos.y += 0.2f; }
		if (OPKEYBOARD.IsDown(OPkeyboardKey::DOWN)) { Camera->pos.y -= 0.2f; }
		if (OPKEYBOARD.IsDown(OPkeyboardKey::LEFT)) { Camera->pos.x -= 0.2f; }
		if (OPKEYBOARD.IsDown(OPkeyboardKey::RIGHT)) { Camera->pos.x += 0.2f; }

		Camera->Update();


		OPphysXSceneUpdate(scene, time);

		OPrenderDepth(true);
		OPrenderClear(0.1f, 0.1f, 0.1f);

		OPmat4 world = OPMAT4_IDENTITY;

		Mesh->Bind();
		Effect->Bind();


		OPeffectSet("uProj", &Camera->proj);
		OPeffectSet("uView", &Camera->view);

		OPvec3 light = OPvec3Create(0, 1, 0);
		OPeffectSet("uLightDirection", &light);
		OPeffectSet("uColorTexture", texture, 0);
		OPmat4 scale = OPMAT4_IDENTITY;
		OPmat4 scratch = OPMAT4_IDENTITY;
		for (ui32 i = 0; i < boxCount; i++) {
			if (boxes[i].dead) continue;
			scale = OPmat4Scl(boxes[i].size * 2, boxes[i].size * 2, boxes[i].size * 2);
			OPphysXGetTransform((OPphysXRigidActor*)boxes[i].physics, &scratch);
			world = scratch * scale;
			OPeffectSet("uWorld", &world);
			OPrenderDrawBufferIndexed(0);
		}
		OPeffectSet("uColorTexture", textureStatic, 0);
		for (ui32 i = 0; i < boxStaticCount; i++) {
			if (boxesStatic[i].dead) continue;
			scale = OPmat4Scl(boxesStatic[i].size * 2, boxesStatic[i].size * 2, boxesStatic[i].size * 2);
			OPphysXGetTransform((OPphysXRigidActor*)boxesStatic[i].physics, &scratch);
			world = scratch * scale;
			OPeffectSet("uWorld", &world);
			OPrenderDrawBufferIndexed(0);
		}

		MeshSphere->Bind();
		SphereEffect->Bind();
		OPeffectSet("uProj", &Camera->proj);
		OPeffectSet("uView", &Camera->view);
		OPeffectSet("uLightDirection", &light);

		OPeffectSet("uColorTexture", texturePlayer, 0);
		scale = OPmat4Scl(spheres[0].size * 2, spheres[0].size * 2, spheres[0].size * 2);
		OPphysXGetTransform((OPphysXRigidActor*)spheres[0].physics, &scratch);
		world = scratch * scale;
		OPeffectSet("uWorld", &world);
		OPrenderDrawBufferIndexed(0);

		OPeffectSet("uColorTexture", textureSphere, 0);
		for (ui32 i = 1; i < sphereCount; i++) {
			scale = OPmat4Scl(spheres[i].size * 2, spheres[i].size * 2, spheres[i].size * 2);
			OPphysXGetTransform((OPphysXRigidActor*)spheres[i].physics, &scratch);
			world = scratch * scale;
			OPeffectSet("uWorld", 1, &world);
			OPrenderDrawBufferIndexed(0);
		}

		OPrenderPresent();
		return false;
	}

	void Render(OPfloat delta) {

	}

	OPint Exit(OPgameState* next) {
		OPphysXSceneDestroy(scene);
		OPphysXShutdown();
		return 0;
	}
};

PhysicsExample _GS_EXAMPLE_PHYSICS;
void OnContact(OPphysXContact contact) {
	OPlog("User Defined Contact %d", _contact++);
	for (OPuint p = 0; p < contact.count; p++) {
		const PxContactPair pair = contact.contactPairs[p];
		for (OPuint i = 0; i < _GS_EXAMPLE_PHYSICS.boxCount; i++) {
			if (pair.shapes[1] == _GS_EXAMPLE_PHYSICS.boxes[i].shape) {
				_GS_EXAMPLE_PHYSICS.boxes[i].dead = 1;
			}
		}
	}
}



OPint GS_EXAMPLE_PHYSICS_AVAILABLE = 1;
OPgameState* GS_EXAMPLE_PHYSICS = &_GS_EXAMPLE_PHYSICS;
#else
OPint GS_EXAMPLE_PHYSICS_AVAILABLE = 0;
OPgameState* GS_EXAMPLE_PHYSICS = NULL;
#endif
