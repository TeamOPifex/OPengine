#pragma once

class OPmaterialPhong;

#include "./Human/include/Rendering/OPmaterial.h"

class OPmaterialPhong : public OPmaterial {
public:
	OPvec3 lightPos;

	OPmaterialPhong() { }
	OPmaterialPhong(OPmaterialPhong* material) : OPmaterial(material) { }
	OPmaterialPhong(OPeffect* effect) {
		Init(effect);
	}

	inline OPmaterial* Base() { return rootMaterial; }

	inline void SetCamera(OPcam* camera) {
		rootMaterial->AddParam("uCamPos", &camera->pos);
	}	
	
	inline void SetLightPos(OPvec3 pos) {
		lightPos = pos;
	}

	void Init(OPeffect* effect);
	inline void Init() {
		Init(OPNEW(OPeffect("Common/Phong.vert", "Common/Phong.frag")));
	}
	OPmaterialPhong* CreateInstance();
};

//
//struct OPmaterialPhongInstance {
//	OPmaterialInstance rootMaterialInstance;
//	OPmaterialPhong* rootMaterial;
//
//	OPmaterialPhongInstance() { }
//	OPmaterialPhongInstance(OPmaterialPhong* material) {
//		Init(material);
//	}
//	OPmaterialPhongInstance(OPmaterialPhongInstance* instance) {
//		Init(instance->rootMaterial);
//
//		OPmaterialParam* param = NULL;
//		param = instance->GetParam("uAlbedoMap");
//		if (param != NULL) {
//			rootMaterialInstance.params[rootMaterialInstance.paramIndex++] = *param;
//		}
//	}
//
//	void Init(OPmaterialPhong* material);
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
//	}
//
//	inline void SetAlbedoMap(const OPchar* texture) {
//		SetAlbedoMap((OPtexture*)OPCMAN.LoadGet(texture));
//	}
//	
//	inline OPmaterialPhongInstance* Clone() {
//		return OPNEW(OPmaterialPhongInstance(this));
//	}
//};