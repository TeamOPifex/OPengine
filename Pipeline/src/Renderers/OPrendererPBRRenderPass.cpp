#include "./Pipeline/include/Renderers/OPrendererPBRRenderPass.h"
#include "./Pipeline/include/Renderers/OPrendererForwardShadowPass.h"
#include "./Human/include/Rendering/Primitives/OPcube.h"

void OPrendererPBRRenderPass::Init(OPcam** cam, OPcam** shadowCam, OPrendererForwardShadowPass* pass) {
	OPrendererPass::Init(cam);

	activeCamera = cam;
	shadowCamera = shadowCam;

	renderBucket.Init(1000, cam);

	SetEnv("newport_loft.hdr");
	OPtexture::GenerateBRDF(&brdfTexture, 512);


	skyboxEffect.Init("skybox.vert", "skybox.frag");
	cubeMesh = OPcubeCreate();


	texturedMaterial.Init();
	skinnedMaterial.Init(OPNEW(OPeffect("Common/SkinnedShadow.vert", "Common/TexturedShadow.frag")));

	albedo = OPvec3(0.5f);
	metallic = 0.0f;
	roughness = 0.5f;
	ambientOcclusion = 1.0f;
	f32 dist = 10;
	lightPostions[0] = OPvec3(dist);
	lightPostions[1] = OPvec3(-dist, dist, dist);
	lightPostions[2] = OPvec3(-dist, dist, -dist);
	lightPostions[3] = OPvec3(dist, dist, -dist);
	lightColors[0] = OPvec3(300.0f);
	lightColors[1] = OPvec3(300.0f);
	lightColors[2] = OPvec3(300.0f);
	lightColors[3] = OPvec3(300.0f);

	//texturedMaterial.AddParam("uAlbedo", &albedo);
	//texturedMaterial.AddParam("uMetallic", &metallic);
	//texturedMaterial.AddParam("uRoughness", &roughness);
	//texturedMaterial.AddParam("uAmbientOcclusion", &ambientOcclusion);
	texturedMaterial.AddParam("uLightPositions[0]", &lightPostions);
	texturedMaterial.AddParam("uLightColors[0]", &lightColors);
	texturedMaterial.AddParam("uCamPos", &(*cam)->pos);
	texturedMaterial.AddParam("uIrradianceMap", &convoluteCube);
	texturedMaterial.AddParam("uPrefilterMap", &hdrRoughnessFilteredCube);
	texturedMaterial.AddParam("uBRDFLUT", &brdfTexture);

	//texturedMaterial.AddParam("uViewShadow", &(*shadowCam)->view);
	//texturedMaterial.AddParam("uProjShadow", &(*shadowCam)->proj);
	//texturedMaterial.AddParam("uShadow", &pass->depthBuffer.texture, 0);
	//texturedMaterial.AddParam("uLightPos", &(*shadowCam)->pos);
	//texturedMaterial.AddParam("uViewPos", &(*cam)->pos);

	skinnedMaterial.AddParam("uViewShadow", &(*shadowCam)->view);
	skinnedMaterial.AddParam("uProjShadow", &(*shadowCam)->proj);
	//skinnedMaterial.AddParam("uShadow", &pass->depthBuffer.texture, 0);
	skinnedMaterial.AddParam("uLightPos", &(*shadowCam)->pos);
	skinnedMaterial.AddParam("uViewPos", &(*cam)->pos);




}

void OPrendererPBRRenderPass::SetEnv(const OPchar* env) {
	// TODO: (garrett) destroy previous texture versions

	OPtexture* envHDRTexture = OPtexture::Load(env);
	OPtextureCube::FromEquirectangular(&envHDRTextureCube, envHDRTexture, 512);
	OPtextureCube::Convolute(&convoluteCube, &envHDRTextureCube, 32);
	OPtextureCube::RoughnessMap(&hdrRoughnessFilteredCube, &envHDRTextureCube, 128);
}

void OPrendererPBRRenderPass::Destroy() {
	texturedMaterial.effect->Destroy();
	OPfree(texturedMaterial.effect);
	texturedMaterial.Destroy();

	skinnedMaterial.effect->Destroy();
	OPfree(skinnedMaterial.effect);
	skinnedMaterial.Destroy();

	renderBucket.Destroy();
}

void OPrendererPBRRenderPass::Submit(OPrendererEntity* rendererEntity) {
	renderBucket.Submit(rendererEntity->model, &rendererEntity->world, rendererEntity->material, rendererEntity->desc.materialPerMesh);
}

void OPrendererPBRRenderPass::Begin() {

}

void OPrendererPBRRenderPass::End() {
	renderBucket.Sort();
	renderBucket.Flush(false);

	// Render Skybox
	OPRENDERER_ACTIVE->SetDepthFunc(OPdepthFunction::LEQUAL);
	skyboxEffect.Bind();
	skyboxEffect.Set("uEnvironmentMap", &envHDRTextureCube, 0);
	skyboxEffect.Set("uProj", &(*activeCamera)->proj);
	skyboxEffect.Set("uView", &(*activeCamera)->view);
	cubeMesh->Bind();
	OPrenderDrawBufferIndexed(0);
	OPRENDERER_ACTIVE->SetDepthFunc(OPdepthFunction::LESS);
}

void OPrendererPBRRenderPass::SetCamera(OPcam** cam) {
	OPrendererPass::SetCamera(cam);
	activeCamera = cam;
	texturedMaterial.SetParam("uCamPos", &(*cam)->pos);
	skinnedMaterial.SetParam("uViewPos", &(*cam)->pos);
}

void OPrendererPBRRenderPass::SetShadowCamera(OPcam** cam) {
	shadowCamera = cam;
	texturedMaterial.SetParam("uViewShadow", &(*cam)->view);
	texturedMaterial.SetParam("uProjShadow", &(*cam)->proj);
	texturedMaterial.SetParam("uLightPos", &(*cam)->pos);

	skinnedMaterial.SetParam("uViewShadow", &(*cam)->view);
	skinnedMaterial.SetParam("uProjShadow", &(*cam)->proj);
	skinnedMaterial.SetParam("uLightPos", &(*cam)->pos);
}