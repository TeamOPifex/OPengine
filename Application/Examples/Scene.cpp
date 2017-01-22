#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Pipeline/include/OPmaterialPBR.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"

class SceneExample : public OPgameState {
	OPfloat Rotation;
	OPscene scene;
	OPrendererForward* renderer;
	OPmodel* model;
	OPmodel* model2;
	OPmodel* model3;
	OPrendererEntity* model1Entity;
	OPrendererEntity* model2Entity;
	OPrendererEntity* model3Entity;
	OPcamFreeFlight camera;
	OPmaterialPBR materialPBR;
	OPtextureCube environment;
	OPmaterialPBR* materialInstance;
	OPmaterialPBR* materialInstance2;

	OPmaterial* materialInst1;
	OPmaterial* materialInst2;


	void Init(OPgameState* last) {
		const OPchar* envImages[6] = {
			"Textures/TetrisBroken.png",
			"Textures/TetrisYellow.png",
			"Textures/TetrisGreen.png",
			"Textures/TetrisGray.png",
			"Textures/TetrisOrange.png",
			"Textures/TetrisRed.png"
		};
		environment.Init(envImages);

		renderer = OPNEW(OPrendererForward());
		//scene.Init(&renderer->rendererRoot, 100, 1);
		camera.Init();
		scene.camera = &camera.Camera;

		materialPBR.Init(OPNEW(OPeffect("Common/PBR.vert", "Common/PBR.frag")));
		materialPBR.AddParam("uCamPos", &camera.Camera.pos);
		//renderer->SetMaterial(&materialPBR, 0);

		materialInstance = OPNEW(OPmaterialPBR(materialPBR));
		materialInstance->SetAlbedoMap("Dagger_Albedo.png");
		materialInstance->SetSpecularMap("Dagger_Specular.png");
		materialInstance->SetGlossMap("Dagger_Gloss.png");
		materialInstance->SetNormalMap("Dagger_Normals.png");
		materialInstance->SetEnvironmentMap(&environment);

		materialInstance2 = OPNEW(OPmaterialPBR(materialPBR));
		materialInstance2->SetAlbedoMap("cemetery_floor.png");
		materialInstance2->SetSpecularMap("Default_Specular.png");
		materialInstance2->SetGlossMap("Default_Gloss.png");
		materialInstance2->SetNormalMap("Default_Normals.png");
		materialInstance2->SetEnvironmentMap(&environment);

		model = (OPmodel*)OPCMAN.LoadGet("daggerpbr.opm");
		model2 = (OPmodel*)OPCMAN.LoadGet("daggerpbr.opm");
		model3 = (OPmodel*)OPCMAN.LoadGet("ground_block_2x2x2.fbx.opm");

		//materialInst1 = &materialInstance;
		//materialInst2 = &materialInstance2;

		model1Entity = scene.Add(model, &materialInstance);
		model2Entity = scene.Add(model2, &materialInstance);
		model3Entity = scene.Add(model3, &materialInstance2);
	}

	OPint Update(OPtimer* time) {
		camera.Update(time);
		Rotation += time->Elapsed * 0.25f;
		model1Entity->world.SetRotY(Rotation / 200.0f)->Scl(1.0f);
		model2Entity->world.SetScl(1.0f)->Translate(-45, 0, 0);
		model3Entity->world.SetScl(1.0f)->Translate(45, 0, 0);
		return false;
	}

	void Render(OPfloat delta) {
		OPrenderClear(0.2f, 0.2f, 0.2f);
		scene.Render(delta);
		OPVISUALDEBUGINFO.Render(delta);
		OPrenderPresent();
	}

	OPint Exit(OPgameState* next) {
		OPfree(materialInstance);
		materialPBR.effect->Destroy();
		OPfree(materialPBR.effect);
		scene.Destroy();
		renderer->Destroy();
		OPfree(renderer);
		environment.Destroy();

		return 0;
	}
};



OPint GS_EXAMPLE_SCENE_AVAILABLE = 1;
SceneExample _GS_EXAMPLE_SCENE;
OPgameState* GS_EXAMPLE_SCENE = &_GS_EXAMPLE_SCENE;