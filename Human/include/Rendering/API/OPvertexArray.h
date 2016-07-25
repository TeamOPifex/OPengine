#ifndef OPENGINE_HUMAN_RENDERING_VERTEX_ARRAY
#define OPENGINE_HUMAN_RENDERING_VERTEX_ARRAY

#include "./Core/include/OPtypes.h"

struct OPvertexArray;
typedef struct OPvertexArray OPvertexArray;

struct OPvertexArray {
	void(*Bind)(void* ptr) = 0;
	void(*Unbind)(void* ptr) = 0;
};

#endif