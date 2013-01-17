#pragma once
#include "./Core/include/Types.h"
#include "./Human/Resources/Buffer/Buffer.h"

class Mesh {
public:
	Mesh(BufferPtr vb, BufferPtr ib, ui32 count){
		VertexBuffer = vb;
		IndexBuffer = ib;
		IndexCount = count;
	}
	BufferPtr VertexBuffer;
	BufferPtr IndexBuffer;
	ui32 IndexCount;
};