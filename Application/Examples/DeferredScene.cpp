#include "./ExampleSelectorState.h"

// OPifex Engine includes
#include "./Pipeline/include/Rendering.h"
#include "./Pipeline/include/OPmaterialPBR.h"
#include "./Human/include/Systems/OPinputSystem.h"
#include "./Human/include/Systems/OPrenderSystem.h"
#include "./Data/include/OPcman.h"

#ifdef ADDON_imgui
#include "OPimgui.h"
#endif

typedef struct {
	OPfloat Rotation;
	OPscene scene;
	OPrendererDeferred* renderer;
	OPmodel* model;
	OPsceneEntity* model1Entity;
	OPcamFreeFlight camera;
	OPtexture2DOLD* texture0;
	OPtexture2DOLD* texture1;
	OPtexture2DOLD* texture2;
	OPtexture2DOLD* texture3;
	OPtexture2DOLD* texture4;
	ui32 state;
} DeferredSceneExample;

DeferredSceneExample deferredSceneExample;

void ExampleDeferredSceneEnter(OPgameState* last) {

	deferredSceneExample.renderer = OPNEW(OPrendererDeferred());
	deferredSceneExample.scene.Init(&deferredSceneExample.renderer->rendererRoot, 100, 100);
	deferredSceneExample.camera.Init();
	deferredSceneExample.scene.camera = &deferredSceneExample.camera.Camera;

	OPsceneLight* light = deferredSceneExample.scene.Add(OPlightSpot());
	light->light.radius = 5.0f;
	light->world.SetIdentity()->Scl(5.0f);

	deferredSceneExample.model = (OPmodel*)OPCMAN.LoadGet("sponza.opm");

	deferredSceneExample.model1Entity = deferredSceneExample.scene.Add(deferredSceneExample.model);
	deferredSceneExample.model1Entity->material = deferredSceneExample.scene.renderer->GetMaterial(0)->CreateInstances(deferredSceneExample.model);

	//deferredSceneExample.model1Entity->material[0]->AddParam("uAlbedoMap", (OPtexture*)OPCMAN.LoadGet("Dagger_Albedo.png"), 0);
	deferredSceneExample.model1Entity->material[0]->AddParam("uSpecularMap", (OPtexture*)OPCMAN.LoadGet("Dagger_Albedo.png"), 1);

	deferredSceneExample.texture0 = OPtexture2DCreate(&deferredSceneExample.renderer->gBuffer.texture[0]);
	deferredSceneExample.texture1 = OPtexture2DCreate(&deferredSceneExample.renderer->gBuffer.texture[1]);
	deferredSceneExample.texture2 = OPtexture2DCreate(&deferredSceneExample.renderer->gBuffer.texture[2]);
	deferredSceneExample.texture3 = OPtexture2DCreate(&deferredSceneExample.renderer->gBuffer.depthTexture);
	deferredSceneExample.texture4 = OPtexture2DCreate(deferredSceneExample.renderer->lightBuffer.texture);

	deferredSceneExample.state = 0;
}

OPint ExampleDeferredSceneUpdate(OPtimer* time) {
	deferredSceneExample.camera.Update(time);
	//deferredSceneExample.Rotation += time->Elapsed * 0.25f;
	deferredSceneExample.model1Entity->world.SetScl(1.0f);// .SetRotY(deferredSceneExample.Rotation / 200.0f);
	return false;
}

void ExampleDeferredSceneRender(OPfloat delta) {
	OPrenderClear(0.2f, 0.2f, 0.2f);

	deferredSceneExample.scene.Render(delta);

	if (deferredSceneExample.state == 1) {
		OPtexture2DRender(deferredSceneExample.texture0);
	}
	else if (deferredSceneExample.state == 2) {
		OPtexture2DRender(deferredSceneExample.texture1);
	}
	else if (deferredSceneExample.state == 3) {
		OPtexture2DRender(deferredSceneExample.texture2);
	}
	else if (deferredSceneExample.state == 4) {
		OPtexture2DRender(deferredSceneExample.texture3);
	}
	else if (deferredSceneExample.state == 5) {
		OPtexture2DRender(deferredSceneExample.texture4);
	}

#ifdef ADDON_imgui
	OPVISUALDEBUGINFO.Begin();
	OPVISUALDEBUGINFO.DrawWindows(delta);

	ImGui::SetNextWindowPos(ImVec2(10, 10));
	bool stayOpen = true;

	ImGui::Begin("Deferred Mode", &stayOpen, ImVec2(0, 0), 0.3f);
	if (ImGui::Button("Deferred")) {
		deferredSceneExample.state = 0;
	}
	if (ImGui::Button("Position")) {
		deferredSceneExample.state = 1;
	}
	if (ImGui::Button("Normal")) {
		deferredSceneExample.state = 2;
	}
	if (ImGui::Button("Specular")) {
		deferredSceneExample.state = 3;
	}
	if (ImGui::Button("Depth")) {
		deferredSceneExample.state = 4;
	}
	if (ImGui::Button("Light")) {
		deferredSceneExample.state = 5;
	}
	ImGui::End();

	ImGui::Render();
#endif

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
