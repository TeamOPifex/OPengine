#pragma once

struct OPrendererDeferred;
typedef struct OPrendererDeferred OPrendererDeferred;

#include "./Human/include/Rendering/OPrenderer.h"
#include "./Human/include/Rendering/OPrenderCommandBucket.h"

struct OPrendererDeferred {
	OPrenderer rendererRoot;
	OPmaterial* passes[2];
	OPrenderCommandBucket renderBucket[2];
	OPframeBuffer gBuffer;
	OPframeBuffer lightBuffer;
	OPeffect* defaultGBufferEffect = NULL;
	OPmaterial* defaultGBufferMaterial = NULL;
	OPeffect* defaultLightingEffect = NULL;
	OPmaterial* defaultLightingMaterial = NULL;
	OPmaterial* defaultLightingMaterialInstance = NULL;
	OPeffect* defaultLightingSpotEffect = NULL;
	OPmaterial* defaultLightingSpotMaterial = NULL;
	OPmaterial* defaultLightingSpotMaterialInstance = NULL;
	OPeffect* defaultCombineEffect = NULL;
	OPmaterial* defaultCombineMaterial = NULL;
	OPmaterial* defaultCombineMaterialInstance = NULL;
	OPmodel* sphereMesh = NULL;
	OPmodel* quadMesh = NULL;
	OPmat4 invertViewProjection;
	OPlightSpot* spotLights;
	ui32 spotLightsInd;

	OPrendererDeferred() { Setup(); }

	OPrendererDeferred* Setup();

	static OPrendererDeferred* Create();

	inline void Init(OPcam** camera, ui32 maxCalls, ui32 maxLights) { rendererRoot.Init(camera, maxCalls, maxLights); }
	inline void SetMaterial(OPmaterial* material, ui32 pass) { rendererRoot.SetMaterial(material, pass); }
	inline void SetMaterialEffect(OPeffect* effect, ui32 pass) { rendererRoot.SetMaterialEffect(effect, pass); }
	inline OPmaterial* CreateMaterial(ui32 pass = 0) { return rendererRoot.CreateMaterial(pass); }
	inline void Begin() { rendererRoot.Begin(); }
	inline void Submit(OPmodel* model, OPmat4* world, bool shadowed, OPmaterial* material) { rendererRoot.Submit(model, world, shadowed, material); }
	inline void End() { rendererRoot.End(); }
	inline void Present() { rendererRoot.Present(); }
	void Destroy();
};

class OPrendererDeferred2 : public OPrenderer2 {
public:
	OPframeBuffer gBuffer;
	OPframeBuffer lightBuffer;
	OPframeBuffer ssaoBuffer;
	OPframeBuffer ssaoBlurBuffer;
	OPvec3 ssaoKernel[64];
	OPtexture* noiseTexture;
	OPmaterial* defaultGBufferMaterial = NULL;
	OPmaterial* defaultLightingMaterial = NULL;
	OPmaterial* defaultLightingSpotMaterial = NULL;
	OPmaterial* defaultCombineMaterial = NULL;
	OPmaterial* defaultSSAOMaterial = NULL;
	OPmaterial* defaultSSAOBlurMaterial = NULL;
	OPmodel* sphereMesh = NULL;
	OPmodel* quadMesh = NULL;
	OPmat4 invertViewProjection;
	OPlightSpot* spotLights;
	ui32 spotLightsInd;


	void Init();
	void Begin();
	void Submit(OPrendererEntity* rendererEntity);
	OPmaterial* GetMaterial(ui32 pass = 0);
	void End();
	void Present();
	void Destroy();
	void SetCamera(OPcam** camera);
	void SetCamera(OPcam** camera, ui32 pass);
};