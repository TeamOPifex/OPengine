#pragma once
#include "./Core/include/Types.h"
#include "./Human/Resources/Buffer/Buffer.h"
#include "./Human/Math/Vector3.h"

class Mesh {
public:
	Mesh(BufferPtr vb, BufferPtr ib, ui32 count, ui32 stride);
	BufferPtr VertexBuffer;
	BufferPtr IndexBuffer;
	ui32 IndexCount;
	ui32 Stride;
	Vector3* Positions;
	ui32 PositionCount;
};

typedef Mesh* MeshPtr;