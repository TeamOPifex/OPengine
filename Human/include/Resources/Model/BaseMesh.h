#pragma once
#include "./Core/include/Types.h"

class BaseMesh {
public:
	BaseMesh() {}
	BaseMesh(ui32 count, ui32 stride);
	void Init(ui32 count, ui32 stride);
	ui32 IndexCount;
	ui32 Stride;
};

typedef BaseMesh* BaseMeshPtr;