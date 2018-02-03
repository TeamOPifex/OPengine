#include "./Pipeline/include/Renderers/OPrendererDeferredSSAOPass.h"
#include "./Core/include/OPdebug.h"


void OPrendererDeferredSSAOPass::Init(OPcam** cam, OPrendererDeferredGBufferPass* pass, OPmodel* mesh) {
	OPrendererPass::Init(cam);
	gbufferPass = pass;
	quadMesh = mesh;


	OPtextureDesc ssaoDesc = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB, OPtextureFormat::RED, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	//OPtextureDesc ssaoDesc = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB, OPtextureFormat::RGB32F, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	ssaoBuffer.Init(ssaoDesc);
	//OPtextureDesc ssaoBlurDesc = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB, OPtextureFormat::RED, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	ssaoBlurBuffer.Init(ssaoDesc);


	// Sample Kernel
	for (ui32 i = 0; i < MAX_SSAO_KERNEL_SIZE; i++) {
		OPvec3 sample = OPvec3(
			OPrandom() * 2.0 - 1.0,
			OPrandom() * 2.0 - 1.0,
			OPrandom()
		);

		sample.Norm();
		sample *= OPrandom();

		OPfloat scale = i / 64.0f;
		scale = 0.1 + (scale * scale) * 0.9f; // linear lerp
		sample *= scale;
		ssaoKernel[i] = sample;
	}

	OPvec3 ssaoNoise[16];
	for (ui32 i = 0; i < 16; i++) {
		OPvec3 noise = OPvec3(
			OPrandom() * 2.0 - 1.0,
			OPrandom() * 2.0 - 1.0,
			0
		);

		ssaoNoise[i] = noise;
	}

	OPtextureDesc desc = OPtextureDesc(4, 4, OPtextureFormat::RGB, OPtextureFormat::RGB16F, OPtextureWrap::REPEAT, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	//OPRENDERER_ACTIVE->Texture.Init(&noiseTexture, desc);
	noiseTexture = OPRENDERER_ACTIVE->Texture.Create(desc, &ssaoNoise[0]);

	defaultSSAOMaterial.Init(OPNEW(OPeffect("Common/ssao.vert", "Common/ssao.frag")));
	defaultSSAOMaterial.AddParam("kernelSize", &kernelSize);
	defaultSSAOMaterial.AddParam("radius", &radius);


	aspectRatio = (*camera)->aspect;
	tanHalfFOV = OPtan((*camera)->fov / 2.0f);
	defaultSSAOMaterial.AddParam("uAspectRatio", &aspectRatio);
	defaultSSAOMaterial.AddParam("uTanHalfFOV", &tanHalfFOV);

	defaultSSAOBlurMaterial.Init(OPNEW(OPeffect("Common/ssao.vert", "Common/ssao_blur.frag")));

	defaultSSAOBlurMaterial.AddParam("samples[0]", &ssaoKernel[0]);
	//char buffer[64];
	//for (ui32 i = 0; i < 64; i++) {
	//	sprintf(buffer, "samples[%d]", i);
	//	OPRENDERER_ACTIVE->Effect.AddUniform(defaultSSAOMaterial.effect, buffer);
	//}
}

void OPrendererDeferredSSAOPass::Submit(OPrendererEntity* rendererEntity) {

}

void OPrendererDeferredSSAOPass::Begin() {

}

void OPrendererDeferredSSAOPass::End() {
	TIMED_BLOCK

	aspectRatio = (*camera)->aspect;
	tanHalfFOV = OPtan((*camera)->fov / 2.0f);

	// SSAO pass
	ssaoBuffer.Bind();
	OPrenderCullMode(OPcullFace::FRONT);
	OPrenderDepth(false);
	OPrenderDepthWrite(false);
	OPrenderClear(0, 0, 0, 1);
	defaultSSAOMaterial.Bind();

	//OPeffectSet("uGbufferPosition", &gbufferPass->gBuffer.texture[0], 0);
	OPeffectSet("uGbufferNormal", &gbufferPass->gBuffer.texture[1], 1);
	OPeffectSet("uNoise", noiseTexture, 2);
	OPeffectSet("uGbufferDepth", &gbufferPass->gBuffer.depthTexture, 3);


	//OPmat4 invProjView;
	//OPmat4 projView = (*camera)->proj * (*camera)->view;
	//OPmat4Inverse(&invProjView, projView);
	//OPeffectSet("uInvProjView", &invProjView);
	//OPeffectSet("uView", &(*camera)->view);

	//char buffer[64];
	//for (ui32 i = 0; i < 64; i++) {
	//	sprintf(buffer, "samples[%d]", i);
	//	OPeffectSet(buffer, &ssaoKernel[i]);
	//}

	OPcam* cam = *(gbufferPass->camera);
	OPRENDERER_ACTIVE->ShaderUniform.SetMat4(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform("uProj"), &cam->proj);
	OPrenderDepth(0);
	OPrenderCull(false);
	quadMesh->Bind();
	OPrenderDrawBufferIndexed(0);
	ssaoBuffer.Unbind();


	// Blur SSAO pass
	ssaoBlurBuffer.Bind();
	OPrenderCullMode(OPcullFace::FRONT);
	OPrenderDepth(false);
	OPrenderDepthWrite(false);
	OPrenderClear(0, 0, 0, 1);
	defaultSSAOBlurMaterial.Bind();

	OPeffectSet("uSSAOBuffer", ssaoBuffer.texture, 0);

	OPrenderDepth(0);
	OPrenderCull(false);
	quadMesh->Bind();
	OPrenderDrawBufferIndexed(0);
	ssaoBlurBuffer.Unbind();
	OPlogInfo("================ SSAO PASS");
}
