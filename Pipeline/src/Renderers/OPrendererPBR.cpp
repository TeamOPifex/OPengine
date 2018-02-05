#include "./Pipeline/include/Renderers/OPrendererPBR.h"
#include "./Core/include/Assert.h"


void OPrendererPBR::Init(OPcam** cam, OPcam** shadowCam) {
	//shadowPass.Init(shadowCam);
	renderPass.Init(cam, shadowCam, &shadowPass);
}

void OPrendererPBR::Destroy() {
	//shadowPass.Destroy();
	renderPass.Destroy();
}

void OPrendererPBR::Submit(OPrendererEntity* rendererEntity) {
	ASSERT(rendererEntity != NULL, "Null rendererEntity?");
	if (rendererEntity->desc.shadowEmitter) {
		//shadowPass.Submit(rendererEntity);
	}
	renderPass.Submit(rendererEntity);
}

OPmaterial* OPrendererPBR::GetMaterial(ui32 pass, ui32 materialType) {
	if (pass == 1) {
		return NULL;// shadowPass.GetMaterial(materialType);
	}
	return renderPass.GetMaterial(materialType);
}


void OPrendererPBR::SetMaterials(OPrendererEntity* rendererEntity) {
	if (rendererEntity->desc.animated) {
		//rendererEntity->shadowMaterial = shadowPass.shadowSkinnedMaterial.CreateInstances(rendererEntity);
		rendererEntity->material = renderPass.skinnedMaterial.CreateInstances(rendererEntity);
	}
	else {
		//rendererEntity->shadowMaterial = shadowPass.shadowMaterial.CreateInstances(rendererEntity);
		rendererEntity->material = renderPass.texturedMaterial.CreateInstances(rendererEntity);
	}
}

void OPrendererPBR::SetEnv(const OPchar* env) {
	renderPass.SetEnv(env);
}


void OPrendererPBR::SetCamera(OPcam** cam) {
	renderPass.SetCamera(cam);
}

void OPrendererPBR::SetShadowCamera(OPcam** cam) {
	//shadowPass.SetCamera(cam);
	renderPass.SetShadowCamera(cam);
}

void OPrendererPBR::Present() {
	OPframeBuffer* active = OPRENDERER_ACTIVE->OPFRAMEBUFFER_ACTIVE;

	//shadowPass.Begin();
	//shadowPass.End();

	active->Bind();
	renderPass.Begin();
	renderPass.End();
}