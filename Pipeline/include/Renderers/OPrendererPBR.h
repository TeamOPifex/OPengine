#pragma once

class OPrendererPBR;


#include "./Human/include/Rendering/OPrenderer.h"
#include "./Human/include/Rendering/OPrenderCommandBucket.h"
#include "./Pipeline/include/Renderers/OPrendererPBRRenderPass.h"
#include "./Pipeline/include/Renderers/OPrendererForwardShadowPass.h"

class OPrendererPBR : public OPrenderer2 {
public:
	OPrendererPBRRenderPass renderPass;
	OPrendererForwardShadowPass shadowPass;
	
	OPrendererPBR() { }
	void Init(OPcam** cam, OPcam** shadowCam);
	void Begin() {}
	void SetMaterials(OPrendererEntity* rendererEntity);
	void Submit(OPrendererEntity* rendererEntity);
	OPmaterial* GetMaterial(ui32 pass = 0, ui32 materialType = (ui32)OPmaterialType::DEFAULT);
	void End() {}
	void Present();
	void Destroy();
	void SetCamera(OPcam** camera);
	void SetShadowCamera(OPcam** camera);
	void SetEnv(const OPchar* env);

};