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
OPmeshPacker* OPMESHPACKER_ACTIVE = NULL;

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPmeshPacker* OPmeshPacker::Init(){
	vertices = OPstream::Create(0);
	indices = OPstream::Create(0);
	vertexElementOffset = 0;
	built = false;
	return this;
}

OPmeshPacker* OPmeshPacker::Create() {
	OPmeshPacker* result = (OPmeshPacker*)OPalloc(sizeof(OPmeshPacker));
	result->Init();
	return result;
}

//-----------------------------------------------------------------------------
OPint OPmeshPacker::Destroy(){
	vertices->Destroy();
	indices->Destroy();
	OPfree(vertices);
	OPfree(indices);
	return 1;
}

//-----------------------------------------------------------------------------
OPuint OPmeshPacker::AddVertexBuffer(ui32 vertexSize, void* verticesData, OPuint vertexCount){
	OPmeshPacker* packer = OPMESHPACKER_ACTIVE;
	OPuint dataStartPos = packer->vertexOffset;
	OPuint vertexBufferSize = vertexSize * vertexCount;
	packer->vertices->Write(verticesData, vertexBufferSize);
	packer->vertexOffset += vertexBufferSize;

	return dataStartPos;
}

//-----------------------------------------------------------------------------
OPuint OPmeshPacker::AddIndexBuffer(OPindexSize indexSize, void* indicesData, OPuint indexCount){
	OPmeshPacker* packer = OPMESHPACKER_ACTIVE;
	OPuint dataStartPos = packer->vertexOffset;
	OPuint indexBufferSize = (ui32)indexSize * indexCount;

	ui16* dat = (ui16*)indicesData;
	OPuint offset = packer->vertexElementOffset;
	for(OPuint i = 0; i < indexCount; ++i){
		ui16 index = dat[i] + (ui16)offset;
		packer->indices->Write(&index, (ui32)indexSize);
	}

	packer->indexOffset += indexBufferSize;
	return dataStartPos;
}

//-----------------------------------------------------------------------------
void OPmeshPacker::Build(){
	VertexBuffer.Init();
	IndexBuffer.Init();

	VertexBuffer.Bind();
	IndexBuffer.Bind();

	VertexBuffer.SetData(1, vertexOffset, vertices->Data);
	IndexBuffer.SetData(OPindexSize::BYTE, indexOffset, indices->Data);
		
	built = true;
}

//-----------------------------------------------------------------------------
void OPmeshPacker::Bind(){
	OPMESHPACKER_ACTIVE = this;
	if (!built) return;

	VertexBuffer.Bind();
	IndexBuffer.Bind();
}