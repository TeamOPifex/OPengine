#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/OPrenderSystem.h"

#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Rendering/Skinning/OPskeleton.h"

#include "./Human/include/Systems/OPinputSystem.h"

typedef struct {
	OPmesh* Mesh;
	OPeffect* Effect;
	OPcam* Camera;
	ui32 pos;
	OPtexture* texture;
	OPskeleton* skeleton;
	OPskeletonAnimation* animation;
	OPskeletonAnimation* animation2;
	OPskeletonAnimation* animation3;
	OPskeletonAnimation* animation4;
} SkinningExample;

SkinningExample* skinningExample;

#define SCALE 1

void ExampleSkinningEnter(OPgameState* last) {

	OPCMAN.Load("Skinning.frag");
	OPCMAN.Load("Skinning.vert");
	skinningExample = (SkinningExample*)OPalloc(sizeof(SkinningExample));

	skinningExample->skeleton = (OPskeleton*)OPCMAN.LoadGet("ld35person.opm.skel");
	skinningExample->animation = (OPskeletonAnimation*)OPCMAN.LoadGet("ld35person.opm.Take 001.anim");
	skinningExample->animation2 = (OPskeletonAnimation*)OPCMAN.LoadGet("ld35person.opm.Take 001.anim");
	skinningExample->animation3 = (OPskeletonAnimation*)OPCMAN.LoadGet("person.opm.Walk.anim");
	skinningExample->animation4 = (OPskeletonAnimation*)OPCMAN.LoadGet("person.opm.Walk.anim");


	OPCMAN.Load("Skinning.frag");
	OPCMAN.Load("Skinning.vert");

	skinningExample->pos = 0;
	skinningExample->Mesh = (OPmesh*)OPCMAN.LoadGet("ld35person.opm");

	OPshaderAttribute attribs[] = {
		{ "aPosition", OPshaderElementType::FLOAT, 3 },
		{ "aNormal", OPshaderElementType::FLOAT, 3 },
		//{ "aTangent", OPshaderElementType::FLOAT, 3 },
		{ "aUV", OPshaderElementType::FLOAT, 2 },
		{ "aBlendIndices", OPshaderElementType::FLOAT, 4 },
		{ "aBlendWeights", OPshaderElementType::FLOAT, 4 }
	};

	skinningExample->Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPCMAN.Get("Skinning.vert");
	OPshader* frag = (OPshader*)OPCMAN.Get("Skinning.frag");
	skinningExample->Effect->Init(vert, frag);

	skinningExample->Camera = (OPcam*)OPalloc(sizeof(OPcam));
	f32 pos = 10 * SCALE;
	skinningExample->Camera->SetPerspective(
		OPvec3Create(pos, pos, pos),
		OPvec3Create(0, pos / 4.0f, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		500.0f,
		45.0f,
		OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width / (f32)OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height
		);

		skinningExample->texture = (OPtexture*)OPCMAN.LoadGet("Knight.png");
}

OPint heldDown = 0;

OPint ExampleSkinningUpdate(OPtimer* time) {
	OPrenderDepth(1);
	OPrenderClear(0, 0, 0);

	if (OPkeyboardWasPressed(OPkeyboardKey::P)) { skinningExample->pos++; }
	if (OPkeyboardWasPressed(OPkeyboardKey::O)) { skinningExample->pos--; }

	//if (OPkeyboardWasPressed(OPkeyboardKey::M)) { skinningExample->Mesh->SkeletonAnimation.Frame++; }
	//if (OPkeyboardWasPressed(OPkeyboardKey::N)) { skinningExample->Mesh->SkeletonAnimation.Frame--; }

	if (OPkeyboardIsDown(OPkeyboardKey::UP)) { skinningExample->Camera->pos.y += (OPfloat)(0.1 * SCALE); }
	if (OPkeyboardIsDown(OPkeyboardKey::DOWN)) { skinningExample->Camera->pos.y -= (OPfloat)(0.1 * SCALE); }
	if (OPkeyboardIsDown(OPkeyboardKey::LEFT)) { skinningExample->Camera->pos.x -= (OPfloat)(0.1 * SCALE); }
	if (OPkeyboardIsDown(OPkeyboardKey::RIGHT)) { skinningExample->Camera->pos.x += (OPfloat)(0.1 * SCALE); }

	skinningExample->Camera->Update();
	skinningExample->Camera->UpdateView();

	if (OPkeyboardWasPressed(OPkeyboardKey::N)) {
		skinningExample->animation2->Elapsed = skinningExample->animation2->Frame = 0;
	}
	if (OPkeyboardWasPressed(OPkeyboardKey::M)) {
		skinningExample->animation3->Elapsed = skinningExample->animation3->Frame = 0;
	}
	if (OPkeyboardWasPressed(OPkeyboardKey::B)) {
		skinningExample->animation4->Elapsed = skinningExample->animation4->Frame = 0;
	}

	if (OPkeyboardIsDown(OPkeyboardKey::N)) {
		heldDown += time->Elapsed;
		if (heldDown > 500) heldDown = 500;
		OPskeletonAnimationUpdate(skinningExample->animation, time);
		OPskeletonAnimationUpdate(skinningExample->animation2, time);
		OPskeletonAnimationMerge(skinningExample->animation, skinningExample->animation2, heldDown / 500.0f);
		OPskeletonAnimationApply(skinningExample->animation, skinningExample->skeleton);
	}
	else if (OPkeyboardIsDown(OPkeyboardKey::M)) {
		OPskeletonAnimationUpdate(skinningExample->animation3, time);
		OPskeletonAnimationApply(skinningExample->animation3, skinningExample->skeleton);
	}
	else if (OPkeyboardIsDown(OPkeyboardKey::B)) {
		OPskeletonAnimationUpdate(skinningExample->animation4, time);
		OPskeletonAnimationApply(skinningExample->animation4, skinningExample->skeleton);
	}
	else {
		heldDown -= time->Elapsed;
		if (heldDown < 0) heldDown = 0;
		OPskeletonAnimationUpdate(skinningExample->animation, time);
		OPskeletonAnimationUpdate(skinningExample->animation2, time);
		OPskeletonAnimationMerge(skinningExample->animation, skinningExample->animation2, heldDown / 500.0f);
		OPskeletonAnimationApply(skinningExample->animation, skinningExample->skeleton);
	}
	//OPmat4Translate(&mesh->Skeleton->localPoses[pos], time->Elapsed / 1000.0f, 0, 0);
	OPmat4RotZ(&skinningExample->skeleton->localPoses[skinningExample->pos], OPkeyboardIsDown(OPkeyboardKey::W) / 100.0f);
	OPmat4RotZ(&skinningExample->skeleton->localPoses[skinningExample->pos], OPkeyboardIsDown(OPkeyboardKey::S) / -100.0f);
	OPskeletonUpdate(skinningExample->skeleton);

	skinningExample->Mesh->Bind();
	skinningExample->Effect->Bind();

	OPmat4 world;
	OPmat4Identity(&world);
	//OPmat4BuildRotX(&world,- OPpi / 2.0);

	OPeffectSet("uWorld", &world);
	OPeffectSet("uView", &skinningExample->Camera->view);
	OPeffectSet("uProj", &skinningExample->Camera->proj);

	OPeffectSet("uBones", skinningExample->skeleton->hierarchyCount, skinningExample->skeleton->skinned);

	OPvec3 light = OPvec3Create(0, 10, 0);
	OPeffectSet("uLightPosition", &light);

	OPeffectSet("uColorTexture", skinningExample->texture, 0);

	OPmeshRender();

	OPrenderPresent();
	return false;
}
void ExampleSkinningRender(OPfloat delta) {

}
OPint ExampleSkinningExit(OPgameState* next) {
	skinningExample->Effect->Destroy();
	OPfree(skinningExample->Effect);
	OPfree(skinningExample->Camera);
	return 0;
}

OPint GS_EXAMPLE_SKINNING_AVAILABLE = 1;
OPgameState GS_EXAMPLE_SKINNING = {
	ExampleSkinningEnter,
	ExampleSkinningUpdate,
	ExampleSkinningRender,
	ExampleSkinningExit
};
