#ifndef OPENGINE_HUMAN_RENDERING_SHADER_UNIFORM
#define OPENGINE_HUMAN_RENDERING_SHADER_UNIFORM

#include "./Core/include/OPtypes.h"

struct OPshaderUniform;
typedef struct OPshaderUniform OPshaderUniform;

struct OPshaderUniform {
	void(*Bind)(void* ptr) = 0;
	void(*Unbind)(void* ptr) = 0;
};

#endif