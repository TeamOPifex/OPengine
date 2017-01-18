#include "./Human/include/Rendering/OPmaterial.h"
#include "./Human/include/Rendering/OPmodel.h"

ui64 OPMATERIAL_GLOBAL_ID = 1;
OPuint OPMATERIALINSTANCE_GLOBAL_ID = 1;

void OPmaterial::Init(OPeffect* effectIn) {
	effect = effectIn;
	paramIndex = 0;
	id = OPMATERIAL_GLOBAL_ID++;
	depth = 1;
	cull = 1;
}

void OPmaterial::Init(OPmaterial* base) {
	ASSERT(base != NULL, "Can't create amaterial based on a NULL material");
	rootMaterial = base;
	depth = base->depth;
	cull = base->cull;
	visible = base->visible;
	alpha = base->alpha;
	cullFace = base->cullFace;
	effect = base->effect;
}

OPmaterialParam* OPmaterial::GetParam(const OPchar* name) {
	// This faster for the cpu than a dictionary lookup despite this being O(n)
	for (OPuint i = 0; i < paramIndex; i++) {
		if (OPstringEquals(params[i].name, name)) {
			return &params[i];
		}
	}
	return NULL;
}

void OPmaterial::Bind(bool onlyParams) {
	if (rootMaterial != NULL) {
		rootMaterial->Bind(true);
	}
	else {
		effect->Bind();
	}

	if (!onlyParams) {
		// We'll use the top most version
		OPrenderDepth(depth);

		if (cull) {
			OPrenderCull(cull);
			OPrenderCullMode(cullFace);
		}
	}

	for (OPuint i = 0; i < paramIndex; i++) {

		switch (params[i].type) {
		case OPmaterialParamType::TEXTURE: {
			effect->Set(params[i].name, (OPtexture*)params[i].data, params[i].count);
			break;
		}
		case OPmaterialParamType::TEXTURE_CUBE: {
			effect->Set(params[i].name, (OPtextureCube*)params[i].data, params[i].count);
			break;
		}
		case OPmaterialParamType::VECTOR3: {
			effect->Set(params[i].name, (OPvec3*)params[i].data);
			break;
		}
		case OPmaterialParamType::VECTOR4: {
			effect->Set(params[i].name, (OPvec4*)params[i].data);
			break;
		}
		case OPmaterialParamType::MATRIX4: {
			effect->Set(params[i].name, (OPmat4*)params[i].data);
			break;
		}
		case OPmaterialParamType::MATRIX4V: {
			effect->Set(params[i].name, params[i].count, (OPmat4*)params[i].data);
			break;
		}
		case OPmaterialParamType::FLOAT: {
			effect->Set(params[i].name, *(f32*)params[i].data);
			break;
		}
		case OPmaterialParamType::BOOL: {
			effect->Set(params[i].name, *(bool*)params[i].data);
			break;
		}
		}
	}
}

void OPmaterial::AddParam(OPmaterialParamType paramType, const OPchar* name, void* data, ui8 count) {
	OPlogChannel((ui32)OPlogLevel::VERBOSE, "SHADER", "Name %s %p", name, data);
	params[paramIndex].type = paramType;
	params[paramIndex].name = name;
	params[paramIndex].data = data;
	params[paramIndex].count = count;
	paramIndex++;
}

void OPmaterial::Destroy() {

}

OPmaterial** OPmaterial::CreateInstances(OPmodel* model, bool materialPerMesh) {
	ui32 count = model->meshCount;

	if (!materialPerMesh) {
		count = 1;
	}

	OPmaterial** result = OPALLOC(OPmaterial*, count);
	for (ui32 i = 0; i < count; i++) {
		result[i] = OPNEW(OPmaterial(this));

		if (model->meshes[i].materialDesc != NULL) {
			if (model->meshes[i].materialDesc->diffuse != NULL) {
				OPtexture* tex = (OPtexture*)OPCMAN.LoadGet(model->meshes[i].materialDesc->diffuse);
				if (tex != NULL) {
					result[i]->AddParam("uAlbedoMap", tex, 0);
				}
			}
		}
	}

	return result;
}

OPmaterial* OPmaterial::Create(OPeffect* effect) {
	OPmaterial* material = (OPmaterial*)OPalloc(sizeof(OPmaterial));
	material->Init(effect);
	return material;
}