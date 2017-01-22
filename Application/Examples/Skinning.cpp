#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/OPrenderSystem.h"

#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Rendering/Skinning/OPskeleton.h"

#include "./Human/include/Systems/OPinputSystem.h"
#include "./Pipeline/include/Materials/OPmaterialSkinned.h"
#include "./Pipeline/include/Renderers/OPrendererForward.h"
#include "./Pipeline/include/Renderers/OPrendererDeferred.h"

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
	OPmaterialSkinned* materialSkinnedInstance;
	OPcam shadowCamera;

	OPvec3 color;
	f32 colorAmount;
} SkinningExample;

SkinningExample skinningExample;

void ExampleSkinningEnter(OPgameState* last) {
	skinningExample.freeFlight.Init(25.0, 1.0, OPvec3(0, skinningExample.scale * 10, skinningExample.scale * 10));

	skinningExample.scene.Init(&skinningExample.rendererForward, 100, 100);
	skinningExample.scene.SetCamera(&skinningExample.freeFlight.Camera);

	OPfloat shadowCameraSize = 16.0f;
	skinningExample.shadowCamera.SetOrtho(OPvec3(-2, 5, 1), OPVEC3_ZERO, OPVEC3_UP, 25.0f, 150.0f, -shadowCameraSize, shadowCameraSize, -shadowCameraSize, shadowCameraSize);
	skinningExample.scene.SetShadowCamera(&skinningExample.shadowCamera);

	skinningExample.color = OPVEC3_ONE;
	skinningExample.colorAmount = 0.0f;
	
	skinningExample.Mesh = (OPmodel*)OPCMAN.LoadGet("archer.opm");
	skinningExample.skeleton = (OPskeleton*)OPCMAN.LoadGet("archer.opm.skel");
	skinningExample.animation = (OPskeletonAnimation*)OPCMAN.LoadGet("archer.opm.walk.anim");
	skinningExample.animation2 = (OPskeletonAnimation*)OPCMAN.LoadGet("archer.opm.run.anim");
	skinningExample.animation3 = (OPskeletonAnimation*)OPCMAN.LoadGet("archer.opm.jump.anim");
	skinningExample.animation4 = (OPskeletonAnimation*)OPCMAN.LoadGet("archer.opm.attack.anim");

	OPrendererEntity* entity = skinningExample.scene.Add(skinningExample.Mesh, skinningExample.skeleton, OPrendererEntityDesc(true, true, true, false));
	//(*entity->material).AddParam("uColor", &skinningExample.color);
	//(*entity->material).AddParam("uColorAmount", &skinningExample.colorAmount);
	entity->world.SetScl(0.1);
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
		skinningExample.animation->Update(time);
		skinningExample.animation2->Update(time);
		skinningExample.animation->Merge(skinningExample.animation2, skinningExample.heldDown / 500.0f);
		skinningExample.animation->Apply(skinningExample.skeleton);
	}
	else if (OPKEYBOARD.IsDown(OPkeyboardKey::M)) {
		skinningExample.animation3->Update(time);
		skinningExample.animation3->Apply(skinningExample.skeleton);
	}
	else if (OPKEYBOARD.IsDown(OPkeyboardKey::B)) {
		skinningExample.animation4->Update(time);
		skinningExample.animation4->Apply(skinningExample.skeleton);
	}
	else {
		skinningExample.heldDown -= time->Elapsed;
		if (skinningExample.heldDown < 0) skinningExample.heldDown = 0;
		skinningExample.animation->Update(time);
		skinningExample.animation2->Update(time);
		skinningExample.animation->Merge(skinningExample.animation2, skinningExample.heldDown / 500.0f);
		skinningExample.animation->Apply(skinningExample.skeleton);
	}

	skinningExample.skeleton->Update();

	return false;
}

void ExampleSkinningRender(OPfloat delta) {
	OPrenderClear(0.1);
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
