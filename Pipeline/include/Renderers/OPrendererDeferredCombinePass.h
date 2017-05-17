#pragma once

class OPrendererDeferredCombinePass;

#include "./Human/include/Rendering/OPrenderer.h"
#include "OPrendererDeferredGBufferPass.h"
#include "OPrendererDeferredSSAOPass.h"
#include "OPrendererDeferredLightPass.h"

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
	OPmaterial* GetMaterial(ui32 materialType) { return defaultCombineMaterial; }
};