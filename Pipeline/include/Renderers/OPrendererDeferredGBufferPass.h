#pragma once

class OPrendererDeferredGBufferPass;

#include "./Human/include/Rendering/OPrenderer.h"
#include "OPrendererForwardShadowPass.h"

class OPrendererDeferredGBufferPass : public OPrendererPass {
public:
	OPrendererForwardShadowPass* shadowPass;
	OPframeBuffer gBuffer;
	OPmaterial materialTextured;
	OPmaterial materialSkinned;

	OPrendererDeferredGBufferPass() { }
	void Init(OPcam** cam, OPcam** shadowCam, OPrendererForwardShadowPass* pass);
	void Begin();
	void Submit(OPrendererEntity* rendererEntity);
	void End();
	void Destroy() {}
	void SetCamera(OPcam** cam); 
	void SetShadowCamera(OPcam** cam);
	OPmaterial* GetMaterial(ui32 materialType) { return &materialTextured; }
};