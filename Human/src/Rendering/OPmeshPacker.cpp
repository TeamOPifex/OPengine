#include "./Human/include/Rendering/OPmeshPacker.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Utilities/Errors.h"

//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
OPmeshPacker* OPMESHPACKER_ACTIVE;

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
	OPmeshPacker* packer = OPMESHPACKER_ACTIVE;
	OPstreamDestroy(&packer->vertices);
	OPstreamDestroy(&packer->indices);
	return 1;
}
//-----------------------------------------------------------------------------
ui32 OPmeshPackerAddVB(ui32 vertexSize, void* verticesData, ui32 vertexCount){
	OPmeshPacker* packer = OPMESHPACKER_ACTIVE;
	ui32 dataStartPos = packer->vertexOffset;
	ui32 vertexBufferSize = vertexSize * vertexCount;
	OPwrite(&packer->vertices, verticesData, vertexBufferSize );
	packer->vertexOffset += vertexBufferSize;

	return dataStartPos;
}
//-----------------------------------------------------------------------------
ui32 OPmeshPackerAddIB(ui32 indexSize, void* indicesData, ui32 indexCount){
	OPmeshPacker* packer = OPMESHPACKER_ACTIVE;
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
	OPmeshPacker* packer = OPMESHPACKER_ACTIVE;
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
	
	OPMESHPACKER_ACTIVE->built = true;
}
//-----------------------------------------------------------------------------
void OPmeshPackerBind(OPmeshPacker* packer){
	OPMESH_ACTIVE_PTR = OPMESHPACKER_ACTIVE = packer;
	if (!OPMESHPACKER_ACTIVE->built) return;

	OPglError("OPmeshPackerBind:Error 0");
	OPrenderBindBuffer(&packer->VertexBuffer);
	OPglError("OPmeshPackerBind:Error 1");
	OPrenderBindBuffer(&packer->IndexBuffer);
	OPglError("OPmeshPackerBind:Error 2");
}