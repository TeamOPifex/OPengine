#ifndef OPENGINE_HUMAN_RENDERING_TEXTURE_DEPTH
#define OPENGINE_HUMAN_RENDERING_TEXTURE_DEPTH

#include "./Core/include/OPtypes.h"

struct OPtextureDepth;
typedef struct OPtextureDepth OPtextureDepth;

struct OPtextureDepth {
	void(*Bind)(void* ptr) = 0;
	void(*Unbind)(void* ptr) = 0;
};

#endif