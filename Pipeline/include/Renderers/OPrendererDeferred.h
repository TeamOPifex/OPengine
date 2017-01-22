#pragma once

class OPrendererDeferred;

#include "./Human/include/Rendering/OPrenderer.h"
#include "OPrendererDeferredCombinePass.h"
#include "OPrendererForwardShadowPass.h"

class OPrendererDeferred : public OPrenderer2 {
public:
	OPrendererForwardShadowPass shadowPass;
	OPrendererDeferredGBufferPass gbufferPass;
	OPrendererDeferredSSAOPass ssaoPass;
	OPrendererDeferredLightPass lightPass;
	OPrendererDeferredCombinePass combinePass; 

	OPmodel* sphereMesh = NULL;
	OPmodel* quadMesh = NULL;

	OPrendererDeferred() { }
	void Init(OPcam** cam, OPcam** shadowCam);
	void Begin();
	void Submit(OPrendererEntity* rendererEntity);
	void SetMaterials(OPrendererEntity* rendererEntity);
	OPmaterial* GetMaterial(ui32 pass = 0, ui32 materialType = (ui32)OPmaterialType::DEFAULT);
	void End();
	void Present();
	void Destroy();
	void SetCamera(OPcam** camera);
	void SetShadowCamera(OPcam** camera);

	OPframeBuffer* GetGBuffer() {
		return &gbufferPass.gBuffer;
	}
};