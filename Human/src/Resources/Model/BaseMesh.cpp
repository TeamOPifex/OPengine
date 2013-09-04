#include "./Human/include/Resources/Model/BaseMesh.h"

BaseMesh::BaseMesh(ui32 count, ui32 stride){
	Init(count, stride);
}

void BaseMesh::Init(ui32 count, ui32 stride){
	IndexCount = count;
	Stride = stride;
}