#include "./Pipeline/include/Renderers/OPrendererDeferred.h"
#include "./Human/include/Rendering/Primitives/OPsphere.h"
#include "./Human/include/Rendering/Primitives/OPquad.h"

#include "./Math/include/OPtween.h"

void OPrendererDeferred::Init(OPcam** cam, OPcam** shadowCam) {

	quadMesh = OPquadCreate(1.0f, 1.0f, OPvec2(0, 1), OPvec2(1, 0));

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
	shadowPass.Submit(rendererEntity); 
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
	// DRAW SCENE
	{

		shadowPass.Begin();
		shadowPass.End();

		OPrenderBlend(false);

		// Geometry pass
		gbufferPass.Begin();
		gbufferPass.End();

		if (combinePass.useSSAO) {
			ssaoPass.Begin();
			ssaoPass.End();
		}
	}

	lightPass.Begin();
	lightPass.End();
}

void OPrendererDeferred::Present() {
	combinePass.Begin();
	combinePass.End();
}

void OPrendererDeferred::Destroy() {

}

void OPrendererDeferred::SetCamera(OPcam** cam) {
	gbufferPass.SetCamera(cam);
}

void OPrendererDeferred::SetShadowCamera(OPcam** cam) {
	gbufferPass.SetShadowCamera(cam);
	shadowPass.SetCamera(cam);
}