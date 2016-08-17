#pragma once

struct OPvertexArray;
typedef struct OPvertexArray OPvertexArray;

#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/OPvertexLayout.h"

struct OPvertexArray {
	void* internalPtr;
	//OPvertexLayout* vertexLayout;

	inline OPvertexArray* Init(OPvertexLayout* vertexLayout) {
		return OPRENDERER_ACTIVE->VertexArray.Init(this, vertexLayout);
	}

	inline void Bind() {
		OPRENDERER_ACTIVE->VertexArray.Bind(this);
	}

	inline void SetLayout(OPvertexLayout* vertexLayout) {
		OPRENDERER_ACTIVE->VertexArray.SetLayout(this, vertexLayout);
	}

	inline void Destroy() {
		OPRENDERER_ACTIVE->VertexArray.Destroy(this);
	}
};