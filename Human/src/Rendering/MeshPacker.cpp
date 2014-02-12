#include "./Human/include/Rendering/MeshPacker.h"
#include "./Human/include/Utilities/Errors.h"

//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
OPmeshPacker* OPRENDER_CURR_PACKER;

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
	packer.vertexElementOffset = 0;
	packer.built = false;

	return packer;
}
//-----------------------------------------------------------------------------
OPint OPmeshPackerDestroy(){
	OPmeshPacker* packer = OPRENDER_CURR_PACKER;
	OPstreamDestroy(&packer->vertices);
	OPstreamDestroy(&packer->indices);
	return 1;
}
//-----------------------------------------------------------------------------
ui32 OPmeshPackerAddVB(ui32 vertexSize, void* verticesData, ui32 vertexCount){
	OPmeshPacker* packer = OPRENDER_CURR_PACKER;
	ui32 dataStartPos = packer->vertexOffset;
	ui32 vertexBufferSize = vertexSize * vertexCount;
	OPwrite(&packer->vertices, verticesData, vertexBufferSize );
	packer->vertexOffset += vertexBufferSize;

	return dataStartPos;
}
//-----------------------------------------------------------------------------
ui32 OPmeshPackerAddIB(ui32 indexSize, void* indicesData, ui32 indexCount){
	OPmeshPacker* packer = OPRENDER_CURR_PACKER;
	ui32 dataStartPos = packer->vertexOffset;
	ui32 indexBufferSize = indexSize * indexCount;

	ui16* dat = (ui16*)indicesData;
	ui32 offset = packer->vertexElementOffset;
	for(OPuint i = 0; i < indexCount; ++i){
		ui16 index = dat[i] + offset;
		OPwrite(&packer->indices, &index, indexSize);
	}

	packer->indexOffset += indexBufferSize;
	return dataStartPos;
}
//-----------------------------------------------------------------------------
void OPmeshPackerBuild(){
	OPmeshPacker* packer = OPRENDER_CURR_PACKER;
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
	OPRENDER_CURR_PACKER->built = true;
}
//-----------------------------------------------------------------------------
void OPmeshPackerBind(OPmeshPacker* packer){
	OPRENDER_CURR_PACKER = packer;
	if (!OPRENDER_CURR_PACKER->built) return;

	CheckError("OPmeshPackerBind:Error 0");
	OPrenderBindBuffer(&packer->VertexBuffer);
	CheckError("OPmeshPackerBind:Error 1");
	OPrenderBindBuffer(&packer->IndexBuffer);
	CheckError("OPmeshPackerBind:Error 2");
}