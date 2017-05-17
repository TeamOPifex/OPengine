#include "./Pipeline/include/Renderers/OPrendererDeferredLightPass.h"
#include "./Core/include/OPdebug.h"



void OPrendererDeferredLightPass::Init(OPcam** cam, OPrendererDeferredGBufferPass* pass, OPmodel* mesh) {
	OPrendererPass::Init(cam);
	quadMesh = mesh;

	defaultLightingMaterial = OPNEW(OPmaterial(OPNEW(OPeffect("Common/DeferredLighting.vert", "Common/DeferredLighting.frag"))));
	defaultLightingMaterial->depth = false;
	defaultLightingMaterial->cull = false;

	OPtextureDesc lightBufferDec = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGBA, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::BYTE);
	lightBuffer.Init(lightBufferDec, &pass->gBuffer.depthTexture);

}

void OPrendererDeferredLightPass::Submit(OPrendererEntity* rendererEntity) {

}

void OPrendererDeferredLightPass::Begin() {

}

void OPrendererDeferredLightPass::End() {
	TIMED_BLOCK

	// DRAW LIGHTS
	
	lightBuffer.Bind();
	f32 ambient = 1.0;
	OPrenderClear(ambient, ambient, ambient, 1.0);



	//// DRAW LIGHTS
	//{
	//	deferredRenderer->invertViewProjection = (*deferredRenderer->rendererRoot.camera)->view * (*deferredRenderer->rendererRoot.camera)->proj;
	//	OPmat4Inverse(&deferredRenderer->invertViewProjection, deferredRenderer->invertViewProjection);

	//	deferredRenderer->lightBuffer.Bind();
	//	f32 ambient = 0.25;
	//	OPrenderClearColor(ambient, ambient, ambient, 1.0);

	//	OPrenderDepthWrite(false);
	//	OPrenderDepth(false);
	//	OPrenderBlend(true);
	//	OPRENDERER_ACTIVE->SetBlendModeAdditive();
	//	//OPrenderCullMode(2);

	//	deferredRenderer->defaultLightingSpotEffect->Bind();
	//	deferredRenderer->sphereMesh->Bind();
	//	OPmat4 world;

	//	//OPrenderSetWireframe(wireframe);
	//	OPrenderCull(true);
	//	OPrenderCullMode(OPcullFace::BACK);
	//	for (ui32 i = 0; i < deferredRenderer->spotLightsInd; i++) {
	//		world.SetScl(deferredRenderer->spotLights[i].radius)->Translate(deferredRenderer->spotLights[i].position);
	//		OPeffectSet("uWorld", 1, &world);
	//		OPeffectSet("uLightPos", &deferredRenderer->spotLights[i].position);
	//		OPeffectSet("uLightColor", &deferredRenderer->spotLights[i].color);
	//		OPeffectSet("uLightRadius", deferredRenderer->spotLights[i].radius);
	//		OPeffectSet(*renderer->camera);
	//		OPeffectSet("uGbufferDepth", &deferredRenderer->gBuffer.depthTexture, 0);
	//		OPeffectSet("uGbufferPosition", &deferredRenderer->gBuffer.texture[0], 1);
	//		OPrenderDrawBufferIndexed(0);
	//	}
	//	OPRENDERER_ACTIVE->SetBlendModeAlpha();
	//	//OPrenderSetWireframe(false);

	//	//OPrenderBlend(true);
	//	//deferredRenderer->renderBucket[1].Submit(deferredRenderer->quadMesh, &deferredWorld, deferredRenderer->defaultLightingMaterialInstance);
	//	//deferredRenderer->renderBucket[1].Sort();
	//	//deferredRenderer->renderBucket[1].Flush(false);

	//	deferredRenderer->lightBuffer.Unbind();

	//	OPrenderCullMode(OPcullFace::FRONT);
	//	OPrenderDepth(true);
	//	OPrenderDepthWrite(true);

	//	deferredRenderer->spotLightsInd = 0;
	//}

	lightBuffer.Unbind();
	OPlogInfo("================ LIGHT PASS");

	
}


