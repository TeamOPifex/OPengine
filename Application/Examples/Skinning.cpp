#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/OPrenderSystem.h"

#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Rendering/Skinning/OPskeleton.h"

#include "./Human/include/Systems/OPinputSystem.h"

void ExampleSkinningEnter(OPgameState* last);
OPint ExampleSkinningUpdate(OPtimer* time);
void ExampleSkinningExit(OPgameState* next);

OPgameState GS_EXAMPLE_SKINNING = {
	ExampleSkinningEnter,
	ExampleSkinningUpdate,
	ExampleSkinningExit
};

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

#define SCALE 20

void ExampleSkinningEnter(OPgameState* last) {
	
	OPcmanLoad("Skinning.frag");
	OPcmanLoad("Skinning.vert");
	skinningExample = (SkinningExample*)OPalloc(sizeof(SkinningExample));

	skinningExample->skeleton = (OPskeleton*)OPcmanLoadGet("blobman.opm.skel");
	skinningExample->animation = (OPskeletonAnimation*)OPcmanLoadGet("blobman.opm.Take 001.anim");
	skinningExample->animation2 = (OPskeletonAnimation*)OPcmanLoadGet("blobman.opm.Take 001.anim");
	skinningExample->animation3 = (OPskeletonAnimation*)OPcmanLoadGet("blobman.opm.Take 001.anim");
	skinningExample->animation4 = (OPskeletonAnimation*)OPcmanLoadGet("blobman.opm.Take 001.anim");


	skinningExample->texture = (OPtexture*)OPcmanLoadGet("noneNorm.png");

	skinningExample->pos = 0;
	skinningExample->Mesh = (OPmesh*)OPcmanLoadGet("blobman.opm");

	OPshaderAttribute attribs[] = {
		{ "aPosition", GL_FLOAT, 3 },
		{ "aNormal", GL_FLOAT, 3 },
		//{ "aTangent", GL_FLOAT, 3 },
		{ "aUV", GL_FLOAT, 2 },
		{ "aBlendIndices", GL_FLOAT, 4 },
		{ "aBlendWeights", GL_FLOAT, 4 }
	};

	skinningExample->Effect = (OPeffect*)OPalloc(sizeof(OPeffect));
	OPshader* vert = (OPshader*)OPcmanGet("Skinning.vert");
	OPshader* frag = (OPshader*)OPcmanGet("Skinning.frag");
	*skinningExample->Effect = OPeffectCreate(
		*vert,
		*frag,
		attribs,
		5,
		"Model Effect",
		skinningExample->Mesh->VertexSize
		);

	skinningExample->Camera = (OPcam*)OPalloc(sizeof(OPcam));
	f32 pos = 10 * SCALE;
	*skinningExample->Camera = OPcamPersp(
		OPvec3Create(pos, pos, pos),
		OPvec3Create(0, pos / 4.0, 0),
		OPvec3Create(0, 1, 0),
		0.1f,
		500.0f,
		45.0f,
		OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
		);
}

OPint heldDown = 0;

OPint ExampleSkinningUpdate(OPtimer* time) {
	OPrenderDepth(1);
	OPrenderClear(0, 0, 0);

	if (OPkeyboardWasPressed(OPKEY_P)) { skinningExample->pos++; }
	if (OPkeyboardWasPressed(OPKEY_O)) { skinningExample->pos--; }

	//if (OPkeyboardWasPressed(OPKEY_M)) { skinningExample->Mesh->SkeletonAnimation.Frame++; }
	//if (OPkeyboardWasPressed(OPKEY_N)) { skinningExample->Mesh->SkeletonAnimation.Frame--; }

	if (OPkeyboardIsDown(OPKEY_UP)) { skinningExample->Camera->_pos.y += 0.1 * SCALE; }
	if (OPkeyboardIsDown(OPKEY_DOWN)) { skinningExample->Camera->_pos.y -= 0.1 * SCALE; }
	if (OPkeyboardIsDown(OPKEY_LEFT)) { skinningExample->Camera->_pos.x -= 0.1 * SCALE; }
	if (OPkeyboardIsDown(OPKEY_RIGHT)) { skinningExample->Camera->_pos.x += 0.1 * SCALE; }

	skinningExample->Camera->_viewStale = 1;
	OPcamUpdateView((*skinningExample->Camera));

	if (OPkeyboardWasPressed(OPKEY_N)) {
		skinningExample->animation2->Elapsed = skinningExample->animation2->Frame = 0;
	}
	if (OPkeyboardWasPressed(OPKEY_M)) {
		skinningExample->animation3->Elapsed = skinningExample->animation3->Frame = 0;
	}
	if (OPkeyboardWasPressed(OPKEY_B)) {
		skinningExample->animation4->Elapsed = skinningExample->animation4->Frame = 0;
	}

	if (OPkeyboardIsDown(OPKEY_N)) {
		heldDown += time->Elapsed;
		if (heldDown > 500) heldDown = 500;
		OPskeletonAnimationUpdate(skinningExample->animation, time);
		OPskeletonAnimationUpdate(skinningExample->animation2, time);
		OPskeletonAnimationMerge(skinningExample->animation, skinningExample->animation2, heldDown / 500.0f, skinningExample->skeleton);
	}
	else if (OPkeyboardIsDown(OPKEY_M)) {
		OPskeletonAnimationUpdate(skinningExample->animation3, time);
		OPskeletonAnimationApply(skinningExample->animation3, skinningExample->skeleton);
	}
	else if (OPkeyboardIsDown(OPKEY_B)) {
		OPskeletonAnimationUpdate(skinningExample->animation4, time);
		OPskeletonAnimationApply(skinningExample->animation4, skinningExample->skeleton);
	}
	else {
		heldDown -= time->Elapsed;
		if (heldDown < 0) heldDown = 0;
		OPskeletonAnimationUpdate(skinningExample->animation, time);
		OPskeletonAnimationUpdate(skinningExample->animation2, time);
		OPskeletonAnimationMerge(skinningExample->animation, skinningExample->animation2, heldDown / 500.0f, skinningExample->skeleton);
	}
	//OPmat4Translate(&mesh->Skeleton->localPoses[pos], time->Elapsed / 1000.0f, 0, 0);
	OPmat4RotZ(&skinningExample->skeleton->localPoses[skinningExample->pos], OPkeyboardIsDown(OPKEY_W) / 100.0f);
	OPmat4RotZ(&skinningExample->skeleton->localPoses[skinningExample->pos], OPkeyboardIsDown(OPKEY_S) / -100.0f);
	OPskeletonUpdate(skinningExample->skeleton);

	OPmeshBind(skinningExample->Mesh);
	OPeffectBind(skinningExample->Effect);
	
	OPmat4 world, view, proj;
	OPmat4Identity(&world);
	//OPmat4BuildRotX(&world,- OPpi / 2.0);

	OPcamGetView((*skinningExample->Camera), &view);
	OPcamGetProj((*skinningExample->Camera), &proj);

	OPeffectParamMat4("uWorld", &world);
	OPeffectParamMat4("uView", &view);
	OPeffectParamMat4("uProj", &proj);

	OPeffectParamMat4v("uBones", skinningExample->skeleton->hierarchyCount, skinningExample->skeleton->skinned);

	OPvec3 light = OPvec3Create(0, 10, 0);
	OPeffectParamVec3("uLightPosition", &light);

	OPtextureClearActive();
	OPeffectParami("uColorTexture", OPtextureBind(skinningExample->texture));

	OPmeshRender();

	OPrenderPresent();
	return false;
}

void ExampleSkinningExit(OPgameState* next) {
	OPeffectUnload(skinningExample->Effect);
	OPfree(skinningExample->Effect);
	OPfree(skinningExample->Camera);
}