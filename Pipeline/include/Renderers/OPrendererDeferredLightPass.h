#pragma once

class OPrendererDeferredLightPass;

#include "./Human/include/Rendering/OPrenderer.h"
#include "OPrendererDeferredGBufferPass.h"

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
	OPmaterial* GetMaterial(ui32 materialType) { return defaultLightingMaterial; }
};