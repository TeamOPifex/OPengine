#include "./Human/include/Rendering/OPmaterial.h"
#include "./Human/include/Rendering/OPmodel.h"
#include "./Human/include/Rendering/OPrendererEntity.h"

OPuint OPMATERIAL_GLOBAL_ID = 1;
OPuint OPMATERIALINSTANCE_GLOBAL_ID = 1;

OPmaterial* OPmaterial::Init(OPeffect* effectIn) {
	effect = effectIn;
	paramIndex = 0;
	id = OPMATERIAL_GLOBAL_ID++;
	depth = 1;
	cull = 1;
	return this;
}

OPmaterial* OPmaterial::Init(OPmaterial* base) {
	ASSERT(base != NULL, "Can't create amaterial based on a NULL material");
	rootMaterial = base;
	id = OPMATERIAL_GLOBAL_ID++;
	depth = base->depth;
	cull = base->cull;
	visible = base->visible;
	alpha = base->alpha;
	cullFace = base->cullFace;
	effect = base->effect;
	return this;
}


OPmaterialParam* OPmaterial::GetParam(const OPchar* name) {
	for (OPuint i = 0; i < paramIndex; i++) {
		if (OPstringEquals(params[i].uniform->name, name)) {
			return &params[i];
		}
	}

	return NULL;
}

OPmaterialUniformBufferParam* OPmaterial::GetParam(const OPchar* ubo, const OPchar* name) {
	for (OPuint i = 0; i < paramUniformBufferIndex; i++) {
		if (OPstringEquals(paramsUnformBuffer[i].ubo->name, ubo) && OPstringEquals(paramsUnformBuffer[i].uniform->name, name)) {
			return &paramsUnformBuffer[i];
		}
	}

	return NULL;
}

bool OPmaterial::SetParam(const OPchar* ubo, const OPchar* name, void* ptr, ui32 loc) {
	if (ubo == NULL) {
		OPmaterialParam* p = GetParam(name);
		if (p != NULL) {
			p->data = ptr;
			p->loc = loc;
			return true;
		}
	}
	else {
		OPmaterialUniformBufferParam* p = GetParam(ubo, name);
		if (p != NULL) {
			p->data = ptr;
			p->loc = loc;
			return true;
		}
	}

	return false;
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

	for (OPuint i = 0; i < paramUniformBufferIndex; i++) {
		effect->Set(paramsUnformBuffer[i].ubo, paramsUnformBuffer[i].uniform, paramsUnformBuffer[i].data, paramsUnformBuffer[i].loc);
	}

	for (OPuint i = 0; i < effect->uniformBufferCount; i++) {
		effect->uniformBuffers[i].Bind();
	}

	for (OPuint i = 0; i < paramIndex; i++) {
		effect->Set(params[i].uniform, params[i].data, params[i].loc);

		//if (params[i].ubo != NULL) {
		//	// Uniform in a Uniform Buffer Object
		//	effect->Set()
		//}
		//else {
		//	// Normal Uniform
		//	switch (params[i].type) {
		//		case OPmaterialParamType::TEXTURE: {
		//			effect->Set(params[i].name, (OPtexture*)params[i].data, params[i].count);
		//			break;
		//		}
		//		case OPmaterialParamType::TEXTURE_CUBE: {
		//			effect->Set(params[i].name, (OPtextureCube*)params[i].data, params[i].count);
		//			break;
		//		}
		//		case OPmaterialParamType::VECTOR3: {
		//			effect->Set(params[i].name, (OPvec3*)params[i].data);
		//			break;
		//		}
		//		case OPmaterialParamType::VECTOR4: {
		//			effect->Set(params[i].name, (OPvec4*)params[i].data);
		//			break;
		//		}
		//		case OPmaterialParamType::MATRIX4: {
		//			effect->Set(params[i].name, (OPmat4*)params[i].data);
		//			break;
		//		}
		//		case OPmaterialParamType::MATRIX4V: {
		//			effect->Set(params[i].name, params[i].count, (OPmat4*)params[i].data);
		//			break;
		//		}
		//		case OPmaterialParamType::FLOAT: {
		//			effect->Set(params[i].name, *(f32*)params[i].data);
		//			break;
		//		}
		//		case OPmaterialParamType::INT: {
		//			effect->Set(params[i].name, *(i32*)params[i].data);
		//			break;
		//		}
		//		case OPmaterialParamType::BOOL: {
		//			effect->Set(params[i].name, *(bool*)params[i].data);
		//			break;
		//		}
		//	}
		//}
	}
}

void OPmaterial::AddParam(const OPchar* ubo, const OPchar* name, void* data, ui32 loc) {

	if (ubo == NULL) {
		OPshaderUniform* uniform = effect->GetUniform(name);
		if (uniform == NULL) {
			OPlogErr("Uniform was not present: %s", name);
			return;
		}
		OPlogChannel((ui32)OPlogLevel::VERBOSE, "SHADER", "Name %s %p", name, data);
		params[paramIndex].uniform = uniform;
		params[paramIndex].data = data;
		params[paramIndex].loc = loc;
		paramIndex++;
	}
	else {
		OPshaderUniformBuffer* uniformBuffer = effect->GetUniformBuffer(ubo);
		if (uniformBuffer == NULL) {
			OPlogErr("Uniform Buffer was not present: %s", ubo);
			return;
		}

		OPshaderUniformBufferUniform* uniformBufferUniform = effect->GetUniformBufferUniform(uniformBuffer, name);
		if (uniformBuffer == NULL) {
			OPlogErr("Uniform Buffer Uniform was not present: %s in %s", name, ubo);
			return;
		}

		OPlogChannel((ui32)OPlogLevel::VERBOSE, "SHADER", "Name %s %p", name, data);
		paramsUnformBuffer[paramUniformBufferIndex].ubo = uniformBuffer;
		paramsUnformBuffer[paramUniformBufferIndex].uniform = uniformBufferUniform;
		paramsUnformBuffer[paramUniformBufferIndex].data = data;
		paramsUnformBuffer[paramUniformBufferIndex].loc = loc;
		paramUniformBufferIndex++;
	}
}

void OPmaterial::AddParam(OPskeleton* skeleton) {
	AddParam("uBones", skeleton->skinned, (ui8)skeleton->hierarchyCount);
}

void OPmaterial::Destroy() {

}

OPmaterial* OPmaterial::CreateInstances(OPmodel* model, bool materialPerMesh) {
	ui32 count = model->meshCount;

	if (!materialPerMesh) {
		count = 1;
	}
	

	OPmaterial* result = OPNEW(OPmaterial[count]);// OPALLOC(OPmaterial, count);
	for (ui32 i = 0; i < count; i++) {
		OPmaterial* mat = &result[i];
		mat->Init(this);

		if (model->meshes[i].materialDesc != NULL) {
			if (model->meshes[i].materialDesc->diffuse != NULL) {
				OPtexture* tex = (OPtexture*)OPCMAN.LoadGet(model->meshes[i].materialDesc->diffuse);
				if (tex != NULL) {
					mat->AddParam("uAlbedoMap", tex, 0);
				}
			}
		}
	}

	return result;
}

void OPmaterial::SetMeta(OPrendererEntity* rendererEntity) {
	ASSERT(rendererEntity->material != NULL, "Must have materials set first");

	ui32 count = rendererEntity->model->meshCount;	
	if (!rendererEntity->desc.materialPerMesh) {
		count = 1;
	}

	for (ui32 i = 0; i < count; i++) {
		OPmaterial* mat = &rendererEntity->material[i];

		if (rendererEntity->model->meshes[i].materialDesc == NULL) continue;		

		// Load Diffuse Texture from meta
		if (rendererEntity->model->meshes[i].materialDesc->diffuse != NULL) {
			OPtexture* tex = (OPtexture*)OPCMAN.LoadGet(rendererEntity->model->meshes[i].materialDesc->diffuse);
			if (tex != NULL) {
				mat->AddParam("uAlbedoMap", tex, 0);
			}
		}
	}

}

OPmaterial* OPmaterial::CreateInstances(OPrendererEntity* rendererEntity) {
	ui32 count = rendererEntity->model->meshCount;
	if (!rendererEntity->desc.materialPerMesh) {
		count = 1;
	}

	OPmaterial* result = OPNEW(OPmaterial[count]);
	for (ui32 i = 0; i < count; i++) {
		OPmaterial* mat = &result[i];
		mat->Init(this);
	}

	return result;
}

OPmaterial* OPmaterial::Create(OPeffect* effect) {
	OPmaterial* material = OPNEW(OPmaterial());
	material->Init(effect);
	return material;
}