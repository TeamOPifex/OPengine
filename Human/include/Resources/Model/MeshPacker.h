#pragma once
#include "./Core/include/Types.h"
#include "./Data/include/OPstream.h"
#include "./Human/include/Resources/Buffer/Buffer.h"

class MeshPacker {
public:
	MeshPacker();
	~MeshPacker();

	ui32 AddVertexBuffer(ui32 vertexSize, void* verticesData, ui32 vertexCount);
	ui32 AddIndexBuffer(ui32 indexSize, void* indicesData, ui32 indexCount);
	void Build();

	BufferPtr VertexBufferPtr;
	BufferPtr IndexBufferPtr;
private:
	ui32 vertexOffset;
	ui32 indexOffset;
	OPstream* vertices;
	OPstream* indices;
};