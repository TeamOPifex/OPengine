#include "./Human/include/Resources/Model/MeshPacker.h"

MeshPacker::MeshPacker(bool init){
	if(init) Init();
}

void MeshPacker::Init(){
	vertexOffset = 0;
	indexOffset = 0;

	vertices = OPstreamCreate(0);
	indices = OPstreamCreate(0);
}

ui32 MeshPacker::AddVertexBuffer(ui32 vertexSize, void* verticesData, ui32 vertexCount) {
	ui32 dataStartPos = vertexOffset;
	ui32 vertexBufferSize = vertexSize * vertexCount;
	OPwrite(vertices, verticesData, vertexBufferSize );
	vertexOffset += vertexBufferSize;
	return dataStartPos;
}

ui32 MeshPacker::AddIndexBuffer(ui32 indexSize, void* indicesData, ui32 indexCount){
	ui32 dataStartPos = vertexOffset;
	ui32 indexBufferSize = indexSize * indexCount;
	OPwrite(indices, indicesData, indexSize * indexCount);
	indexOffset += indexBufferSize;
	return dataStartPos;
}

void MeshPacker::Build(){
	VertexBufferPtr = new Buffer(VertexBuffer, vertexOffset, vertices->Data);
	IndexBufferPtr = new Buffer(IndexBuffer, indexOffset, indices->Data);
}