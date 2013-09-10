#include "./Human/include/Resources/Model/Mesh.h"

Mesh::Mesh(BufferPtr vb, BufferPtr ib, ui32 count, ui32 stride) : BaseMesh(count, stride) {
	VertexBuffer = vb;
	IndexBuffer = ib;
}