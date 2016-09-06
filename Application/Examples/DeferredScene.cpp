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
	OPrendererDeferred* renderer;
	OPmodel* model;
	OPsceneEntity* model1Entity;
	OPcamFreeFlight camera;
	OPtexture2DOLD* texture;
} DeferredSceneExample;

DeferredSceneExample deferredSceneExample;

void ExampleDeferredSceneEnter(OPgameState* last) {

	deferredSceneExample.renderer = OPNEW(OPrendererDeferred());
	deferredSceneExample.scene.Init(&deferredSceneExample.renderer->rendererRoot, 100, 100);
	deferredSceneExample.camera.Init();
	deferredSceneExample.scene.camera = &deferredSceneExample.camera.Camera;

	OPsceneLight* light = deferredSceneExample.scene.Add(OPlightSpot());
	light->light.radius = 5.0f;
	light->world.SetIdentity()->Scl(25.0f);

	deferredSceneExample.model = (OPmodel*)OPCMAN.LoadGet("uvsphere20.opm");

	deferredSceneExample.model1Entity = deferredSceneExample.scene.Add(deferredSceneExample.model);

	deferredSceneExample.model1Entity->material[0]->AddParam("uAlbedoMap", (OPtexture*)OPCMAN.LoadGet("Dagger_Albedo.png"), 0);
	deferredSceneExample.model1Entity->material[0]->AddParam("uSpecularMap", (OPtexture*)OPCMAN.LoadGet("Dagger_Albedo.png"), 1);

	//deferredSceneExample.texture = OPtexture2DCreate(&deferredSceneExample.renderer->gBuffer.texture[2]);
}

OPint ExampleDeferredSceneUpdate(OPtimer* time) {
	deferredSceneExample.camera.Update(time);
	deferredSceneExample.Rotation += time->Elapsed * 0.25f;
	deferredSceneExample.model1Entity->world.SetRotY(deferredSceneExample.Rotation / 200.0f);
	return false;
}

void ExampleDeferredSceneRender(OPfloat delta) {
	OPrenderClear(0.2f, 0.2f, 0.2f);
	deferredSceneExample.scene.Render(delta);
	OPVISUALDEBUGINFO.Render(delta);

	//OPtexture2DRender(deferredSceneExample.texture);
	OPrenderPresent();
}

OPint ExampleDeferredSceneExit(OPgameState* next) {
	deferredSceneExample.scene.Destroy();
	deferredSceneExample.renderer->Destroy();
	OPfree(deferredSceneExample.renderer);

	return 0;
}

OPint GS_EXAMPLE_DEFERREDSCENE_AVAILABLE = 1;
OPgameState GS_EXAMPLE_DEFERREDSCENE = {
	ExampleDeferredSceneEnter,
	ExampleDeferredSceneUpdate,
	ExampleDeferredSceneRender,
	ExampleDeferredSceneExit
};
