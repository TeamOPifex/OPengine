#include "./Pipeline/include/Materials/OPmaterialSkinned.h"

OPmaterialSkinned* OPmaterialSkinned::Init(OPmaterial* material) {
	rootMaterial = material;
	return this;
}

OPmaterialSkinnedInstance* OPmaterialSkinned::CreateInstance() {
	return OPNEW(OPmaterialSkinnedInstance(this));
}

inline void OPmaterialSkinnedInstance::Init(OPmaterialSkinned* material) {
	rootMaterial = material;
	rootMaterialInstance.Init(material->rootMaterial);
}
