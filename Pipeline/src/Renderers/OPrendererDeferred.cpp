#include "./Pipeline/include/Renderers/OPrendererDeferred.h"
#include "./Human/include/Rendering/Primitives/OPsphere.h"
#include "./Human/include/Rendering/Primitives/OPquad.h"
#include "./Core/include/OPdebug.h"

#include "./Math/include/OPtween.h"

#ifdef ADDON_imgui
#include "OPimgui.h"
#endif

void OPrendererDeferred::Init(OPcam** cam, OPcam** shadowCam) {

	quadMesh = OPquadCreate(1.0f, 1.0f, OPvec2(0, 0), OPvec2(1, 1));

	shadowPass.Init(shadowCam);
	gbufferPass.Init(cam, shadowCam, &shadowPass);
	ssaoPass.Init(cam, &gbufferPass, quadMesh);
	lightPass.Init(cam, &gbufferPass, quadMesh);
	combinePass.Init(cam, &gbufferPass, &ssaoPass, &lightPass, quadMesh);

	passes = OPALLOC(OPrendererPass*, 4);
	passes[0] = &gbufferPass;
	passes[1] = &ssaoPass;
	passes[2] = &lightPass;
	passes[3] = &combinePass;

	//sphereMesh = (OPmodel*)OPCMAN.LoadGet("uvsphere20.opm");	
}

void OPrendererDeferred::Begin() {
	//gBuffer.Bind();
	//OPrenderClear(0, 0, 0);
}

void OPrendererDeferred::Submit(OPrendererEntity* rendererEntity) {
	if (rendererEntity->desc.shadowEmitter) {
		shadowPass.Submit(rendererEntity);
	}
	gbufferPass.Submit(rendererEntity);
}

void OPrendererDeferred::SetMaterials(OPrendererEntity* rendererEntity) {
	if (rendererEntity->desc.animated) {
		rendererEntity->material = gbufferPass.materialSkinned.CreateInstances(rendererEntity);
		rendererEntity->shadowMaterial = shadowPass.shadowSkinnedMaterial.CreateInstances(rendererEntity);
	}
	else {
		rendererEntity->material = gbufferPass.materialTextured.CreateInstances(rendererEntity);
		rendererEntity->shadowMaterial = shadowPass.shadowMaterial.CreateInstances(rendererEntity);
	}
}

OPmaterial* OPrendererDeferred::GetMaterial(ui32 pass, ui32 materialType) {
	return passes[pass]->GetMaterial(materialType);
}

void OPrendererDeferred::End() {
	TIMED_BLOCK

	// DRAW SCENE
	{
		//
		{ 
			TIMED_BLOCK
			shadowPass.Begin();
			shadowPass.End();
		}

		OPrenderBlend(false);

		// Geometry pass
		{
			TIMED_BLOCK
			gbufferPass.Begin();
			gbufferPass.End();
		}
		
		// SSAO
		if (combinePass.useSSAO) {
			TIMED_BLOCK
			ssaoPass.Begin();
			ssaoPass.End();
		}
	}

	{
		TIMED_BLOCK
		lightPass.Begin();
		lightPass.End();
	}

	{
		TIMED_BLOCK
		combinePass.Begin();
		combinePass.End();
	}
	//OPlogInfo("================ FULL DEFERRED PASS");
}

void OPrendererDeferred::RenderDebug() {
#ifdef _DEBUG

#ifdef ADDON_imgui
	if (ImGui::Button("Use SSAO")) {
		combinePass.useSSAO = !combinePass.useSSAO;
	}
	ImGui::SliderInt("SSAO Kernel", &ssaoPass.kernelSize, 2, 64);
	ImGui::InputFloat("SSAO Radius", &ssaoPass.radius, 0.1, 1.0);
#endif

#endif
}

void OPrendererDeferred::Present() {
}

void OPrendererDeferred::Destroy() {

}

void OPrendererDeferred::SetCamera(OPcam** cam) {
	gbufferPass.SetCamera(cam);
	ssaoPass.SetCamera(cam);
}

void OPrendererDeferred::SetShadowCamera(OPcam** cam) {
	gbufferPass.SetShadowCamera(cam);
	shadowPass.SetCamera(cam);
}