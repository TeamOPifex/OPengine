#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Pipeline/include/OPmaterialPBR.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"

typedef struct {
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
} SceneExample;

SceneExample sceneExample;

void ExampleSceneEnter(OPgameState* last) {
	const OPchar* envImages[6] = {
		"Textures/TetrisBroken.png",
		"Textures/TetrisYellow.png",
		"Textures/TetrisGreen.png",
		"Textures/TetrisGray.png",
		"Textures/TetrisOrange.png",
		"Textures/TetrisRed.png"
	};
	sceneExample.environment.Init(envImages);

	sceneExample.renderer = OPNEW(OPrendererForward());
	//sceneExample.scene.Init(&sceneExample.renderer->rendererRoot, 100, 1);
	sceneExample.camera.Init();
	sceneExample.scene.camera = &sceneExample.camera.Camera;

	sceneExample.materialPBR.Init(OPNEW(OPeffect("Common/PBR.vert", "Common/PBR.frag")));
	sceneExample.materialPBR.AddParam("uCamPos", &sceneExample.camera.Camera.pos);
	sceneExample.renderer->SetMaterial(&sceneExample.materialPBR, 0);

	sceneExample.materialInstance = OPNEW(OPmaterialPBR(sceneExample.materialPBR));
	sceneExample.materialInstance->SetAlbedoMap("Dagger_Albedo.png");
	sceneExample.materialInstance->SetSpecularMap("Dagger_Specular.png");
	sceneExample.materialInstance->SetGlossMap("Dagger_Gloss.png");
	sceneExample.materialInstance->SetNormalMap("Dagger_Normals.png");
	sceneExample.materialInstance->SetEnvironmentMap(&sceneExample.environment);

	sceneExample.materialInstance2 = OPNEW(OPmaterialPBR(sceneExample.materialPBR));
	sceneExample.materialInstance2->SetAlbedoMap("cemetery_floor.png");
	sceneExample.materialInstance2->SetSpecularMap("Default_Specular.png");
	sceneExample.materialInstance2->SetGlossMap("Default_Gloss.png");
	sceneExample.materialInstance2->SetNormalMap("Default_Normals.png");
	sceneExample.materialInstance2->SetEnvironmentMap(&sceneExample.environment);

	sceneExample.model = (OPmodel*)OPCMAN.LoadGet("daggerpbr.opm");
	sceneExample.model2 = (OPmodel*)OPCMAN.LoadGet("daggerpbr.opm");
	sceneExample.model3 = (OPmodel*)OPCMAN.LoadGet("ground_block_2x2x2.fbx.opm");

	//sceneExample.materialInst1 = &sceneExample.materialInstance;
	//sceneExample.materialInst2 = &sceneExample.materialInstance2;

	sceneExample.model1Entity = sceneExample.scene.Add(sceneExample.model, &sceneExample.materialInstance);
	sceneExample.model2Entity = sceneExample.scene.Add(sceneExample.model2, &sceneExample.materialInstance);
	sceneExample.model3Entity = sceneExample.scene.Add(sceneExample.model3, &sceneExample.materialInstance2);
}

OPint ExampleSceneUpdate(OPtimer* time) {
	sceneExample.camera.Update(time);
	sceneExample.Rotation += time->Elapsed * 0.25f;
	sceneExample.model1Entity->world.SetRotY(sceneExample.Rotation / 200.0f)->Scl(1.0f);
	sceneExample.model2Entity->world.SetScl(1.0f)->Translate(-45, 0, 0);
	sceneExample.model3Entity->world.SetScl(1.0f)->Translate(45, 0, 0);
	return false;
}

void ExampleSceneRender(OPfloat delta) {
	OPrenderClear(0.2f, 0.2f, 0.2f);
	sceneExample.scene.Render(delta);
	OPVISUALDEBUGINFO.Render(delta);
	OPrenderPresent();
}

OPint ExampleSceneExit(OPgameState* next) {
	OPfree(sceneExample.materialInstance);
	sceneExample.materialPBR.effect->Destroy();
	OPfree(sceneExample.materialPBR.effect);
	sceneExample.scene.Destroy();
	sceneExample.renderer->Destroy();
	OPfree(sceneExample.renderer);
	sceneExample.environment.Destroy();

	return 0;
}

OPint GS_EXAMPLE_SCENE_AVAILABLE = 1;
OPgameState GS_EXAMPLE_SCENE = {
	ExampleSceneEnter,
	ExampleSceneUpdate,
	ExampleSceneRender,
	ExampleSceneExit
};
