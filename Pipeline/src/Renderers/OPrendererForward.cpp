#include "./Pipeline/include/Renderers/OPrendererForward.h"
#include "./Core/include/Assert.h"


void OPrendererForward::Init(OPcam** cam, OPcam** shadowCam) {
	shadowPass.Init(shadowCam);
	renderPass.Init(cam, shadowCam, &shadowPass);
}

void OPrendererForward::Destroy() {
	shadowPass.Destroy();
	renderPass.Destroy();
}

void OPrendererForward::Submit(OPrendererEntity* rendererEntity) {
	ASSERT(rendererEntity != NULL, "Null rendererEntity?");
	if (rendererEntity->desc.shadowEmitter) {
		shadowPass.Submit(rendererEntity);
	}
	renderPass.Submit(rendererEntity);
}

OPmaterial* OPrendererForward::GetMaterial(ui32 pass, ui32 materialType) {
	if (pass == 1) {
		return shadowPass.GetMaterial(materialType);
	}
	return renderPass.GetMaterial(materialType);
}


void OPrendererForward::SetMaterials(OPrendererEntity* rendererEntity) {
	if (rendererEntity->desc.animated) {
		rendererEntity->shadowMaterial = shadowPass.shadowSkinnedMaterial.CreateInstances(rendererEntity);
		rendererEntity->material = renderPass.skinnedMaterial.CreateInstances(rendererEntity);
	}
	else {
		rendererEntity->shadowMaterial = shadowPass.shadowMaterial.CreateInstances(rendererEntity);
		rendererEntity->material = renderPass.texturedMaterial.CreateInstances(rendererEntity);
	}
}

void OPrendererForward::SetCamera(OPcam** cam) {
	renderPass.SetCamera(cam);
}

void OPrendererForward::SetShadowCamera(OPcam** cam) {
	shadowPass.SetCamera(cam);
	renderPass.SetShadowCamera(cam);
}

void OPrendererForward::Present() {
	OPframeBuffer* active = OPRENDERER_ACTIVE->OPFRAMEBUFFER_ACTIVE;

	shadowPass.Begin();
	shadowPass.End();

	active->Bind();
	renderPass.Begin();
	renderPass.End();
}