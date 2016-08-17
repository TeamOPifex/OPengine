#pragma once

struct OPshaderUniform;
typedef OPshaderUniform OPshaderUniform;

#include "./Human/include/Rendering/OPrender.h"

struct OPshaderUniform {
	void* internalPtr;
	bool Found;

	inline void Destroy() {
		OPRENDERER_ACTIVE->ShaderUniform.Destroy(this);
	}
};