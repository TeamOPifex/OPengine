#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/OPrenderSystem.h"

#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Rendering/Skinning/OPskeleton.h"

#include "./Human/include/Systems/OPinputSystem.h"
#include "./Pipeline/include/Materials/OPmaterialSkinned.h"

typedef struct {
	OPmodel* Mesh;
	OPeffect Effect;
	OPtexture* texture;
	OPskeleton* skeleton;
	OPskeletonAnimation* animation;
	OPskeletonAnimation* animation2;
	OPskeletonAnimation* animation3;
	OPskeletonAnimation* animation4;
	OPcamFreeFlight freeFlight;
	OPint heldDown = 0;
	OPfloat scale = 100.0f;
	OPscene scene;
	OPrendererForward rendererForward;
	OPmaterialSkinned materialSkinned;
	OPmaterialSkinnedInstance* materialSkinnedInstance;
} SkinningExample;

SkinningExample skinningExample;

void ExampleSkinningEnter(OPgameState* last) {
	skinningExample.freeFlight.Init(25.0, 1.0, OPvec3(0, skinningExample.scale * 10, skinningExample.scale * 10));

	skinningExample.scene.Init(&skinningExample.rendererForward.rendererRoot, 100, 100);
	skinningExample.scene.camera = &skinningExample.freeFlight.Camera;

	
	// Initializes a wrapper around the skinning animation material
	skinningExample.materialSkinnedInstance = skinningExample.materialSkinned.Init(
		skinningExample.rendererForward.passes[(ui32)OPrendererForwardPass::ANIMATED_MODEL])->
		CreateInstance();


	skinningExample.Mesh = (OPmodel*)OPCMAN.LoadGet("swordsman.opm");
	skinningExample.skeleton = (OPskeleton*)OPCMAN.LoadGet("swordsman.opm.skel");
	skinningExample.animation = (OPskeletonAnimation*)OPCMAN.LoadGet("swordsman.opm.walk.anim");
	skinningExample.animation2 = (OPskeletonAnimation*)OPCMAN.LoadGet("swordsman.opm.run.anim");
	skinningExample.animation3 = (OPskeletonAnimation*)OPCMAN.LoadGet("swordsman.opm.jump.anim");
	skinningExample.animation4 = (OPskeletonAnimation*)OPCMAN.LoadGet("swordsman.opm.default attack.anim");
	skinningExample.materialSkinnedInstance->SetAlbedoMap("swordsman.png");


	skinningExample.materialSkinnedInstance->SetBones(skinningExample.skeleton);
	OPrendererEntity* entity = skinningExample.scene.Add(skinningExample.Mesh, &skinningExample.materialSkinnedInstance->rootMaterialInstance);
	entity->shadowMaterial = (OPmaterialInstance**)skinningExample.rendererForward.defaultShadowAnimatedMaterialInstance;
	skinningExample.rendererForward.defaultShadowAnimatedMaterialInstance->AddParam("uBones", skinningExample.skeleton->skinned, skinningExample.skeleton->hierarchyCount);
}

OPint ExampleSkinningUpdate(OPtimer* time) {


	// Update the camera
	skinningExample.freeFlight.Update(time);
	
	///
	// Update the Skinning Animation data
	///
	if (OPKEYBOARD.WasPressed(OPkeyboardKey::N)) {
		skinningExample.animation2->Elapsed = skinningExample.animation2->Frame = 0;
	}
	if (OPKEYBOARD.WasPressed(OPkeyboardKey::M)) {
		skinningExample.animation3->Elapsed = skinningExample.animation3->Frame = 0;
	}
	if (OPKEYBOARD.WasPressed(OPkeyboardKey::B)) {
		skinningExample.animation4->Elapsed = skinningExample.animation4->Frame = 0;
	}

	if (OPKEYBOARD.IsDown(OPkeyboardKey::N)) {
		skinningExample.heldDown += time->Elapsed;
		if (skinningExample.heldDown > 500) skinningExample.heldDown = 500;
		OPskeletonAnimationUpdate(skinningExample.animation, time);
		OPskeletonAnimationUpdate(skinningExample.animation2, time);
		OPskeletonAnimationMerge(skinningExample.animation, skinningExample.animation2, skinningExample.heldDown / 500.0f);
		OPskeletonAnimationApply(skinningExample.animation, skinningExample.skeleton);
	}
	else if (OPKEYBOARD.IsDown(OPkeyboardKey::M)) {
		OPskeletonAnimationUpdate(skinningExample.animation3, time);
		OPskeletonAnimationApply(skinningExample.animation3, skinningExample.skeleton);
	}
	else if (OPKEYBOARD.IsDown(OPkeyboardKey::B)) {
		OPskeletonAnimationUpdate(skinningExample.animation4, time);
		OPskeletonAnimationApply(skinningExample.animation4, skinningExample.skeleton);
	}
	else {
		skinningExample.heldDown -= time->Elapsed;
		if (skinningExample.heldDown < 0) skinningExample.heldDown = 0;
		OPskeletonAnimationUpdate(skinningExample.animation, time);
		OPskeletonAnimationUpdate(skinningExample.animation2, time);
		OPskeletonAnimationMerge(skinningExample.animation, skinningExample.animation2, skinningExample.heldDown / 500.0f);
		OPskeletonAnimationApply(skinningExample.animation, skinningExample.skeleton);
	}
	OPskeletonUpdate(skinningExample.skeleton);

	return false;
}

void ExampleSkinningRender(OPfloat delta) {
	///
	// Render the actual skinned model
	///
	//OPrenderDepth(1);
	OPrenderClear(0.1);

	//skinningExample.Mesh->Bind();
	//skinningExample.Effect.Bind();

	//OPmat4 world;
	//OPmat4Identity(&world);

	//OPeffectSet("uWorld", &world);
	//OPeffectSet("uView", &skinningExample.freeFlight.Camera.view);
	//OPeffectSet("uProj", &skinningExample.freeFlight.Camera.proj);

	//OPeffectSet("uBones", skinningExample.skeleton->hierarchyCount, skinningExample.skeleton->skinned);

	//OPvec3 light = OPvec3Create(0, 10, 0);
	//OPeffectSet("uLightPosition", &light);

	//OPeffectSet("uColorTexture", skinningExample.texture, 0);

	//OPrenderDrawBufferIndexed(0);

	skinningExample.scene.Render(delta);

	OPrenderPresent();
}

OPint ExampleSkinningExit(OPgameState* next) {
	//skinningExample.Effect.Destroy();
	return 0;
}

OPint GS_EXAMPLE_SKINNING_AVAILABLE = 1;
OPgameState GS_EXAMPLE_SKINNING = {
	ExampleSkinningEnter,
	ExampleSkinningUpdate,
	ExampleSkinningRender,
	ExampleSkinningExit
};
