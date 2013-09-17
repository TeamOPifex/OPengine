#include "./Human/include/Rendering/OPmeshPacker.h"

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPmeshPacker OPmeshPackerCreate(){
	OPmeshPacker packer;
	OPbzero(&packer, sizeof(OPmeshPacker));
	
	packer.vertices = *OPstreamCreate(0);
	packer.indices = *OPstreamCreate(0);

	return packer;
}
//-----------------------------------------------------------------------------
OPint OPmeshPackerDestroy(OPmeshPacker* packer){
	OPstreamDestroy(&packer->vertices);
	OPstreamDestroy(&packer->indices);
	return 1;
}
//-----------------------------------------------------------------------------
ui32 OPmeshPackerAddVB(OPmeshPacker* packer, ui32 vertexSize, void* verticesData, ui32 vertexCount){
	ui32 dataStartPos = packer->vertexOffset;
	ui32 vertexBufferSize = vertexSize * vertexCount;
	OPwrite(&packer->vertices, verticesData, vertexBufferSize );
	packer->vertexOffset += vertexBufferSize;
	return dataStartPos;
}
//-----------------------------------------------------------------------------
ui32 OPmeshPackerAddIB(OPmeshPacker* packer, ui32 indexSize, void* indicesData, ui32 indexCount){
	ui32 dataStartPos = packer->vertexOffset;
	ui32 indexBufferSize = indexSize * indexCount;
	OPwrite(&packer->indices, indicesData, indexSize * indexCount);
	packer->indexOffset += indexBufferSize;
	return dataStartPos;
}
//-----------------------------------------------------------------------------
void OPmeshPackerBuild(OPmeshPacker* packer){
	packer->VertexBuffer = OPrenderGenBuffer(OPvertexBuffer);
	packer->IndexBuffer = OPrenderGenBuffer(OPindexBuffer);

	 OPrenderSetBufferData(
		&packer->VertexBuffer,
		1,
		packer->vertexOffset,
		packer->vertices.Data
	);
	OPrenderSetBufferData(
		&packer->IndexBuffer,
		1,
		packer->indexOffset,
		packer->indices.Data
	);
}