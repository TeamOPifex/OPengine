#pragma once

class OPrendererForwardRenderPass;

#include "./Pipeline/include/Renderers/OPrendererForwardShadowPass.h"
#include "./Pipeline/include/OPmaterialPhong.h"
#include "./Pipeline/include/Materials/OPmaterialSkinned.h"
#include "./Human/include/Rendering/OPrenderer.h"

class OPrendererForwardRenderPass : public OPrendererPass {
public:
	OPmaterialPhong texturedMaterial;
	OPmaterialSkinned skinnedMaterial;

	OPrendererForwardRenderPass() { }
	void Init(OPcam** cam, OPcam** shadowCam, OPrendererForwardShadowPass* pass);
	void Begin();
	void Submit(OPrendererEntity* rendererEntity);
	void End();
	void Destroy();
	OPmaterial* GetMaterial(ui32 materialType) { return &texturedMaterial; }
	void SetCamera(OPcam** cam);
	void SetShadowCamera(OPcam** cam);
};