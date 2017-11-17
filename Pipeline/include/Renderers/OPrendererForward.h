#pragma once

class OPrendererForward;


#include "./Human/include/Rendering/OPrenderer.h"
#include "./Human/include/Rendering/OPrenderCommandBucket.h"
#include "./Pipeline/include/Renderers/OPrendererForwardRenderPass.h"
#include "./Pipeline/include/Renderers/OPrendererForwardShadowPass.h"

class OPrendererForward : public OPrenderer2 {
public:
	OPrendererForwardRenderPass renderPass;
	OPrendererForwardShadowPass shadowPass;




	OPrendererForward() { }
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


	//OPrendererFullForward() { Setup(); }

	//OPrendererFullForward* Setup();
	//static OPrendererFullForward* Create();
	//void Destroy();

	//void _Init(OPcam** camera, ui32 maxCalls, ui32 maxLights);

	//inline void Init(OPcam** camera, ui32 maxCalls, ui32 maxLights) { rendererRoot.Init(camera, maxCalls, maxLights); }
	//inline void SetMaterial(OPmaterial* material, ui32 pass) { rendererRoot.SetMaterial(material, pass); }
	//inline void SetMaterialEffect(OPeffect* effect, ui32 pass) { rendererRoot.SetMaterialEffect(effect, pass); }
	//inline OPmaterial* CreateMaterial(ui32 pass = 0) { return rendererRoot.CreateMaterial(pass); }
	//inline void Begin() { rendererRoot.Begin(); }
	//inline void Submit(OPmodel* model, OPmat4* world, bool shadowed, OPmaterial* material) { rendererRoot.Submit(model, world, shadowed, material); }
	//inline void End() { rendererRoot.End(); }
	//inline void Present() { rendererRoot.Present(); }
	//inline OPrenderer* GetBase() { return &rendererRoot; }
};