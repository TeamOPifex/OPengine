#ifndef OPENGINE_HUMAN_RENDERING_SHADER_RESOURCE
#define OPENGINE_HUMAN_RENDERING_SHADER_RESOURCE

#include "./Core/include/OPtypes.h"

struct OPshaderResource;
typedef struct OPshaderResource OPshaderResource;

struct OPshaderResource {
	void(*Bind)(void* ptr) = 0;
	void(*Unbind)(void* ptr) = 0;
};

#endif