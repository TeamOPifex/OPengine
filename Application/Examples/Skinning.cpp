#include "./ExampleSelectorState.h"
#include "./Human/include/Systems/OPrenderSystem.h"

#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPMvertex.h"
#include "./Human/include/Rendering/Skinning/OPskeleton.h"

#include "./Human/include/Systems/OPinputSystem.h"
#include "./Pipeline/include/Materials/OPmaterialSkinned.h"
#include "./Pipeline/include/Renderers/OPrendererForward.h"
#include "./Pipeline/include/Renderers/OPrendererDeferred.h"

class SkinningExample : public OPgameState {
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


	void Init(OPgameState* last) {
		freeFlight.Init(25.0, 1.0, OPvec3(0, scale * 10, scale * 10));

		scene.Init(&rendererForward, 100, 100);
		scene.SetCamera(&freeFlight.Camera);

		OPfloat shadowCameraSize = 16.0f;
		shadowCamera.SetOrtho(OPvec3(-2, 5, 1), OPVEC3_ZERO, OPVEC3_UP, 25.0f, 150.0f, -shadowCameraSize, shadowCameraSize, -shadowCameraSize, shadowCameraSize);
		scene.SetShadowCamera(&shadowCamera);

		color = OPVEC3_ONE;
		colorAmount = 0.0f;

		Mesh = OPmodel::Load("archer.opm");
		skeleton = OPskeleton::Load("archer.opm.skel");
		animation = OPskeletonAnimation::Load("archer.opm.walk.anim");
		animation2 = OPskeletonAnimation::Load("archer.opm.run.anim");
		animation3 = OPskeletonAnimation::Load("archer.opm.jump.anim");
		animation4 = OPskeletonAnimation::Load("archer.opm.attack.anim");

		OPrendererEntity* entity = scene.Add(Mesh, skeleton, OPrendererEntityDesc(true, true, true, false));
		//(*entity->material).AddParam("uColor", &color);
		//(*entity->material).AddParam("uColorAmount", &colorAmount);
		entity->world.SetScl(0.1);
	}

	OPint Update(OPtimer* time) {


		// Update the camera
		freeFlight.Update(time);

		///
		// Update the Skinning Animation data
		///
		if (OPKEYBOARD.WasPressed(OPkeyboardKey::N)) {
			animation2->Elapsed = animation2->Frame = 0;
		}
		if (OPKEYBOARD.WasPressed(OPkeyboardKey::M)) {
			animation3->Elapsed = animation3->Frame = 0;
		}
		if (OPKEYBOARD.WasPressed(OPkeyboardKey::B)) {
			animation4->Elapsed = animation4->Frame = 0;
		}

		if (OPKEYBOARD.IsDown(OPkeyboardKey::N)) {
			heldDown += time->Elapsed;
			if (heldDown > 500) heldDown = 500;
			animation->Update(time);
			animation2->Update(time);
			animation->Merge(animation2, heldDown / 500.0f);
			animation->Apply(skeleton);
		}
		else if (OPKEYBOARD.IsDown(OPkeyboardKey::M)) {
			animation3->Update(time);
			animation3->Apply(skeleton);
		}
		else if (OPKEYBOARD.IsDown(OPkeyboardKey::B)) {
			animation4->Update(time);
			animation4->Apply(skeleton);
		}
		else {
			heldDown -= time->Elapsed;
			if (heldDown < 0) heldDown = 0;
			animation->Update(time);
			animation2->Update(time);
			animation->Merge(animation2, heldDown / 500.0f);
			animation->Apply(skeleton);
		}

		skeleton->Update();

		return false;
	}

	void Render(OPfloat delta) {
		OPrenderClear(0.1);
		scene.Render(delta);
		OPrenderPresent();
	}

	OPint Exit(OPgameState* next) {
		//Effect.Destroy();
		return 0;
	}
};


OPint GS_EXAMPLE_SKINNING_AVAILABLE = 1;
SkinningExample _GS_EXAMPLE_SKINNING;
OPgameState* GS_EXAMPLE_SKINNING = &_GS_EXAMPLE_SKINNING;