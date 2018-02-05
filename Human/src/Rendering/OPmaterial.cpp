#include "./Human/include/Rendering/OPmaterial.h"
#include "./Human/include/Rendering/OPmodel.h"
#include "./Human/include/Rendering/OPrendererEntity.h"

OPuint OPMATERIAL_GLOBAL_ID = 1;
OPuint OPMATERIALINSTANCE_GLOBAL_ID = 1;

OPmaterial* OPmaterial::Init(OPeffect* effectIn) {
	rootMaterial = NULL;
	effect = effectIn;
	paramIndex = 0;
	id = OPMATERIAL_GLOBAL_ID++;
	depth = true;
	cull = true;
	cullFace = OPcullFace::BACK;
	SetupWVP();
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
	SetupWVP();
	return this;
}

void OPmaterial::SetupWVP() {

	if (rootMaterial == NULL) {
		// locate world, view, proj
		worldUniform = effect->GetUniform("uWorld");
		if (worldUniform == NULL) {
			worldBuffer = effect->GetUniformBuffer("ModelBuffer");
			if (worldBuffer != NULL) {
				worldBufferUniform = effect->GetUniformBufferUniform(worldBuffer, "uWorld");
			}
		}

		viewUniform = effect->GetUniform("uView");
		projUniform = effect->GetUniform("uProj");
		if (viewUniform == NULL || projUniform == NULL) {
			viewProjBuffer = effect->GetUniformBuffer("CameraBuffer");
			if (viewProjBuffer != NULL) {
				viewBufferUniform = effect->GetUniformBufferUniform(viewProjBuffer, "uView");
				projBufferUniform = effect->GetUniformBufferUniform(viewProjBuffer, "uProj");
			}
		}
	}
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
			return true;
		}
	}
	else {
		OPmaterialUniformBufferParam* p = GetParam(ubo, name);
		if (p != NULL) {
			p->data = ptr;
			return true;
		}
	}

	return false;
}

void OPmaterial::SetWorld(OPmat4* world) {
	if (rootMaterial != NULL) {
		rootMaterial->SetWorld(world);
	}
	else {
		if (worldUniform != NULL) {
			// set world
			effect->Set(worldUniform, world, 0);
		}
		else if (worldBuffer != NULL && worldBufferUniform != NULL) {
			effect->Set(worldBuffer, worldBufferUniform, world, 0);
		}
	}
}

void OPmaterial::SetCamera(OPcam* camera) {
	if (rootMaterial != NULL) {
		rootMaterial->SetCamera(camera);
	}
	else {
		if (viewUniform != NULL) {
			// set world
			effect->Set(viewUniform, &camera->view, 0);
		}
		else if (viewProjBuffer != NULL && viewBufferUniform != NULL) {
			effect->Set(viewProjBuffer, viewBufferUniform, &camera->view, 0);
		}

		if (projUniform != NULL) {
			// set world
			effect->Set(projUniform, &camera->proj, 0);
		}
		else if (viewProjBuffer != NULL && viewBufferUniform != NULL) {
			effect->Set(viewProjBuffer, projBufferUniform, &camera->proj, 0);
		}
	}
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

		//OPrenderCull(cull);
		//if (cull) {
		//	OPrenderCullMode(cullFace);
		//}
	}

	for (OPuint i = 0; i < paramUniformBufferIndex; i++) {
		effect->Set(paramsUnformBuffer[i].ubo, paramsUnformBuffer[i].uniform, paramsUnformBuffer[i].data, paramsUnformBuffer[i].loc);
	}

	for (OPuint i = 0; i < effect->uniformBufferCount; i++) {
		effect->uniformBuffers[i].Bind();
	}

	for (OPuint i = 0; i < paramIndex; i++) {
		effect->Set(params[i].uniform, params[i].data, params[i].loc);
	}
}

ui32 OPmaterial::TextureOffset() {
	if (rootMaterial != NULL) {
		return textureSlot + rootMaterial->TextureOffset();
	}
	return textureSlot;
}

ui32 OPmaterial::NextTextureSlot() {
	ui32 offset = 0;
	if (rootMaterial != NULL) {
		offset = rootMaterial->TextureOffset();
	}
	
	return offset + (textureSlot++);
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
		if (uniform->type == OPshaderUniformType::TEXTURE || uniform->type == OPshaderUniformType::TEXTURE_CUBE) {
			params[paramIndex].loc = NextTextureSlot();
		}
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