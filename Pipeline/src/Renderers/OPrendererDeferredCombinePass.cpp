#include "./Pipeline/include/Renderers/OPrendererDeferredCombinePass.h"
#include "./Core/include/OPdebug.h"


void OPrendererDeferredCombinePass::Init(OPcam** cam, OPrendererDeferredGBufferPass* pass, OPrendererDeferredSSAOPass* pass2, OPrendererDeferredLightPass* pass3, OPmodel* mesh) {
	OPrendererPass::Init(cam);
	gbufferPass = pass;
	ssaoPass = pass2;
	lightPass = pass3;
	quadMesh = mesh;

	defaultCombineMaterial = OPNEW(OPmaterial(OPNEW(OPeffect("Common/DeferredCombine.vert", "Common/DeferredCombine.frag"))));
	defaultCombineMaterial->AddParam("uUseSSAO", &useSSAO);
	defaultCombineMaterial->depth = false;
	defaultCombineMaterial->cull = false;

	//defaultCombineMaterial->AddParam("uGbufferPosition", &gBuffer.texture[0], 0);
	//defaultCombineMaterial->AddParam("uGbufferNormal", &gBuffer.texture[1], 1);
	//defaultCombineMaterial->AddParam("uGbufferAlbedoSpec", &gBuffer.texture[2], 2);
	//defaultCombineMaterial->AddParam("uLightBuffer", lightBuffer.texture, 3);
}

void OPrendererDeferredCombinePass::Submit(OPrendererEntity* rendererEntity) {

}

void OPrendererDeferredCombinePass::Begin() {

}

void OPrendererDeferredCombinePass::End() {
	TIMED_BLOCK
	OPrenderCullMode(OPcullFace::FRONT);
	OPrenderDepth(false);
	OPrenderDepthWrite(false);

	defaultCombineMaterial->Bind();
	quadMesh->Bind();
	OPrenderDepth(0);
	OPrenderCull(false);

	OPmat4 world = OPMAT4_IDENTITY;

	OPeffectSet("uGbufferPosition", &gbufferPass->gBuffer.texture[0], 0);
	OPeffectSet("uGbufferNormal", &gbufferPass->gBuffer.texture[1], 1);
	OPeffectSet("uGbufferAlbedoSpec", &gbufferPass->gBuffer.texture[2], 2);
	//OPeffectSet("uLightBuffer", lightPass->lightBuffer.texture, 3);
	OPeffectSet("uSSAOBuffer", ssaoPass->ssaoBlurBuffer.texture, 4);
	OPeffectSet("uLightPos", &(*camera)->pos);
	OPeffectSet("uWorld", 1, &world);

	OPrenderDrawBufferIndexed(0);

	OPrenderCullMode(OPcullFace::FRONT);
	OPrenderDepth(true);
	OPrenderDepthWrite(true);
	OPlogInfo("================ COMBINE PASS");
}