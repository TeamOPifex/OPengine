#ifndef OPENGINE_HUMAN_RENDERING_INDEX_BUFFER
#define OPENGINE_HUMAN_RENDERING_INDEX_BUFFER

struct OPindexBuffer;
typedef struct OPindexBuffer OPindexBuffer;

#include "./Core/include/OPtypes.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/Enums/OPindexSize.h"


struct OPindexBuffer {
	void* internalPointer;
	OPuint ElementCount;
	OPindexSize ElementSize;

	inline OPindexBuffer* Init() {
		return OPRENDERER_ACTIVE->IndexBuffer.Init(this);
	}

	inline void SetData(OPindexSize elementSize, OPuint count, const void* data) {
		OPRENDERER_ACTIVE->IndexBuffer.SetData(this, elementSize, count, data);
	}

	inline void Bind() {
		OPRENDERER_ACTIVE->IndexBuffer.Bind(this);
	}

	inline void Unbind() {
		OPRENDERER_ACTIVE->IndexBuffer.Unbind(this);
	}

	inline void Destroy() {
		OPRENDERER_ACTIVE->IndexBuffer.Destroy(this);
	}
};

inline OPindexBuffer* OPindexBufferCreate() {
	return OPRENDERER_ACTIVE->IndexBuffer.Create();
}

#endif