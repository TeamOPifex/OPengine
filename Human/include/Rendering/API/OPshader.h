#ifndef OPENGINE_HUMAN_RENDERING_SHADER
#define OPENGINE_HUMAN_RENDERING_SHADER

#include "./Core/include/OPtypes.h"

struct OPshader;
typedef struct OPshader OPshader;

struct OPshader {
	void(*Bind)(void* ptr) = 0;
	void(*Unbind)(void* ptr) = 0;
};

#endif