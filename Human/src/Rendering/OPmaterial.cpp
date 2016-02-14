#include "./Human/include/Rendering/OPmaterial.h"

ui64 OPMATERIAL_GLOBAL_ID = 1;

void OPmaterialInit(OPmaterial* material, OPeffect* effect) {
	material->effect = effect;
	material->paramIndex = 0;
	material->id = OPMATERIAL_GLOBAL_ID++;
}