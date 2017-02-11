#include "./Pipeline/include/Renderers/OPrendererDeferredGBufferPass.h"
#include "./Core/include/OPdebug.h"

void OPrendererDeferredGBufferPass::Init(OPcam** cam, OPcam** shadowCam, OPrendererForwardShadowPass* pass) {
	OPrendererPass::Init(cam);
	renderBucket.Init(1000, camera);

	shadowCamera = shadowCam;

	shadowPass = pass;

	materialTextured.Init(OPNEW(OPeffect("Common/GBuffer.vert", "Common/GBuffer.frag")));
	materialSkinned.Init(OPNEW(OPeffect("Common/GBufferSkinned.vert", "Common/GBuffer.frag")));

	materialTextured.AddParam("uViewShadow", &(*shadowCamera)->view);
	materialTextured.AddParam("uProjShadow", &(*shadowCamera)->proj);
	materialTextured.AddParam("uShadow", &shadowPass->depthBuffer.texture, 1);
	materialTextured.AddParam("uLightPos", &(*shadowCamera)->pos);
	//materialTextured.AddParam("uViewPos", &(*cam)->pos);

	materialSkinned.AddParam("uViewShadow", &(*shadowCamera)->view);
	materialSkinned.AddParam("uProjShadow", &(*shadowCamera)->proj);
	materialSkinned.AddParam("uShadow", &shadowPass->depthBuffer.texture, 1);
	materialSkinned.AddParam("uLightPos", &(*shadowCamera)->pos);
	//materialSkinned.AddParam("uViewPos", &(*cam)->pos);

	OPtextureDesc gBufferDesc[3];
	// Position
	gBufferDesc[0] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGBA, OPtextureFormat::RGBA32F, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	// Normal
	gBufferDesc[1] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGBA, OPtextureFormat::RGBA32F, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	// Specular
	gBufferDesc[2] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGBA, OPtextureFormat::RGBA, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::BYTE);
	gBuffer.Init(gBufferDesc, 3);
}

void OPrendererDeferredGBufferPass::Submit(OPrendererEntity* rendererEntity) {
	renderBucket.Submit(rendererEntity->model, &rendererEntity->world, rendererEntity->material, rendererEntity->desc.materialPerMesh);
}

void OPrendererDeferredGBufferPass::Begin() {
}

void OPrendererDeferredGBufferPass::End() {
	TIMED_BLOCK
	gBuffer.Bind();
	OPrenderClear(0, 0, 0);
	renderBucket.Sort();
	renderBucket.Flush(false);
	gBuffer.Unbind();
	OPlogInfo("================ GBUFFER PASS");
}

void OPrendererDeferredGBufferPass::SetCamera(OPcam** cam) {
	OPrendererPass::SetCamera(cam);
	materialTextured.SetParam("uViewPos", &(*cam)->pos);
	materialSkinned.SetParam("uViewPos", &(*cam)->pos);
}

void OPrendererDeferredGBufferPass::SetShadowCamera(OPcam** cam) {
	OPrendererPass::SetCamera(cam);
	materialTextured.SetParam("uViewShadow", &(*cam)->view);
	materialTextured.SetParam("uProjShadow", &(*cam)->proj);
	materialTextured.SetParam("uLightPos", &(*cam)->pos);

	materialSkinned.SetParam("uViewShadow", &(*cam)->view);
	materialSkinned.SetParam("uProjShadow", &(*cam)->proj);
	materialSkinned.SetParam("uLightPos", &(*cam)->pos);
}
