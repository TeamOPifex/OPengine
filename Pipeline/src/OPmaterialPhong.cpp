#include "./Pipeline/include/OPmaterialPhong.h"

void OPmaterialPhong::Init(OPeffect* effect) {
	lightPos = OPvec3(1, 1, 1);

	rootMaterial.Init(effect);
	rootMaterial.AddParam("uLightPos", &lightPos);
}

OPmaterialPhongInstance* OPmaterialPhong::CreateInstance() {
	return OPNEW(OPmaterialPhongInstance(this));
}

inline void OPmaterialPhongInstance::Init(OPmaterialPhong* material) {
	rootMaterial = material;
	rootMaterialInstance.Init(&material->rootMaterial);
}