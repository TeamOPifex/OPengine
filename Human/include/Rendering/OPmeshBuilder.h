#pragma once

struct OPmeshBuilder;
typedef struct OPmeshBuilder OPmeshBuilder;

#include "./Human/include/Rendering/OPmodel.h"
#include "./Data/include/OPvector.h"

struct OPmeshBuilder {
	OPvertexLayout VertexLayout;
	OPvector* Vertices;
	OPvector* Indices;

	OPmeshBuilder() {

	}

	OPmeshBuilder(OPvertexLayout vertexLayout) {
		Init(vertexLayout);
	}

	OPmeshBuilder* Create(OPvertexLayout vertexSize);
	void Init(OPvertexLayout vertexLayout);
	void Add(void* one, void* two, void* three);
	void Add(void* one, void* two, void* three, void* four);
	void Add(void** vertices, ui16 vertCount, ui16* indices, ui16 indCount);
	OPmodel Build();
	void Destroy();

	inline OPmodel BuildAndDestroy() {
		OPmodel result = Build();
		Destroy();
		return result;
	}
};