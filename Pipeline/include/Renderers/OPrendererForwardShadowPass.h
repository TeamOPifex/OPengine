#pragma once

class OPrendererForwardShadowPass;

#include "./Human/include/Rendering/OPrenderer.h"
#include "./Pipeline/include/Renderers/OPrendererForwardShadowPass.h"
#include "./Human/include/Rendering/OPframeBufferDepth.h"

class OPrendererForwardShadowPass : public OPrendererPass {
public:
	OPframeBufferDepth depthBuffer;
	OPmaterial shadowMaterial;
	OPmaterial shadowSkinnedMaterial;

	OPrendererForwardShadowPass() { }
	void Init(OPcam** cam);
	void Begin();
	void Submit(OPrendererEntity* rendererEntity);
	void End();
	void Destroy();
	OPmaterial* GetMaterial(ui32 materialType) { return &shadowMaterial; }
};