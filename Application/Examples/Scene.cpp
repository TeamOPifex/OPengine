#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
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
} SceneExample;

SceneExample sceneExample;


void ExampleSceneEnter(OPgameState* last) {
	//OPeffect* result = OPNEW(OPeffect("Common/PBR.vert", "Common/PBR.frag"));
	//OPmesh* mesh = (OPmesh*)OPcmanLoadGet("boxPBR.opm");
	//mesh->vertexLayout.Log();

	sceneExample.renderer = OPNEW(OPrendererForward());
	sceneExample.scene.Init(&sceneExample.renderer->rendererRoot, 100);
	sceneExample.camera.Init();
	sceneExample.scene.camera = &sceneExample.camera.Camera;

	//sceneExample.renderer->SetMaterialEffect(OPNEW(OPeffect("ColoredModel.vert", "ColoredModel.frag")), 0);
	sceneExample.model.Init("daggerpbr.opm");
	sceneExample.model.mesh->vertexLayout.Log();
	OPsceneEntity* entity = sceneExample.scene.Add(&sceneExample.model);
	entity->material->AddParam("uAlbedoMap", (OPtexture*)OPcmanLoadGet("Dagger_Albedo.png"), 0);
	entity->material->AddParam("uSpecularMap", (OPtexture*)OPcmanLoadGet("Dagger_Specular.png"), 1);
	entity->material->AddParam("uGlossMap", (OPtexture*)OPcmanLoadGet("Dagger_Gloss.png"), 2);
	entity->material->AddParam("uNormalMap", (OPtexture*)OPcmanLoadGet("Dagger_Normals.png"), 3);

	sceneExample.albedoUsage = 1.0f;
	sceneExample.specularUsage = 1.0f;
	sceneExample.glossUsage = 1.0f;
	sceneExample.normalUsage = 1.0f;
	entity->material->AddParam("uUsingAlbedoMap", &sceneExample.albedoUsage);
	entity->material->AddParam("uUsingSpecularMap", &sceneExample.specularUsage);
	entity->material->AddParam("uUsingGlossMap", &sceneExample.glossUsage);
	entity->material->AddParam("uUsingNormalMap", &sceneExample.normalUsage);

	sceneExample.albedoColor = OPvec4(0, 0, 0, 1);
	sceneExample.normalColor = OPvec3(1);
	sceneExample.glossColor = 0.8f;
	sceneExample.specularColor = OPvec3(1, 1, 1);
	entity->material->AddParam("uAlbedoColor", &sceneExample.albedoColor);
	entity->material->AddParam("uNormalColor", &sceneExample.normalColor);
	entity->material->AddParam("uGlossColor", &sceneExample.glossColor);
	entity->material->AddParam("uSpecularColor", &sceneExample.specularColor);

	sceneExample.light.color = OPvec4(1, 1, 1, 1);
	sceneExample.light.direction = OPvec3(1, 1, 1);
	sceneExample.light.direction.Norm();
	sceneExample.light.intensity = 1.0;
	entity->material->rootMaterial->AddParam("uLight.color", &sceneExample.light.color);
	entity->material->rootMaterial->AddParam("uLight.direction", &sceneExample.light.direction);
	entity->material->rootMaterial->AddParam("uLight.intensity", &sceneExample.light.intensity);

	entity->material->rootMaterial->AddParam("uCamPos", &sceneExample.camera.Camera.pos);
}

OPint ExampleSceneUpdate(OPtimer* time) {
	sceneExample.camera.Update(time);
	if (OPkeyboardIsDown(OPkeyboardKey::SPACE)) { sceneExample.Rotation += time->Elapsed * 0.25f; }
	sceneExample.model.world.SetRotY(sceneExample.Rotation / 100.0f)->Scl(1.0f);
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
