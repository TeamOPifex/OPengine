#include "./Pipeline/include/Materials/OPmaterialPBR.h"
#include "./Human/include/Rendering/OPmodel.h"

OPmaterialPBR* OPmaterialPBR::Init(OPmaterial* material) {
	rootMaterial = material;
	return this;
}

OPmaterialPBR* OPmaterialPBR::Init(OPeffect* effect) {
	OPmaterial::Init(effect);
	return this;
}

OPmaterialPBR* OPmaterialPBR::Init() {
	Init(OPNEW(OPeffect("Common/OPpbr.vert", "Common/OPpbrIBL.frag")));
	return this;
}