#include "./Pipeline/include/OPmaterialPhong.h"

void OPmaterialPhong::Init(OPeffect* effect) {
	lightPos = OPvec3(1, 1, 1);

	rootMaterial->Init(effect);
	rootMaterial->AddParam("uLightPos", &lightPos);
}

OPmaterialPhong* OPmaterialPhong::CreateInstance() {
	return OPNEW(OPmaterialPhong(this));
}
//
//void OPmaterialPhong::Init(OPmaterialPhong* material) {
//	rootMaterial->Init(&material->rootMaterial);
//}