#pragma once
#include "./Core/include/Types.h"

class BaseMesh {
public:
	BaseMesh(ui32 count, ui32 stride);
	ui32 IndexCount;
	ui32 Stride;
};

typedef BaseMesh* BaseMeshPtr;