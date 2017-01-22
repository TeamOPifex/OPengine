#pragma once

class OPrendererDeferredSSAOPass;

#include "./Human/include/Rendering/OPrenderer.h"
#include "OPrendererDeferredGBufferPass.h"

class OPrendererDeferredSSAOPass : public OPrendererPass {
public:
	OPrendererDeferredGBufferPass* gbufferPass;
	OPframeBuffer ssaoBuffer;
	OPframeBuffer ssaoBlurBuffer;
	OPvec3 ssaoKernel[64];
	OPtexture* noiseTexture;
	OPmodel* quadMesh;
	OPmaterial defaultSSAOMaterial;
	OPmaterial defaultSSAOBlurMaterial;
	f32 radius = 1.0f;

	OPrendererDeferredSSAOPass() { }
	void Init(OPcam** cam, OPrendererDeferredGBufferPass* pass, OPmodel* mesh);
	void Begin();
	void Submit(OPrendererEntity* rendererEntity);
	void End();
	void Destroy() {}
	OPmaterial* GetMaterial(ui32 materialType) { return &defaultSSAOMaterial; }
};