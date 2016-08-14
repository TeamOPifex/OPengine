#pragma once

struct OPmeshPacked;
typedef struct OPmeshPacked OPmeshPacked;

#include "./Human/include/Rendering/OPmeshPacker.h"
#include "./Human/include/Rendering/OPrenderBuffer.h"
#include "./Human/include/Rendering/Enums/OPindexSize.h"
#include "./Human/include/Rendering/OPvertexLayout.h"

struct OPmeshPacked {
	OPuint offset;
	OPuint count;
	OPuint elementCount;
	OPvertexLayout vertexLayout;

	OPmeshPacked() {

	}

	OPmeshPacked(OPvertexLayout vertexLayout, OPindexSize indSize, OPuint vertCount, OPuint indCount, void* vertices, void* indices) {
		Init(vertexLayout, indSize, vertCount, indCount, vertices, indices);
	}
	OPmeshPacked* Init(OPvertexLayout vertexLayout, OPindexSize indSize, OPuint vertCount, OPuint indCount, void* vertices, void* indices);
	static OPmeshPacked* Create(OPvertexLayout vertexLayout, OPindexSize indSize, OPuint vertCount, OPuint indCount, void* vertices, void* indices);
	void Render();
};