#pragma once

class OPrendererPBRRenderPass;

#include "./Pipeline/include/Renderers/OPrendererForwardShadowPass.h"
#include "./Pipeline/include/OPmaterialPhong.h"
#include "./Pipeline/include/Materials/OPmaterialPBR.h"
#include "./Pipeline/include/Materials/OPmaterialPBRSkinned.h"
#include "./Human/include/Rendering/OPrenderer.h"

class OPrendererPBRRenderPass : public OPrendererPass {
public:
	OPmaterialPBR texturedMaterial;
	OPmaterialPBRSkinned skinnedMaterial;
	OPeffect skyboxEffect;
	OPmodel* cubeMesh;
	OPtextureCube convoluteCube;
	OPtextureCube envHDRTextureCube;
	OPtextureCube hdrRoughnessFilteredCube;
	OPtexture brdfTexture;
	OPcam** activeCamera = NULL;

	OPvec3 albedo;
	f32 metallic;
	f32 roughness;
	f32 ambientOcclusion;
	OPvec3 lightPostions[4];
	OPvec3 lightColors[4];

	OPrendererPBRRenderPass() { }
	void Init(OPcam** cam, OPcam** shadowCam, OPrendererForwardShadowPass* pass);
	void Begin();
	void Submit(OPrendererEntity* rendererEntity);
	void End();
	void Destroy();
	OPmaterial* GetMaterial(ui32 materialType) { return &texturedMaterial; }
	void SetCamera(OPcam** cam);
	void SetShadowCamera(OPcam** cam);
	void SetEnv(const OPchar* env);
};