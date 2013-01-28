#include "Mesh.h"

Mesh::Mesh(BufferPtr vb, BufferPtr ib, ui32 count, ui32 stride){
	VertexBuffer = vb;
	IndexBuffer = ib;
	IndexCount = count;
	Stride = stride;
}