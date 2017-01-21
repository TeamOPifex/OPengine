#include "./Pipeline/include/Renderers/OPrendererDeferred.h"
#include "./Human/include/Rendering/Primitives/OPsphere.h"
#include "./Human/include/Rendering/Primitives/OPquad.h"

#include "./Math/include/OPtween.h"

void OPrendererDeferred::Init(OPcam** cam, OPcam** shadowCam) {

	quadMesh = OPquadCreate(1.0f, 1.0f, OPvec2(0, 1), OPvec2(1, 0));

	gbufferPass.Init(cam);
	ssaoPass.Init(cam, &gbufferPass, quadMesh);
	lightPass.Init(cam, &gbufferPass, quadMesh);
	combinePass.Init(cam, &gbufferPass, &ssaoPass, &lightPass, quadMesh);

	passes = OPNEW(OPrendererPass*, 4);
	passes[0] = &gbufferPass;
	passes[1] = &ssaoPass;
	passes[2] = &lightPass;
	passes[3] = &combinePass;

	sphereMesh = (OPmodel*)OPCMAN.LoadGet("uvsphere20.opm");	
}

void OPrendererDeferred::Begin() {
	//gBuffer.Bind();
	//OPrenderClear(0, 0, 0);
}

void OPrendererDeferred::Submit(OPrendererEntity* rendererEntity) {
	//passes[0].renderBucket.Submit(rendererEntity->model, &rendererEntity->world, rendererEntity->material, rendererEntity->desc.materialPerMesh);
	passes[0]->Submit(rendererEntity);
	//passes[1]->Submit(rendererEntity);
	//passes[2]->Submit(rendererEntity);
	//passes[3]->Submit(rendererEntity);
}

OPmaterial* OPrendererDeferred::GetMaterial(ui32 pass, ui32 materialType) {
	return passes[pass]->GetMaterial(materialType);
}

void OPrendererDeferred::End() {
	// DRAW SCENE
	{
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

void OPrendererDeferred::SetCamera(OPcam** camera) {
	SetCamera(camera, 0);
	//SetCamera(camera, 1);
}

void OPrendererDeferred::SetCamera(OPcam** camera, ui32 pass) {
	//this->camera = camera;
	passes[pass]->SetCamera(camera);
}

void OPrendererDeferredGBufferPass::Init(OPcam** cam) {
	OPrendererPass::Init(cam);
	renderBucket.Init(1000, camera);

	materials = OPNEW(OPmaterial[2]);
	// Non Animated version
	materials[0].Init(OPNEW(OPeffect("Common/GBuffer.vert", "Common/GBuffer.frag")));
	// Animated version
	materials[1].Init(OPNEW(OPeffect("Common/GBuffer.vert", "Common/GBuffer.frag")));

	OPtextureDesc gBufferDesc[3];
	// Position
	gBufferDesc[0] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGBA, OPtextureFormat::RGBA32F, OPtextureWrap::CLAMP_TO_EDGE, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	// Normal
	gBufferDesc[1] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB, OPtextureFormat::RGB32F, OPtextureWrap::CLAMP_TO_EDGE, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	// Specular
	gBufferDesc[2] = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB, OPtextureFormat::RGBA, OPtextureWrap::CLAMP_TO_EDGE, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	gBuffer.Init(gBufferDesc, 3);
}

void OPrendererDeferredGBufferPass::Submit(OPrendererEntity* rendererEntity) {
	renderBucket.Submit(rendererEntity->model, &rendererEntity->world, rendererEntity->material, rendererEntity->desc.materialPerMesh);
}

void OPrendererDeferredGBufferPass::Begin() {
	gBuffer.Bind();
	OPrenderClear(0, 0, 0);
}

void OPrendererDeferredGBufferPass::End() {
	renderBucket.Sort();
	renderBucket.Flush(false);
	gBuffer.Unbind();
}


void OPrendererDeferredSSAOPass::Init(OPcam** cam, OPrendererDeferredGBufferPass* pass, OPmodel* mesh) {
	OPrendererPass::Init(cam);
	gbufferPass = pass;
	quadMesh = mesh;

	materials = OPNEW(OPmaterial[2]);
	// Non Animated version
	materials[0].Init(OPNEW(OPeffect("Common/GBuffer.vert", "Common/GBuffer.frag")));
	// Animated version
	materials[1].Init(OPNEW(OPeffect("Common/GBuffer.vert", "Common/GBuffer.frag")));


	OPtextureDesc ssaoDesc = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB, OPtextureFormat::RED, OPtextureWrap::CLAMP_TO_EDGE, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	//OPtextureDesc ssaoDesc = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB, OPtextureFormat::RGB32F, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	ssaoBuffer.Init(ssaoDesc);
	//OPtextureDesc ssaoBlurDesc = OPtextureDesc(OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Width, OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height, OPtextureFormat::RGB, OPtextureFormat::RED, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT);
	ssaoBlurBuffer.Init(ssaoDesc);


	// Sample Kernel
	for (ui32 i = 0; i < 64; i++) {
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

	defaultSSAOMaterial = OPNEW(OPmaterial(OPNEW(OPeffect("Common/ssao.vert", "Common/ssao.frag"))));
	defaultSSAOMaterial->AddParam("radius", &radius);

	defaultSSAOBlurMaterial = OPNEW(OPmaterial(OPNEW(OPeffect("Common/ssao.vert", "Common/ssao_blur.frag"))));

	char buffer[64];
	for (ui32 i = 0; i < 64; i++) {
		sprintf(buffer, "samples[%d]", i);
		OPRENDERER_ACTIVE->Effect.AddUniform(defaultSSAOMaterial->effect, buffer);
	}
}

void OPrendererDeferredSSAOPass::Submit(OPrendererEntity* rendererEntity) {

}

void OPrendererDeferredSSAOPass::Begin() {

}

void OPrendererDeferredSSAOPass::End() {
	// SSAO pass
	ssaoBuffer.Bind();
	OPrenderCullMode(OPcullFace::FRONT);
	OPrenderDepth(false);
	OPrenderDepthWrite(false);
	OPrenderClear(0, 0, 0, 1);
	defaultSSAOMaterial->Bind();

	OPeffectSet("uGbufferPosition", &gbufferPass->gBuffer.texture[0], 0);
	OPeffectSet("uGbufferNormal", &gbufferPass->gBuffer.texture[1], 1);
	OPeffectSet("uNoise", noiseTexture, 2);
	OPeffectSet("uGbufferDepth", &gbufferPass->gBuffer.depthTexture, 3);
	char buffer[64];
	for (ui32 i = 0; i < 64; i++) {
		sprintf(buffer, "samples[%d]", i);
		OPeffectSet(buffer, &ssaoKernel[i]);
	}

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
	defaultSSAOBlurMaterial->Bind();

	OPeffectSet("uSSAOBuffer", ssaoBuffer.texture, 0);

	OPrenderDepth(0);
	OPrenderCull(false);
	quadMesh->Bind();
	OPrenderDrawBufferIndexed(0);
	ssaoBlurBuffer.Unbind();
}



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

	// DRAW LIGHTS
	{
		lightBuffer.Bind();
		f32 ambient = 0.0;
		OPrenderClearColor(ambient, ambient, ambient, 1.0);



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

	}
}




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
	OPrenderCullMode(OPcullFace::FRONT);
	OPrenderDepth(false);
	OPrenderDepthWrite(false);

	defaultCombineMaterial->Bind();
	quadMesh->Bind();
	OPrenderDepth(0);
	OPrenderCull(false);

	OPmat4 world = OPMAT4_IDENTITY;

	//OPeffectSet("uGbufferPosition", &gBuffer.texture[0], 0);
	//OPeffectSet("uGbufferNormal", &gBuffer.texture[1], 1);
	OPeffectSet("uGbufferAlbedoSpec", &gbufferPass->gBuffer.texture[2], 2);
	OPeffectSet("uLightBuffer", lightPass->lightBuffer.texture, 3);
	OPeffectSet("uSSAOBuffer", ssaoPass->ssaoBlurBuffer.texture, 3);
	OPeffectSet("uWorld", 1, &world);

	OPrenderDrawBufferIndexed(0);

	OPrenderCullMode(OPcullFace::FRONT);
	OPrenderDepth(true);
	OPrenderDepthWrite(true);
}