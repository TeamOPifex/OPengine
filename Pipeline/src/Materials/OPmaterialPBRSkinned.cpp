#include "./Pipeline/include/Materials/OPmaterialPBRSkinned.h"
#include "./Human/include/Rendering/OPmodel.h"

OPmaterialPBRSkinned* OPmaterialPBRSkinned::Init(OPmaterial* material) {
	rootMaterial = material;
	return this;
}

OPmaterialPBRSkinned* OPmaterialPBRSkinned::Init(OPeffect* effect) {
	if (effect == NULL) {
		effect = OPNEW(OPeffect("", ""));
	}
	OPmaterial::Init(effect);
	return this;
}

OPmaterialPBRSkinned* OPmaterialPBRSkinned::Init() {
	Init(OPNEW(OPeffect("Common/OPpbrSkinned.vert", "Common/OPpbr.frag")));
	return this;
}