#include "./Human/include/Rendering/OPeffect.h"
#include "./Core/include/Assert.h"

OPeffect* OPEFFECT_ACTIVE = NULL;

void OPeffect::Init(OPshader* vert, OPshader* frag, OPvertexLayout* vertexLayout) {
	OPRENDERER_ACTIVE->Effect.Init(this, vert, frag, vertexLayout);
}

OPshaderUniform* OPeffect::GetUniform(const OPchar* name) {
	OPshaderUniform* result = NULL;

	bool found = this->uniforms.Get(name, (void**)&result);
	if (!found) {
		OPlogChannel((ui32)OPlogLevel::VERBOSE, "SHADER", "Shader Uniform not present: %s", name);
	}
	//ASSERT(found, "The uniform must be added first");

	return result;
}


void OPeffect::UpdateVertexLayout(OPvertexLayout* vertexLayout) {
	OPRENDERER_ACTIVE->Effect.SetVertexLayout(this, vertexLayout);
}