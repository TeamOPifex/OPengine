#ifndef OPENGINE_HUMAN_RENDERING_API_INDEX_BUFFER
#define OPENGINE_HUMAN_RENDERING_API_INDEX_BUFFER

#include "./Core/include/OPtypes.h"

struct OPindexBufferAPI;
typedef struct OPindexBufferAPI OPindexBufferAPI;

struct OPindexBuffer;
#include "./Human/include/Rendering/Enums/OPindexSize.h"

struct OPindexBufferAPI {
	OPindexBuffer*(*Create)() = 0;
	OPindexBuffer*(*Init)(OPindexBuffer* indexBuffer) = 0;
	void(*SetData)(OPindexBuffer* indexBuffer, OPindexSize size, ui32 count, const void* data) = 0;
	void(*Bind)(OPindexBuffer* indexBuffer) = 0;
	void(*Unbind)(OPindexBuffer* indexBuffer) = 0;
	void(*Destroy)(OPindexBuffer* indexBuffer) = 0;
};

#endif