#pragma once

class OPmaterialPBR;

#include "./Human/include/Rendering/OPmaterial.h"
#include "./Human/include/Rendering/Skinning/OPskeleton.h"

class OPmaterialPBR : public OPmaterial {
public:
	OPvec3 lightPos;

	OPmaterialPBR() { }
	OPmaterialPBR(OPmaterial* material) : OPmaterial(material) {
	}
	OPmaterialPBR(OPeffect* effect) {
		Init(effect);
	}

	inline OPmaterial* Base() { return rootMaterial; }

	inline void SetCameraPos(OPcam* camera) {
		rootMaterial->AddParam("uCamPos", &camera->pos);
	}

	inline void SetLightPos(OPvec3 pos) {
		lightPos = pos;
	}

	inline void SetAlbedoMap(OPtexture* texture) {
		SetMap("uAlbedoMap", texture);
	}

	inline void SetAlbedoMap(const OPchar* texture) {
		SetAlbedoMap((OPtexture*)OPCMAN.LoadGet(texture));
	}

	OPmaterialPBR* Init(OPmaterial* material);
	OPmaterialPBR* Init(OPeffect* effect);
	OPmaterialPBR* Init();
};