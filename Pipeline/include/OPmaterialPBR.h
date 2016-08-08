#pragma once

struct OPmaterialPBR;
struct OPmaterialPBRInstance;

#include "./Human/include/Rendering/OPmaterial.h"

struct OPmaterialPBR {
	OPmaterial rootMaterial;
	OPvec4 lightColor;
	OPvec3 lightDirection;
	OPfloat lightIntensity;

	OPmaterialPBR() { }
	OPmaterialPBR(OPeffect* effect) {
		Init(effect);
		rootMaterial.AddParam("uPreintegratedFG", (OPtexture*)OPcmanLoadGet("PreintegratedFG.png"), 6);
	}

	void Init(OPeffect* effect);
	OPmaterialPBRInstance* CreateInstance();
};

struct OPmaterialPBRInstance {
	OPmaterialPBR* rootMaterial;
	OPmaterialInstance rootMaterialInstance;

	OPfloat albedoUsage;
	OPfloat specularUsage;
	OPfloat glossUsage;
	OPfloat normalUsage;
	OPvec4 albedoColor;
	OPvec3 specularColor;
	OPfloat glossColor;
	OPvec3 normalColor;

	OPmaterialPBRInstance() { }
	OPmaterialPBRInstance(OPmaterialPBR* material) {
		Init(material);
	}

	void Init(OPmaterialPBR* material);

	inline void SetAlbedoMap(OPtexture* texture) {
		rootMaterialInstance.AddParam("uAlbedoMap", texture, 0);
		albedoUsage = 1.0f;
	}

	inline void SetSpecularMap(OPtexture* texture) {
		rootMaterialInstance.AddParam("uSpecularMap", texture, 1);
		specularUsage = 1.0f;
	}

	inline void SetGlossMap(OPtexture* texture) {
		rootMaterialInstance.AddParam("uGlossMap", texture, 2);
		glossUsage = 1.0f;
	}

	inline void SetNormalMap(OPtexture* texture) {
		rootMaterialInstance.AddParam("uNormalMap", texture, 3);
		normalUsage = 1.0f;
	}	
	
	inline void SetAlbedoMap(const OPchar* texture) {
		SetAlbedoMap((OPtexture*)OPcmanLoadGet(texture));
	}

	inline void SetSpecularMap(const OPchar* texture) {
		SetSpecularMap((OPtexture*)OPcmanLoadGet(texture));
	}

	inline void SetGlossMap(const OPchar* texture) {
		SetGlossMap((OPtexture*)OPcmanLoadGet(texture));
	}

	inline void SetNormalMap(const OPchar* texture) {
		SetNormalMap((OPtexture*)OPcmanLoadGet(texture));
	}

	inline void SetEnvironmentMap(OPtextureCube* textureCube) {
		rootMaterialInstance.AddParam("uEnvironmentMap", textureCube, 4);
	}
};