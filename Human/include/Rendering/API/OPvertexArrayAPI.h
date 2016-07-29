#ifndef OPENGINE_HUMAN_RENDERING_VERTEX_ARRAY_API
#define OPENGINE_HUMAN_RENDERING_VERTEX_ARRAY_API

#include "./Core/include/OPtypes.h"

struct OPvertexArrayAPI;
typedef struct OPvertexArrayAPI OPvertexArrayAPI;

struct OPvertexArray;
struct OPvertexLayout;

struct OPvertexArrayAPI {
	OPvertexArray*(*Create)(OPvertexLayout* vertexLayout) = 0;
	OPvertexArray*(*Init)(OPvertexArray* vertexArray, OPvertexLayout* vertexLayout) = 0;
	void(*Bind)(OPvertexArray* ptr) = 0;
	void(*Draw)(OPvertexArray* ptr, OPuint count) = 0;
	void(*Unbind)(OPvertexArray* ptr) = 0;
	void(*Destroy)(OPvertexArray* vertexArray) = 0;
};

#endif