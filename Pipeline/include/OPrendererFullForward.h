#pragma once

struct OPrendererFullForward;
typedef struct OPrendererFullForward OPrendererFullForward;


#include "./Pipeline/include/OPmaterialPhong.h"
#include "./Pipeline/include/Materials/OPmaterialSkinned.h"
#include "./Human/include/Rendering/OPrenderer.h"
#include "./Human/include/Rendering/OPrenderCommandBucket.h"
#include "./Human/include/Rendering/OPframeBufferDepth.h"

struct OPrendererFullForward {
	OPrenderer rendererRoot;

	OPrenderCommandBucket renderDepthBucket;
	OPrenderCommandBucket renderBucket;

	OPmaterial shadowMaterial;
	OPmaterial shadowSkinnedMaterial;
	OPmaterialPhong texturedMaterial;
	OPmaterialSkinned skinnedMaterial;

	OPframeBufferDepth depthBuffer;
	OPcam shadowCamera;
	OPcam* shadowCameraPtr;

	OPrendererFullForward() { Setup(); }

	OPrendererFullForward* Setup();
	static OPrendererFullForward* Create();
	void Destroy();

	void _Init(OPcam** camera, ui32 maxCalls, ui32 maxLights);

	inline void Init(OPcam** camera, ui32 maxCalls, ui32 maxLights) { rendererRoot.Init(camera, maxCalls, maxLights); }
	inline void SetMaterial(OPmaterial* material, ui32 pass) { rendererRoot.SetMaterial(material, pass); }
	inline void SetMaterialEffect(OPeffect* effect, ui32 pass) { rendererRoot.SetMaterialEffect(effect, pass); }
	inline OPmaterial* CreateMaterial(ui32 pass = 0) { return rendererRoot.CreateMaterial(pass); }
	inline void Begin() { rendererRoot.Begin(); }
	inline void Submit(OPmodel* model, OPmat4* world, bool shadowed, OPmaterial* material) { rendererRoot.Submit(model, world, shadowed, material); }
	inline void End() { rendererRoot.End(); }
	inline void Present() { rendererRoot.Present(); }
	inline OPrenderer* GetBase() { return &rendererRoot; }
};