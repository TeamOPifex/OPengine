#ifndef OPENGINE_HUMAN_RENDERING_VERTEX_BUFFER_API
#define OPENGINE_HUMAN_RENDERING_VERTEX_BUFFER_API

#include "./Core/include/OPtypes.h"

struct OPvertexBufferAPI;
typedef struct OPvertexBufferAPI OPvertexBufferAPI;

struct OPvertexBuffer;
struct OPvertexLayout;
struct OPeffect;

struct OPvertexBufferAPI {
	OPvertexBuffer*(*Create)() = 0;
	OPvertexBuffer*(*Init)(OPvertexBuffer* vertexBuffer) = 0;
	void(*SetData)(OPvertexBuffer* vertexBuffer, ui32 elementSize, OPuint count, const void* data) = 0;
	void(*SetLayout)(OPvertexBuffer* vertexBuffer, OPvertexLayout* vertexLayout) = 0;
	void(*Bind)(OPvertexBuffer* vertexBuffer) = 0;
	void(*Unbind)(OPvertexBuffer* vertexBuffer) = 0;
	void(*Destroy)(OPvertexBuffer* vertexBuffer) = 0;
};

#endif