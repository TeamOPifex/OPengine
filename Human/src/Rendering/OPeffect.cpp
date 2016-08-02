#include "./Human/include/Rendering/OPeffect.h"
#include "./Core/include/Assert.h"

OPeffect* OPEFFECT_ACTIVE = NULL;

void OPeffect::Init(OPshader* vert, OPshader* frag) {
	OPhashMapInit(&uniforms, 32);
	OPRENDERER_ACTIVE->Effect.Init(this, vert, frag);
}

OPshaderUniform* OPeffect::GetUniform(const OPchar* name) {
	OPshaderUniform* result = NULL;

	bool found = OPhashMapGet(&uniforms, name, (void**)&result);
	if (!found) {
		OPlog("Shader Uniform not added: %s", name);
	}
	ASSERT(found, "The uniform must be added first");

	return result;
}