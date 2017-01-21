#pragma once

struct OPrendererDeferred;
typedef struct OPrendererDeferred OPrendererDeferred;

#include "./Human/include/Rendering/OPrenderer.h"
#include "./Human/include/Rendering/OPrenderCommandBucket.h"

class OPrendererDeferredGBufferPass : public OPrendererPass {
public:
	OPframeBuffer gBuffer;

	OPrendererDeferredGBufferPass() { }
	void Init(OPcam** cam);
	void Begin();
	void Submit(OPrendererEntity* rendererEntity);
	void End();
	void Destroy() {}
	OPmaterial* GetMaterial(ui32 materialType) { return materials; }
}; 

class OPrendererDeferredSSAOPass : public OPrendererPass {
public:
	OPrendererDeferredGBufferPass* gbufferPass;
	OPframeBuffer ssaoBuffer;
	OPframeBuffer ssaoBlurBuffer;
	OPvec3 ssaoKernel[64];
	OPtexture* noiseTexture; 
	OPmodel* quadMesh;
	OPmaterial* defaultSSAOMaterial = NULL;
	OPmaterial* defaultSSAOBlurMaterial = NULL;
	f32 radius = 1.0f;

	OPrendererDeferredSSAOPass() { }
	void Init(OPcam** cam, OPrendererDeferredGBufferPass* pass, OPmodel* mesh);
	void Begin();
	void Submit(OPrendererEntity* rendererEntity);
	void End();
	void Destroy() {}
	OPmaterial* GetMaterial(ui32 materialType) { return materials; }
};

class OPrendererDeferredLightPass : public OPrendererPass {
public:
	OPframeBuffer lightBuffer;
	OPmaterial* defaultLightingMaterial = NULL;
	OPmaterial* defaultLightingSpotMaterial = NULL;
	OPmodel* quadMesh = NULL;

	OPmat4 invertViewProjection;
	OPlightSpot* spotLights;
	ui32 spotLightsInd;

	OPrendererDeferredLightPass() { }
	void Init(OPcam** cam, OPrendererDeferredGBufferPass* pass, OPmodel* mesh);
	void Begin();
	void Submit(OPrendererEntity* rendererEntity);
	void End();
	void Destroy() {}
	OPmaterial* GetMaterial(ui32 materialType) { return materials; }
};

class OPrendererDeferredCombinePass : public OPrendererPass {
public:
	OPrendererDeferredGBufferPass* gbufferPass;
	OPrendererDeferredSSAOPass* ssaoPass;
	OPrendererDeferredLightPass* lightPass;
	OPmodel* quadMesh;
	OPmaterial* defaultCombineMaterial = NULL;
	bool useSSAO = true;

	OPrendererDeferredCombinePass() { }
	void Init(OPcam** cam,
		OPrendererDeferredGBufferPass* pass, 
		OPrendererDeferredSSAOPass* pass2, 
		OPrendererDeferredLightPass* pass3,
		OPmodel* mesh);
	void Begin();
	void Submit(OPrendererEntity* rendererEntity);
	void End();
	void Destroy() {}
	OPmaterial* GetMaterial(ui32 materialType) { return materials; }
};

class OPrendererDeferred : public OPrenderer2 {
public:
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
	OPmaterial* GetMaterial(ui32 pass = 0, ui32 materialType = (ui32)OPmaterialType::DEFAULT);
	void End();
	void Present();
	void Destroy();
	void SetCamera(OPcam** camera);
	void SetCamera(OPcam** camera, ui32 pass);

	OPframeBuffer* GetGBuffer() {
		return &gbufferPass.gBuffer;
	}
};