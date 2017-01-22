//#pragma once
//
//struct OPrendererForward;
//typedef struct OPrendererForward OPrendererForward;
//
//#include "./Human/include/Rendering/OPrenderer.h"
//#include "./Human/include/Rendering/OPrenderCommandBucket.h"
//#include "./Human/include/Rendering/OPframeBufferDepth.h"
//
//enum struct OPrendererForwardPass {
//	SHADOW = 0,
//	MODEL = 1,
//	ANIMATED_MODEL = 2
//};
//
//// 3 Pass renderer
//// Shadow Pass (optional)
//// Render Pass
//// Post Processing Pass (optional)
//
//struct OPrendererForward {
//	OPrenderer rendererRoot;
//	OPmaterial* passes[4];
//	OPrenderCommandBucket renderBucket[2];
//	OPeffect* defaultShadowEffect = NULL;
//	OPmaterial* defaultShadowMaterial = NULL;
//	OPmaterial* defaultShadowMaterialInstance = NULL;
//	OPeffect* defaultShadowAnimatedEffect = NULL;
//	OPmaterial* defaultShadowAnimatedMaterial = NULL;
//	OPmaterial* defaultShadowAnimatedMaterialInstance = NULL;
//	OPeffect* defaultEffect = NULL;
//	OPmaterial* defaultMaterial = NULL;
//	OPeffect* defaultAnimatedEffect = NULL;
//	OPmaterial* defaultAnimatedMaterial = NULL;
//	OPframeBufferDepth depthBuffer;
//	OPcam shadowCamera;
//
//	OPrendererForward() { Setup(); }
//
//	OPrendererForward* Setup();
//	static OPrendererForward* Create();
//	void Destroy();
//
//	inline void Init(OPcam** camera, ui32 maxCalls, ui32 maxLights) { rendererRoot.Init(camera, maxCalls, maxLights); }
//	inline void SetMaterial(OPmaterial* material, ui32 pass) { rendererRoot.SetMaterial(material, pass); }
//	inline void SetMaterialEffect(OPeffect* effect, ui32 pass) { rendererRoot.SetMaterialEffect(effect, pass); }
//	inline OPmaterial* CreateMaterialInstance(ui32 pass = 0) { return rendererRoot.CreateMaterial(pass); }
//	inline void Begin() { rendererRoot.Begin(); }
//	inline void Submit(OPmodel* model, OPmat4* world, bool shadowed, OPmaterial* material) { rendererRoot.Submit(model, world, shadowed, material); }
//	inline void End() { rendererRoot.End(); }
//	inline void Present() { rendererRoot.Present(); }
//	inline OPrenderer* GetBase() { return &rendererRoot; }
//};
