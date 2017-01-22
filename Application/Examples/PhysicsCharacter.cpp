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

ui32 _contactCharacter = 0;
ui32 _triggerCharacter = 0;

void CharacterOnTrigger(OPphysXTrigger trigger) {
	OPlog("User Defined Trigger %d", _triggerCharacter++);
}
void CharacterOnContact(OPphysXContact contact);
void CharacterOnShapeHit(PxControllerShapeHit hit);

class PhysicsCharacterExample : public OPgameState {
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
	OPphysXControllerManager* manager;
	OPphysXController* controller;


	void Init(OPgameState* last) {

		OPphysXInit();
		scene = OPphysXSceneCreate(OPvec3Create(0.0f, -9.8f, 0.0f), CharacterOnTrigger, CharacterOnContact);

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

		boxStaticCount = 100;
		boxesStatic = (Static*)OPallocZero(sizeof(Static)*boxStaticCount);
		for (ui32 i = 0; i < boxStaticCount; i++) {
			f32 r2 = 0.5;
			f32 size = 1;// 0.05f + (OPrandom() * 2);
			boxesStatic[i].physics = OPphysXSceneCreateStatic(scene, OPvec3Create(-200 + OPrandom() * 400, 0, -200 + OPrandom() * 400));
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

		OPfloat pSize = 50;
		OPfloat pHeight = 10;

		OPvec3 verts[8] = {
			{ -pSize, -pHeight, -pSize },
			{ 0, pHeight, -pSize },
			{ 0, pHeight, pSize },
			{ -pSize, -pHeight, pSize },

			{ 0, pHeight, -pSize },
			{ pSize, -pHeight, -pSize },
			{ pSize, -pHeight, pSize },
			{ 0, pHeight, pSize }
		};

		ui32 indices[12] = {
			2, 1, 0,
			3, 2, 0,
			6, 5, 4,
			7, 6, 4
		};

		PxTriangleMesh* mesh = OPphysXCreateTriangleMesh(8, (PxVec3*)verts, 4, (PxU32*)indices);
		physicsPlane = OPphysXSceneCreateStatic(scene, OPVEC3_ZERO);
		OPphysXAddTriangleMeshShape(physicsPlane, material, mesh);
		OPphysXSceneAddActor(scene, physicsPlane);

		manager = OPphysXControllerCreateManager(scene);
		controller = OPphysXControllerCreate(manager, material, 0.5, 0.5, CharacterOnShapeHit, NULL, NULL, NULL);
	}

	OPint Update(OPtimer* time) {

		for (OPuint i = 0; i < boxCount; i++) {
			if (boxes[i].dead == 1) {
				OPphysXSceneRemove(scene, boxes[i].physics);
				boxes[i].dead++;
			}
		}

		OPvec3 disp = OPvec3Create(0.0f, -0.98f * 0.25f, 0.0f);
		OPvec2 leftThumb = OPGAMEPADS[0]->LeftThumb();
		disp.x += leftThumb.x * 0.5f;
		disp.z -= leftThumb.y * 0.5f;

		if (OPKEYBOARD.IsDown(OPkeyboardKey::D)) {
			disp.x = 0.5f;
		}
		if (OPKEYBOARD.IsDown(OPkeyboardKey::A)) {
			disp.x = -0.5f;
		}
		if (OPKEYBOARD.IsDown(OPkeyboardKey::W)) {
			disp.z = -0.5f;
		}
		if (OPKEYBOARD.IsDown(OPkeyboardKey::S)) {
			disp.z = 0.5f;
		}
		if (OPKEYBOARD.WasPressed(OPkeyboardKey::SPACE)) {
			disp.y = 5.0;
		}

		OPphysXControllerMove(controller, disp, time);
		OPvec3 pos = OPphysXControllerGetPos(controller);

		f32 rate = 500 * spheres[0].size;
		f32 rate2 = 50 * spheres[0].size;

		if (OPKEYBOARD.IsDown(OPkeyboardKey::UP)) { Camera->pos.y += 0.2f; }
		if (OPKEYBOARD.IsDown(OPkeyboardKey::DOWN)) { Camera->pos.y -= 0.2f; }
		if (OPKEYBOARD.IsDown(OPkeyboardKey::LEFT)) { Camera->pos.x -= 0.2f; }
		if (OPKEYBOARD.IsDown(OPkeyboardKey::RIGHT)) { Camera->pos.x += 0.2f; }

		Camera->Update();

		OPphysXSceneUpdate(scene, time);

		OPrenderDepth(1);
		OPrenderClear(0.1f, 0.1f, 0.1f);

		OPmat4 world = OPMAT4_IDENTITY;

		Mesh->Bind();
		Effect->Bind();

		//OPtextureClearActive();


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

		{
			world = OPmat4Translate(pos.x, pos.y, pos.z) * OPmat4Scl(1);
			OPeffectSet("uWorld", &world);
			OPrenderDrawBufferIndexed(0);
		}

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


PhysicsCharacterExample _GS_EXAMPLE_PHYSICSCHARACTER;

void CharacterOnContact(OPphysXContact contact) {
	OPlog("User Defined Contact %d", _contactCharacter++);
	for (OPuint p = 0; p < contact.count; p++) {
		const PxContactPair pair = contact.contactPairs[p];
		for (OPuint i = 0; i < _GS_EXAMPLE_PHYSICSCHARACTER.boxCount; i++) {
			if (pair.shapes[1] == _GS_EXAMPLE_PHYSICSCHARACTER.boxes[i].shape) {
				_GS_EXAMPLE_PHYSICSCHARACTER.boxes[i].dead = 1;
	}
}
	}
}

void CharacterOnShapeHit(PxControllerShapeHit hit) {
	OPlog("Hit");
	for (OPuint i = 0; i < _GS_EXAMPLE_PHYSICSCHARACTER.boxCount; i++) {
		if (hit.shape == _GS_EXAMPLE_PHYSICSCHARACTER.boxes[i].shape) {
			_GS_EXAMPLE_PHYSICSCHARACTER.boxes[i].dead = 1;
		}
	}
}

OPint GS_EXAMPLE_PHYSICSCHARACTER_AVAILABLE = 1;

OPgameState* GS_EXAMPLE_PHYSICSCHARACTER = &_GS_EXAMPLE_PHYSICSCHARACTER;
#else
OPint GS_EXAMPLE_PHYSICSCHARACTER_AVAILABLE = 0;
OPgameState* GS_EXAMPLE_PHYSICSCHARACTER = NULL;
#endif