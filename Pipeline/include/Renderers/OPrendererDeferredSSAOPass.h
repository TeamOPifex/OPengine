#pragma once

class OPrendererDeferredSSAOPass;

#include "./Human/include/Rendering/OPrenderer.h"
#include "OPrendererDeferredGBufferPass.h"

#define MAX_SSAO_KERNEL_SIZE 64

class OPrendererDeferredSSAOPass : public OPrendererPass {
public:
	OPrendererDeferredGBufferPass* gbufferPass;
	OPframeBuffer ssaoBuffer;
	OPframeBuffer ssaoBlurBuffer;
	OPvec3 ssaoKernel[MAX_SSAO_KERNEL_SIZE];
	OPtexture* noiseTexture;
	OPmodel* quadMesh;
	OPmaterial defaultSSAOMaterial;
	OPmaterial defaultSSAOBlurMaterial;
	f32 aspectRatio;
	f32 tanHalfFOV;
	i32 kernelSize = MAX_SSAO_KERNEL_SIZE;
	f32 radius = 1.0f;

	OPrendererDeferredSSAOPass() { }
	void Init(OPcam** cam, OPrendererDeferredGBufferPass* pass, OPmodel* mesh);
	void Begin();
	void Submit(OPrendererEntity* rendererEntity);
	void End();
	void Destroy() {}
	OPmaterial* GetMaterial(ui32 materialType) { return &defaultSSAOMaterial; }
};