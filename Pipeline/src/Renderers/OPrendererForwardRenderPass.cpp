#include "./Pipeline/include/Renderers/OPrendererForwardRenderPass.h"
#include "./Pipeline/include/Renderers/OPrendererForwardShadowPass.h"

void OPrendererForwardRenderPass::Init(OPcam** cam, OPcam** shadowCam, OPrendererForwardShadowPass* pass) {
	OPrendererPass::Init(cam);	

	shadowCamera = shadowCam;
	
	renderBucket.Init(1000, cam);

	texturedMaterial.Init(OPNEW(OPeffect("Common/TexturedShadow.vert", "Common/TexturedShadow.frag")));
	skinnedMaterial.Init(OPNEW(OPeffect("Common/SkinnedShadow.vert", "Common/TexturedShadow.frag")));

	texturedMaterial.AddParam("uViewShadow", &(*shadowCam)->view);
	texturedMaterial.AddParam("uProjShadow", &(*shadowCam)->proj);
	texturedMaterial.AddParam("uShadow", &pass->depthBuffer.texture, 1);
	texturedMaterial.AddParam("uLightPos", &(*shadowCam)->pos);
	texturedMaterial.AddParam("uViewPos", &(*cam)->pos);

	skinnedMaterial.AddParam("uViewShadow", &(*shadowCam)->view);
	skinnedMaterial.AddParam("uProjShadow", &(*shadowCam)->proj);
	skinnedMaterial.AddParam("uShadow", &pass->depthBuffer.texture, 1);
	skinnedMaterial.AddParam("uLightPos", &(*shadowCam)->pos);
	skinnedMaterial.AddParam("uViewPos", &(*cam)->pos);
}

void OPrendererForwardRenderPass::Submit(OPrendererEntity* rendererEntity) {
	renderBucket.Submit(rendererEntity->model, &rendererEntity->world, rendererEntity->material, rendererEntity->desc.materialPerMesh);
}

void OPrendererForwardRenderPass::Begin() {

}

void OPrendererForwardRenderPass::End() {
	renderBucket.Sort();
	renderBucket.Flush(false);
}

void OPrendererForwardRenderPass::SetCamera(OPcam** cam) {
	OPrendererPass::SetCamera(cam);
	texturedMaterial.SetParam("uViewPos", &(*cam)->pos);
	skinnedMaterial.SetParam("uViewPos", &(*cam)->pos);
}

void OPrendererForwardRenderPass::SetShadowCamera(OPcam** cam) {
	shadowCamera = cam;
	texturedMaterial.SetParam("uViewShadow", &(*cam)->view);
	texturedMaterial.SetParam("uProjShadow", &(*cam)->proj);
	texturedMaterial.SetParam("uLightPos", &(*cam)->pos);

	skinnedMaterial.SetParam("uViewShadow", &(*cam)->view);
	skinnedMaterial.SetParam("uProjShadow", &(*cam)->proj);
	skinnedMaterial.SetParam("uLightPos", &(*cam)->pos);
}