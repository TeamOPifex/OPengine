#include "./Pipeline/include/OPmaterialPhong.h"

OPmaterialPhong* OPmaterialPhong::Init(OPeffect* effect) {
	OPmaterial::Init(effect);
	lightPos = OPvec3(1, 1, 1);
	AddParam("uLightPos", &lightPos);
	return this;
}

OPmaterialPhong* OPmaterialPhong::Init(OPmaterial* material) {
	rootMaterial = material;
	lightPos = OPvec3(1, 1, 1);
	AddParam("uLightPos", &lightPos);
	return this;
}

OPmaterialPhong* OPmaterialPhong::CreateInstance() {
	return OPNEW(OPmaterialPhong(this));
}
//
//void OPmaterialPhong::Init(OPmaterialPhong* material) {
//	rootMaterial->Init(&material->rootMaterial);
//}