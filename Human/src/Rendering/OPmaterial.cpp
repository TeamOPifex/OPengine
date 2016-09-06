#include "./Human/include/Rendering/OPmaterial.h"
#include "./Human/include/Rendering/OPmodel.h"

ui64 OPMATERIAL_GLOBAL_ID = 1;
OPuint OPMATERIALINSTANCE_GLOBAL_ID = 1;

void OPmaterialInit(OPmaterial* material, OPeffect* effect) {
	material->effect = effect;
	material->paramIndex = 0;
	material->id = OPMATERIAL_GLOBAL_ID++;
	material->depth = 1;
	material->cull = 1;
}

void OPmaterial::Destroy() {

}

void OPmaterialInstance::Destroy() {

}


OPmaterialInstance** OPmaterial::CreateInstances(OPmodel* model) {
	OPmaterialInstance** result = OPALLOC(OPmaterialInstance*, model->meshCount);
	for (ui32 i = 0; i < model->meshCount; i++) {
		result[i] = OPNEW(OPmaterialInstance(this));

		if (model->meshes[i].materialDesc != NULL) {
			if (model->meshes[i].materialDesc->albedo != NULL) {
				OPtexture* tex = (OPtexture*)OPCMAN.LoadGet(model->meshes[i].materialDesc->albedo);
				result[i]->AddParam("uAlbedo", tex, 0);
			}
		}
	}
	return result;
}