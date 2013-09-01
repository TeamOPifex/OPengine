#pragma once
#include "./Core/include/Types.h"
#include "./Human/include/Resources/Buffer/Buffer.h"
#include "./Human/include/Math/Vector3.h"
#include "./Human/include/Resources/Model/BaseMesh.h"

class Mesh : public BaseMesh {
public:
	Mesh(BufferPtr vb, BufferPtr ib, ui32 count, ui32 stride);
	BufferPtr VertexBuffer;
	BufferPtr IndexBuffer;
};

typedef Mesh* MeshPtr;