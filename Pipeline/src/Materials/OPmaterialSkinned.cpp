#include "./Pipeline/include/Materials/OPmaterialSkinned.h"
#include "./Human/include/Rendering/OPmodel.h"

OPmaterialSkinned* OPmaterialSkinned::Init(OPmaterial* material) {
	rootMaterial = material;
	return this;
}

OPmaterialSkinned* OPmaterialSkinned::Init(OPeffect* effect) {
	OPmaterial::Init(effect);
	return this;
}

OPmaterialSkinned** OPmaterialSkinned::CreateInstances(OPmodel* model, bool setMeta, bool materialPerMesh) {
	ui32 count = model->meshCount;
	if (!materialPerMesh) {
		count = 1;
	}

	OPmaterialSkinned** result = OPALLOC(OPmaterialSkinned*, count);

	for (ui32 i = 0; i < count; i++) {
		result[i] = OPNEW(OPmaterialSkinned(this));

		if (!setMeta) continue;

		if (model->meshes[i].materialDesc != NULL) {
			if (model->meshes[i].materialDesc->diffuse != NULL) {
				result[i]->SetAlbedoMap(model->meshes[i].materialDesc->diffuse);
			}
		}
	}
	return result;
}