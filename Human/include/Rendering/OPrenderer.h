#pragma once

struct OPrenderer;

#include "./Human/include/Rendering/OPcam.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPrenderCommandBucket.h"

struct OPrenderer {
	void* internalPtr;
	OPcam** camera;

	void(*_Init)(OPrenderer* renderer, OPcam** camera, ui32 maxCalls, ui32 maxLights) = 0;
	void(*_SetMaterialEffect)(OPrenderer* renderer, OPeffect* effect, ui32 pass) = 0;
	OPmaterialInstance*(*_CreateMaterialInstance)(OPrenderer* renderer, ui32 pass) = 0;
	void(*_Begin)(OPrenderer* renderer) = 0;
	void(*_Submit)(OPrenderer* renderer, OPmodel* model, OPmaterialInstance* material) = 0;
	//void(*SubmitLightSetup)(const LightSetup& lightSetup) = 0;
	void(*_End)(OPrenderer* renderer) = 0;
	void(*_Present)(OPrenderer* renderer) = 0;

	inline void Init(OPcam** camera, ui32 maxCalls, ui32 maxLights) { _Init(this, camera, maxCalls, maxLights); }
	inline void SetMaterialEffect(OPeffect* effect, ui32 pass) { _SetMaterialEffect(this, effect, pass); }
	inline OPmaterialInstance* CreateMaterialInstance(ui32 pass = 0) { return _CreateMaterialInstance(this, pass); }
	inline void Begin() { _Begin(this); }
	inline void Submit(OPmodel* model, OPmaterialInstance* material) { _Submit(this, model, material); }
	//void(*SubmitLightSetup)(const LightSetup& lightSetup) = 0;
	inline void End() { _End(this); }
	inline void Present() { _Present(this); }
};