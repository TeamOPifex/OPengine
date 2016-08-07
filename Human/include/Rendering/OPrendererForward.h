#pragma once

struct OPrendererForward;

#include "./Human/include/Rendering/OPrenderer.h"
#include "./Human/include/Rendering/OPrenderCommandBucket.h"

struct OPrendererForward {
	OPrenderer rendererRoot;
	OPmaterial* passes[1];
	OPrenderCommandBucket renderBucket[1];

	OPrendererForward() { Setup(); }

	OPrendererForward* Setup();

	static OPrendererForward* Create();

	inline void Init(OPcam** camera, ui32 maxCalls, ui32 maxLights) { rendererRoot.Init(camera, maxCalls, maxLights); }
	inline void SetMaterial(OPmaterial* material, ui32 pass) { rendererRoot.SetMaterial(material, pass); }
	inline void SetMaterialEffect(OPeffect* effect, ui32 pass) { rendererRoot.SetMaterialEffect(effect, pass); }
	inline OPmaterialInstance* CreateMaterialInstance(ui32 pass = 0) { return rendererRoot.CreateMaterialInstance(pass); }
	inline void Begin() { rendererRoot.Begin(); }
	inline void Submit(OPmodel* model, OPmaterialInstance* material) { rendererRoot.Submit(model, material); }
	inline void End() { rendererRoot.End(); }
	inline void Present() { rendererRoot.Present(); }
};
