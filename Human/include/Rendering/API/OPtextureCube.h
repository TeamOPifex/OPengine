#ifndef OPENGINE_HUMAN_RENDERING_TEXTURE_CUBE
#define OPENGINE_HUMAN_RENDERING_TEXTURE_CUBE

#include "./Core/include/OPtypes.h"

struct OPtextureCube;
typedef struct OPtextureCube OPtextureCube;

struct OPtextureCube {
	void(*Bind)(void* ptr) = 0;
	void(*Unbind)(void* ptr) = 0;
};

#endif