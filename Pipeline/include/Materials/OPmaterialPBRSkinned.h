#pragma once

class OPmaterialPBRSkinned;

#include "./Pipeline/include/Materials/OPmaterialPBR.h"
#include "./Human/include/Rendering/OPmaterial.h"
#include "./Human/include/Rendering/Skinning/OPskeleton.h"

class OPmaterialPBRSkinned : public OPmaterial {
public:
	OPvec3 lightPos;

	OPmaterialPBRSkinned() { }
	OPmaterialPBRSkinned(OPmaterial* material) : OPmaterial(material) {
	}
	OPmaterialPBRSkinned(OPeffect* effect) {
		Init(effect);
	}

	inline void SetBones(OPskeleton* skeleton) {
		AddParam("uBones", skeleton->skinned, (ui8)skeleton->hierarchyCount);
	}
	
	OPmaterialPBRSkinned* Init(OPmaterial* material);
	OPmaterialPBRSkinned* Init(OPeffect* effect);
	OPmaterialPBRSkinned* Init();
};