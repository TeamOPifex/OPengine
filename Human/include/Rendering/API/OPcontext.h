#ifndef OPENGINE_HUMAN_RENDERING_CONTEXT
#define OPENGINE_HUMAN_RENDERING_CONTEXT

#include "./Core/include/OPtypes.h"

struct OPcontext;
typedef struct OPcontext OPcontext;

struct OPcontext {
	void(*Bind)(void* ptr) = 0;
	void(*Unbind)(void* ptr) = 0;
};

#endif