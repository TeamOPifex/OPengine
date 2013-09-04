#include "./Human/include/Resources/Model/Mesh.h"

Mesh::Mesh(BufferPtr vb, BufferPtr ib, ui32 count, ui32 stride) : BaseMesh() {
	Init(vb, ib, count, stride);
}

void Mesh::Init(BufferPtr vb, BufferPtr ib, ui32 count, ui32 stride) {
	VertexBuffer = vb;
	IndexBuffer = ib;
	BaseMesh::Init(count, stride);
}