#pragma once

class OPmaterialPBR;

#include "./Human/include/Rendering/OPmaterial.h"

class OPmaterialPBR : public OPmaterial {
public:
	OPeffect* internalEffect = NULL;
	OPvec4 lightColor;
	OPvec3 lightDirection;
	OPfloat lightIntensity;

	OPfloat albedoUsage;
	OPfloat specularUsage;
	OPfloat glossUsage;
	OPfloat normalUsage;
	OPvec4 albedoColor;
	OPvec3 specularColor;
	OPfloat glossColor;
	OPvec3 normalColor;

	OPmaterialPBR() : OPmaterial() {  }
	OPmaterialPBR(OPeffect* effect) : OPmaterial(effect) {
		Init(effect);
		rootMaterial->AddParam("uPreintegratedFG", (OPtexture*)OPCMAN.LoadGet("PreintegratedFG.png"), 6);
	}

	OPmaterialPBR* Init(OPeffect* effect);
	OPmaterialPBR* Init(OPmaterial* material);

	inline void SetCamera(OPcam* camera) {
		rootMaterial->AddParam("uCamPos", &camera->pos);
	}

	inline OPmaterialPBR* CreateInstances(OPmodel* model, bool materialPerMesh) {
		// TODO: (garrett) fill in
		return NULL;
	}


	inline void SetAlbedoMap(OPtexture* texture) {
		AddParam("uAlbedoMap", texture, 0);
		albedoUsage = 1.0f;
	}
	
	inline void SetSpecularMap(OPtexture* texture) {
		AddParam("uSpecularMap", texture, 1);
		specularUsage = 1.0f;
	}
	
	inline void SetGlossMap(OPtexture* texture) {
		AddParam("uGlossMap", texture, 2);
		glossUsage = 1.0f;
	}
	
	inline void SetNormalMap(OPtexture* texture) {
		AddParam("uNormalMap", texture, 3);
		normalUsage = 1.0f;
	}	
		
	inline void SetAlbedoMap(const OPchar* texture) {
		SetAlbedoMap((OPtexture*)OPCMAN.LoadGet(texture));
	}
	
	inline void SetSpecularMap(const OPchar* texture) {
		SetSpecularMap((OPtexture*)OPCMAN.LoadGet(texture));
	}
	
	inline void SetGlossMap(const OPchar* texture) {
		SetGlossMap((OPtexture*)OPCMAN.LoadGet(texture));
	}
	
	inline void SetNormalMap(const OPchar* texture) {
		SetNormalMap((OPtexture*)OPCMAN.LoadGet(texture));
	}
	
	inline void SetEnvironmentMap(OPtextureCube* textureCube) {
		AddParam("uEnvironmentMap", textureCube, 4);
	}
		
};

//
//struct OPmaterialPBRInstance {
//	OPmaterial rootMaterialInstance;
//	OPmaterialPBR* rootMaterial;
//
//	OPfloat albedoUsage;
//	OPfloat specularUsage;
//	OPfloat glossUsage;
//	OPfloat normalUsage;
//	OPvec4 albedoColor;
//	OPvec3 specularColor;
//	OPfloat glossColor;
//	OPvec3 normalColor;
//
//	OPmaterialPBRInstance() { }
//	OPmaterialPBRInstance(OPmaterialPBR* material) {
//		Init(material);
//	}
//	OPmaterialPBRInstance(OPmaterialPBRInstance* instance) {
//		Init(instance->rootMaterial);
//		albedoUsage = instance->albedoUsage;
//		specularUsage = instance->specularUsage;
//		glossUsage = instance->glossUsage;
//		normalUsage = instance->normalUsage;
//		albedoColor = instance->albedoColor;
//		specularColor = instance->specularColor;
//		glossColor = instance->glossColor;
//		normalColor = instance->normalColor;
//		OPmaterialParam* param = NULL;
//		param = instance->GetParam("uAlbedoMap");
//		if (param != NULL) {
//			rootMaterialInstance.params[rootMaterialInstance.paramIndex++] = *param;
//		}
//		param = instance->GetParam("uSpecularMap");
//		if (param != NULL) {
//			rootMaterialInstance.params[rootMaterialInstance.paramIndex++] = *param;
//		}
//		param = instance->GetParam("uGlossMap");
//		if (param != NULL) {
//			rootMaterialInstance.params[rootMaterialInstance.paramIndex++] = *param;
//		}
//		param = instance->GetParam("uNormalMap");
//		if (param != NULL) {
//			rootMaterialInstance.params[rootMaterialInstance.paramIndex++] = *param;
//		}
//		param = instance->GetParam("uEnvironmentMap");
//		if (param != NULL) {
//			rootMaterialInstance.params[rootMaterialInstance.paramIndex++] = *param;
//		}
//	}
//
//	void Init(OPmaterialPBR* material);
//
//	inline OPmaterialInstance* Base() {
//		return &rootMaterialInstance;
//	}
//
//	inline OPmaterialParam* GetParam(const OPchar* name) {
//		return rootMaterialInstance.GetParam(name);
//	}
//
//	inline void SetAlbedoMap(OPtexture* texture) {
//		rootMaterialInstance.AddParam("uAlbedoMap", texture, 0);
//		albedoUsage = 1.0f;
//	}
//
//	inline void SetSpecularMap(OPtexture* texture) {
//		rootMaterialInstance.AddParam("uSpecularMap", texture, 1);
//		specularUsage = 1.0f;
//	}
//
//	inline void SetGlossMap(OPtexture* texture) {
//		rootMaterialInstance.AddParam("uGlossMap", texture, 2);
//		glossUsage = 1.0f;
//	}
//
//	inline void SetNormalMap(OPtexture* texture) {
//		rootMaterialInstance.AddParam("uNormalMap", texture, 3);
//		normalUsage = 1.0f;
//	}	
//	
//	inline void SetAlbedoMap(const OPchar* texture) {
//		SetAlbedoMap((OPtexture*)OPCMAN.LoadGet(texture));
//	}
//
//	inline void SetSpecularMap(const OPchar* texture) {
//		SetSpecularMap((OPtexture*)OPCMAN.LoadGet(texture));
//	}
//
//	inline void SetGlossMap(const OPchar* texture) {
//		SetGlossMap((OPtexture*)OPCMAN.LoadGet(texture));
//	}
//
//	inline void SetNormalMap(const OPchar* texture) {
//		SetNormalMap((OPtexture*)OPCMAN.LoadGet(texture));
//	}
//
//	inline void SetEnvironmentMap(OPtextureCube* textureCube) {
//		rootMaterialInstance.AddParam("uEnvironmentMap", textureCube, 4);
//	}
//	
//	inline OPmaterialPBRInstance* Clone() {
//		return OPNEW(OPmaterialPBRInstance(this));
//	}
//};