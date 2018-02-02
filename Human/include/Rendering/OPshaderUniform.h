#pragma once

struct OPshaderUniform;
typedef OPshaderUniform OPshaderUniform;

#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/Enums/OPmaterialParamType.h"

struct OPshaderUniform {
	void* internalPtr;
	bool Found;
	OPchar* name = NULL;
	OPshaderUniformType::Enum type;
	ui8 count;

	inline void Destroy() {
		OPRENDERER_ACTIVE->ShaderUniform.Destroy(this);
		if (name != NULL) {
			OPfree(name);
		}
	}
};