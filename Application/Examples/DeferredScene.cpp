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
	OPrendererDeferred2 renderer2;
	OPmodel* model;
	OPrendererEntity* model1Entity;
	OPcamFreeFlight camera;
	OPcam camera2;
	OPtexture2DOLD* texture0;
	OPtexture2DOLD* texture1;
	OPtexture2DOLD* texture2;
	OPtexture2DOLD* texture3;
	OPtexture2DOLD* texture4;
	OPtexture2DOLD* texture5;
	OPtexture2DOLD* texture6;
	OPeffect DepthTextureEffect;
	ui32 state;
} DeferredSceneExample;

DeferredSceneExample deferredSceneExample;

OPsceneLight* light, *light2, *light3;

void ExampleDeferredSceneEnter(OPgameState* last) {

	deferredSceneExample.renderer = OPNEW(OPrendererDeferred());
	deferredSceneExample.scene.Init(&deferredSceneExample.renderer2, 100, 100);
	deferredSceneExample.camera.Init(1.0, 1.0, OPvec3(0, 5, 0), 0.1f, 50.0f);
	deferredSceneExample.camera2.SetPerspective(OPvec3(0, 1, 5), OPvec3(0, 1, 0));
	deferredSceneExample.scene.camera = &deferredSceneExample.camera.Camera;
	deferredSceneExample.renderer2.SetCamera(&deferredSceneExample.scene.camera);
	//deferredSceneExample.scene.camera = &deferredSceneExample.camera2;
	//deferredSceneExample.renderer2.SetCamera(&deferredSceneExample.scene.camera);

	light = deferredSceneExample.scene.Add(OPlightSpot());
	light->light.position = OPvec3(0, 0, 0);
	light->light.color = OPvec3(1, 0, 0);
	light->light.radius = 5.0f;

	light2 = deferredSceneExample.scene.Add(OPlightSpot());
	light2->light.position = OPvec3(0, 0, 0);
	light2->light.color = OPvec3(0, 0, 1);
	light2->light.radius = 5.0f;

	light3 = deferredSceneExample.scene.Add(OPlightSpot());
	light3->light.position = OPvec3(5, 0, -5);
	light3->light.color = OPvec3(0, 1, 0);
	light3->light.radius = 5.0f;

	deferredSceneExample.model = (OPmodel*)OPCMAN.LoadGet("sponza.opm");

	deferredSceneExample.model1Entity = deferredSceneExample.scene.Add(deferredSceneExample.model, true);
	deferredSceneExample.model1Entity->material = deferredSceneExample.scene.renderer->GetMaterial(0);
	deferredSceneExample.model1Entity->material->CreateInstances(deferredSceneExample.model1Entity);

	//deferredSceneExample.model1Entity->material[0]->AddParam("uAlbedoMap", (OPtexture*)OPCMAN.LoadGet("Dagger_Albedo.png"), 0);
	//deferredSceneExample.model1Entity->material[0].AddParam("uSpecularMap", (OPtexture*)OPCMAN.LoadGet("Dagger_Albedo.png"), 1);

	deferredSceneExample.DepthTextureEffect.Init("Common/Texture2D.vert", "Common/TextureDepth.frag");

	deferredSceneExample.texture0 = OPtexture2DCreate(&deferredSceneExample.renderer2.gBuffer.texture[0]);
	deferredSceneExample.texture1 = OPtexture2DCreate(&deferredSceneExample.renderer2.gBuffer.texture[1]);
	deferredSceneExample.texture2 = OPtexture2DCreate(&deferredSceneExample.renderer2.gBuffer.texture[2]);
	deferredSceneExample.texture3 = OPtexture2DCreate(&deferredSceneExample.renderer2.gBuffer.depthTexture, &deferredSceneExample.DepthTextureEffect);
	deferredSceneExample.texture4 = OPtexture2DCreate(deferredSceneExample.renderer2.lightBuffer.texture);
	deferredSceneExample.texture5 = OPtexture2DCreate(deferredSceneExample.renderer2.ssaoBuffer.texture);
	deferredSceneExample.texture6 = OPtexture2DCreate(deferredSceneExample.renderer2.ssaoBlurBuffer.texture);

	deferredSceneExample.state = 0;
}

bool dir = true;

OPint ExampleDeferredSceneUpdate(OPtimer* time) {
	deferredSceneExample.camera.Update(time);
	if (dir) {
		light->light.position.y += time->Elapsed * 0.01f;
	}
	else {
		light->light.position.y -= time->Elapsed * 0.01f;
	}

	if (light->light.position.y > 10) dir = false;
	if (light->light.position.y < 0) dir = true;

	light3->light.position = OPvec3(OPsin(time->TotalGametime * 0.001f) * 5.0, 2.5f, OPcos(time->TotalGametime * 0.001f) * 5.0);

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
	else if (deferredSceneExample.state == 6) {
		OPtexture2DRender(deferredSceneExample.texture5);
	}
	else if (deferredSceneExample.state == 7) {
		OPtexture2DRender(deferredSceneExample.texture6);
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
	if (ImGui::Button("SSAO")) {
		deferredSceneExample.state = 6;
	}
	if (ImGui::Button("SSAO Blur")) {
		deferredSceneExample.state = 7;
	}
	if (ImGui::Button("Use SSAO")) {
		deferredSceneExample.renderer2.useSSAO = !deferredSceneExample.renderer2.useSSAO;
	}
	ImGui::InputFloat("SSAO Radius", &deferredSceneExample.renderer2.radius, 0.1, 1.0);
	ImGui::ColorEdit3("Light Color", (float*)&light->light.color);
	ImGui::SliderFloat("Radius", &light->light.radius, 0, 10);
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
