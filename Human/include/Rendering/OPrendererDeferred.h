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
	OPeffect* defaultGBufferEffect = NULL;
	OPmaterial* defaultGBufferMaterial = NULL;
	OPeffect* defaultLightingEffect = NULL;
	OPmaterial* defaultLightingMaterial = NULL;
	OPmaterialInstance* defaultLightingMaterialInstance = NULL;
	OPeffect* defaultLightingSpotEffect = NULL;
	OPmaterial* defaultLightingSpotMaterial = NULL;
	OPmaterialInstance* defaultLightingSpotMaterialInstance = NULL;
	OPmodel* sphereMesh = NULL;
	OPmodel* quadMesh = NULL;

	OPrendererDeferred() { Setup(); }

	OPrendererDeferred* Setup();

	static OPrendererDeferred* Create();

	inline void Init(OPcam** camera, ui32 maxCalls, ui32 maxLights) { rendererRoot.Init(camera, maxCalls, maxLights); }
	inline void SetMaterial(OPmaterial* material, ui32 pass) { rendererRoot.SetMaterial(material, pass); }
	inline void SetMaterialEffect(OPeffect* effect, ui32 pass) { rendererRoot.SetMaterialEffect(effect, pass); }
	inline OPmaterialInstance* CreateMaterialInstance(ui32 pass = 0) { return rendererRoot.CreateMaterialInstance(pass); }
	inline void Begin() { rendererRoot.Begin(); }
	inline void Submit(OPmodel* model, OPmat4* world, OPmaterialInstance* material) { rendererRoot.Submit(model, world, material); }
	inline void End() { rendererRoot.End(); }
	inline void Present() { rendererRoot.Present(); }
	void Destroy();
};
