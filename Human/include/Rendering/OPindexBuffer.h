#pragma once

struct OPindexBuffer;
typedef struct OPindexBuffer OPindexBuffer;

#include "./Core/include/OPtypes.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/Enums/OPindexSize.h"

struct OPindexBuffer {
	void* internalPointer;
	OPuint ElementCount;
	OPindexSize::Enum ElementSize;

	inline OPindexBuffer* Init() {
		return OPRENDERER_ACTIVE->IndexBuffer.Init(this);
	}

	inline void SetData(OPindexSize::Enum elementSize, OPuint count, const void* data) {
		OPRENDERER_ACTIVE->IndexBuffer.SetData(this, elementSize, (ui32)count, data);
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

	static inline OPindexBuffer* Create() {
		return OPRENDERER_ACTIVE->IndexBuffer.Create();
	}
};