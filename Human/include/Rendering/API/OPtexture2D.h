#ifndef OPENGINE_HUMAN_RENDERING_TEXTURE2D
#define OPENGINE_HUMAN_RENDERING_TEXTURE2D

#include "./Core/include/OPtypes.h"

struct OPtexture2D;
typedef struct OPtexture2D OPtexture2D;

struct OPtexture2D {
	void(*Bind)(void* ptr) = 0;
	void(*Unbind)(void* ptr) = 0;
};

#endif