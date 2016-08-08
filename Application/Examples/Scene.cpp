#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Pipeline/include/OPmaterialPBR.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"

struct Light {
	OPvec4 color;
	OPvec3 position;
	OPfloat p0;
	OPvec3 direction;
	OPfloat p1;
	OPvec3 lightVector;
	OPfloat intensity;
};

// Data for this Game State Example
typedef struct {
	OPfloat Rotation;			// The amount to rotate the Mesh

	OPscene scene;
	OPrendererForward* renderer;
	OPmodel model;
	OPmodel model2;
	OPmat4 world;
	OPfloat albedoUsage;
	OPfloat specularUsage;
	OPfloat glossUsage;
	OPfloat normalUsage;
	OPvec4 albedoColor;
	OPvec3 specularColor;
	OPfloat glossColor;
	OPvec3 normalColor;
	Light light;
	OPcamFreeFlight camera;
	OPmaterialPBR materialPBR;
	OPtextureCube environment;
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
	sceneExample.scene.Init(&sceneExample.renderer->rendererRoot, 100);
	sceneExample.camera.Init();
	sceneExample.scene.camera = &sceneExample.camera.Camera;

	sceneExample.materialPBR.Init(OPNEW(OPeffect("Common/PBR.vert", "Common/PBR.frag")));
	sceneExample.renderer->SetMaterial(&sceneExample.materialPBR.rootMaterial, 0);

	sceneExample.model.Init("daggerpbr.opm");
	sceneExample.model.mesh->vertexLayout.Log();
	sceneExample.model2.Init("daggerpbr.opm");

	OPmaterialPBRInstance* materialInstance = sceneExample.materialPBR.CreateInstance();
	materialInstance->SetAlbedoMap("Dagger_Albedo.png");
	materialInstance->SetSpecularMap("Dagger_Specular.png");
	materialInstance->SetGlossMap("Dagger_Gloss.png");
	materialInstance->SetNormalMap("Dagger_Normals.png");
	materialInstance->SetEnvironmentMap(&sceneExample.environment);
	OPsceneEntity* entity = sceneExample.scene.Add(&sceneExample.model, &materialInstance->rootMaterialInstance);
	OPsceneEntity* entity2 = sceneExample.scene.Add(&sceneExample.model2, &materialInstance->rootMaterialInstance);

	entity->material->rootMaterial->AddParam("uCamPos", &sceneExample.camera.Camera.pos);
}

OPint ExampleSceneUpdate(OPtimer* time) {
	sceneExample.camera.Update(time);
	sceneExample.Rotation += time->Elapsed * 0.25f;
	sceneExample.model.world.SetRotY(sceneExample.Rotation / 200.0f)->Scl(1.0f);
	sceneExample.model2.world.SetScl(1.0f)->Translate(-45, 0, 0);
	return false;
}

void ExampleSceneRender(OPfloat delta) {
	OPrenderClear(0.2f, 0.2f, 0.2f);
	sceneExample.scene.Render(delta);
	OPrenderPresent();
}

OPint ExampleSceneExit(OPgameState* next) {
	OPfree(sceneExample.renderer);
	return 0;
}

OPint GS_EXAMPLE_SCENE_AVAILABLE = 1;
OPgameState GS_EXAMPLE_SCENE = {
	ExampleSceneEnter,
	ExampleSceneUpdate,
	ExampleSceneRender,
	ExampleSceneExit
};
