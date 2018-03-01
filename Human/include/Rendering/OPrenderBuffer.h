#pragma once

#include "./Core/include/OPtypes.h"

#include "./Human/include/Rendering/OPvertexBuffer.h"
#include "./Human/include/Rendering/OPindexBuffer.h"

void OPrenderBindBuffer(OPvertexBuffer* buffer);
void OPrenderBindBuffer(OPindexBuffer* buffer);

void OPrenderDrawBufferIndexed(ui32 offset);
void OPrenderDrawBuffer(ui32 offset);
void OPrenderDrawIndexed(ui32 offset, ui32 count);
void OPrenderDrawUserArray(void* vertices, ui32 attrs, ui32 offset, ui32 count);


struct OPrenderBuffer {
	void* internalPtr;
	ui8 samples = 1;

	inline void Bind() {
		OPRENDERER_ACTIVE->RenderBuffer.Bind(this);
	}

	inline void SetSize(ui32 width, ui32 height) {
		OPRENDERER_ACTIVE->RenderBuffer.SetSize(this, width, height);
	}

	inline void Destroy() {
		OPRENDERER_ACTIVE->RenderBuffer.Destroy(this);
	}
};