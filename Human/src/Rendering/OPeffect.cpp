#include "./Human/include/Rendering/OPeffect.h"
#include "./Core/include/Assert.h"

OPeffect* OPEFFECT_ACTIVE = NULL;

void OPeffect::Init(OPshader* vert, OPshader* frag, OPvertexLayout* vertexLayout) {
	OPRENDERER_ACTIVE->Effect.Init(this, vert, frag, vertexLayout);
}

OPshaderUniform* OPeffect::GetUniform(const OPchar* name) {
	for (ui32 i = 0; i < uniformCount; i++) {
		if (OPstringEquals(name, uniforms[i].name)) {
			return &uniforms[i];
		}
	}

	OPlogChannel((ui32)OPlogLevel::VERBOSE, "SHADER", "Shader Uniform not present: %s", name);
	return NULL;

	//bool found = this->uniforms.Get(name, (void**)&result);
	//if (!found) {
	//	//OPlogErr("Shader Uniform not present: %s", name);
	//}
	////ASSERT(found, "The uniform must be added first");

	//return result;
}

OPshaderUniformBuffer* OPeffect::GetUniformBuffer(const OPchar* name) {
	for (ui32 i = 0; i < uniformBufferCount; i++) {
		if (OPstringEquals(name, uniformBuffers[i].name)) {
			return &uniformBuffers[i];
		}
	}

	OPlogChannel((ui32)OPlogLevel::VERBOSE, "SHADER", "Shader Uniform Buffer not present: %s", name);
	return NULL;

	//OPlinkedListNode* node = this->uniformBuffers.First;

	//while (node != NULL) {
	//	OPshaderUniformBuffer* ubo = (OPshaderUniformBuffer*)node->Data;
	//	if (OPstringEquals(name, ubo->name)) {
	//		return ubo;
	//	}
	//	node = node->Next;
	//}
	//
	//return NULL;
}	

OPshaderUniformBufferUniform* OPeffect::GetUniformBufferUniform(OPshaderUniformBuffer* ubo, const OPchar* name) {
	for (ui32 i = 0; i < ubo->uniformCount; i++) {
		if (OPstringEquals(name, ubo->uniforms[i].name)) {
			return &ubo->uniforms[i];
		}
	}

	OPlogChannel((ui32)OPlogLevel::VERBOSE, "SHADER", "Shader Uniform Buffer Uniform not present: %s", name);

	return NULL;

	//OPshaderUniformBufferUniform* result = NULL;

	//bool found = ubo->uniforms.Get(name, (void**)&result);
	//if (!found) {
	//	//OPlogErr("Shader Uniform not present: %s", name);
	//}

	//return result;
}


void OPeffect::UpdateVertexLayout(OPvertexLayout* vertexLayout) {
	OPRENDERER_ACTIVE->Effect.SetVertexLayout(this, vertexLayout);
}