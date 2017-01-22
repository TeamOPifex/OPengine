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

class DeferredSceneExample : public OPgameState {
	OPfloat Rotation;
	OPscene scene;
	OPrendererDeferred renderer;
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
	OPtexture2DOLD* texture7;
	OPeffect DepthTextureEffect;
	ui32 state;


	OPsceneLight* light, *light2, *light3;
	bool dir = true;

	void Init(OPgameState* last) {

		scene.Init(&renderer, 100, 100);
		camera.Init(1.0, 1.0, OPvec3(0, 5, 0), 0.1f, 50.0f);
		camera2.SetPerspective(OPvec3(0, 1, 5), OPvec3(0, 1, 0));
		scene.camera = &camera.Camera;
		renderer.SetCamera(&scene.camera);
		//scene.camera = &camera2;
		//renderer2.SetCamera(&scene.camera);

		light = scene.Add(OPlightSpot());
		light->light.position = OPvec3(0, 0, 0);
		light->light.color = OPvec3(1, 0, 0);
		light->light.radius = 5.0f;

		light2 = scene.Add(OPlightSpot());
		light2->light.position = OPvec3(0, 0, 0);
		light2->light.color = OPvec3(0, 0, 1);
		light2->light.radius = 5.0f;

		light3 = scene.Add(OPlightSpot());
		light3->light.position = OPvec3(5, 0, -5);
		light3->light.color = OPvec3(0, 1, 0);
		light3->light.radius = 5.0f;

		model = (OPmodel*)OPCMAN.LoadGet("sponza.opm");

		model1Entity = scene.Add(model, OPrendererEntityDesc(true, true));
		//model1Entity->material = scene.renderer->GetMaterial(0);
		//model1Entity->material->CreateInstances(model1Entity);

		//model1Entity->material[0]->AddParam("uAlbedoMap", (OPtexture*)OPCMAN.LoadGet("Dagger_Albedo.png"), 0);
		//model1Entity->material[0].AddParam("uSpecularMap", (OPtexture*)OPCMAN.LoadGet("Dagger_Albedo.png"), 1);

		DepthTextureEffect.Init("Common/Texture2D.vert", "Common/TextureDepth.frag");

		texture0 = OPtexture2DCreate(&renderer.GetGBuffer()->texture[0]);
		texture1 = OPtexture2DCreate(&renderer.GetGBuffer()->texture[1]);
		texture2 = OPtexture2DCreate(&renderer.GetGBuffer()->texture[2]);
		texture3 = OPtexture2DCreate(&renderer.GetGBuffer()->depthTexture, &DepthTextureEffect);
		texture4 = OPtexture2DCreate(renderer.lightPass.lightBuffer.texture);
		texture5 = OPtexture2DCreate(renderer.ssaoPass.ssaoBuffer.texture);
		texture6 = OPtexture2DCreate(renderer.ssaoPass.ssaoBlurBuffer.texture);
		texture7 = OPtexture2DCreate(&renderer.shadowPass.depthBuffer.texture);

		state = 0;
	}


	OPint Update(OPtimer* time) {
		camera.Update(time);
		if (dir) {
			light->light.position.y += time->Elapsed * 0.01f;
		}
		else {
			light->light.position.y -= time->Elapsed * 0.01f;
		}

		if (light->light.position.y > 10) dir = false;
		if (light->light.position.y < 0) dir = true;

		light3->light.position = OPvec3(OPsin(time->TotalGametime * 0.001f) * 5.0, 2.5f, OPcos(time->TotalGametime * 0.001f) * 5.0);

		//Rotation += time->Elapsed * 0.25f;
		model1Entity->world.SetScl(1.0f);// .SetRotY(Rotation / 200.0f);
		return false;
	}

	void Render(OPfloat delta) {
		OPrenderClear(0.2f, 0.2f, 0.2f);

		scene.Render(delta);

		if (state == 1) {
			OPtexture2DRender(texture0);
		}
		else if (state == 2) {
			OPtexture2DRender(texture1);
		}
		else if (state == 3) {
			OPtexture2DRender(texture2);
		}
		else if (state == 4) {
			OPtexture2DRender(texture3);
		}
		else if (state == 5) {
			OPtexture2DRender(texture4);
		}
		else if (state == 6) {
			OPtexture2DRender(texture5);
		}
		else if (state == 7) {
			OPtexture2DRender(texture6);
		}
		else if (state == 8) {
			OPtexture2DRender(texture7);
		}

#ifdef ADDON_imgui
		OPVISUALDEBUGINFO.Begin();
		OPVISUALDEBUGINFO.DrawWindows(delta);

		ImGui::SetNextWindowPos(ImVec2(10, 10));
		bool stayOpen = true;

		ImGui::Begin("Deferred Mode", &stayOpen, ImVec2(0, 0), 0.3f);
		if (ImGui::Button("Deferred")) {
			state = 0;
		}
		if (ImGui::Button("Position")) {
			state = 1;
		}
		if (ImGui::Button("Normal")) {
			state = 2;
		}
		if (ImGui::Button("Specular")) {
			state = 3;
		}
		if (ImGui::Button("Depth")) {
			state = 4;
		}
		if (ImGui::Button("Light")) {
			state = 5;
		}
		if (ImGui::Button("SSAO")) {
			state = 6;
		}
		if (ImGui::Button("SSAO Blur")) {
			state = 7;
		}
		if (ImGui::Button("Shadow")) {
			state = 8;
		}
		if (ImGui::Button("Use SSAO")) {
			renderer.combinePass.useSSAO = !renderer.combinePass.useSSAO;
		}
		ImGui::InputFloat("SSAO Radius", &renderer.ssaoPass.radius, 0.1, 1.0);
		ImGui::ColorEdit3("Light Color", (float*)&light->light.color);
		ImGui::SliderFloat("Radius", &light->light.radius, 0, 10);
		ImGui::End();

		ImGui::Render();
#endif

		//OPtexture2DRender(texture);
		OPrenderPresent();
	}

	OPint Exit(OPgameState* next) {
		scene.Destroy();

		return 0;
	}

};

OPint GS_EXAMPLE_DEFERREDSCENE_AVAILABLE = 1;
DeferredSceneExample _GS_EXAMPLE_DEFERREDSCENE;
OPgameState* GS_EXAMPLE_DEFERREDSCENE = &_GS_EXAMPLE_DEFERREDSCENE;