#include "./Human/include/Rendering/OPmeshPacked.h"
#include "./Human/include/Rendering/Enums/OPindexSize.h"

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//-----------------------------------------------------------------------------

OPmeshPacked* OPmeshPacked::Init(
			OPvertexLayout vertexLayout, OPindexSize indSize,
			OPuint vertCount, OPuint indCount,
			void* vertices, void* indices){

	OPmeshPacker* packer = OPMESHPACKER_ACTIVE;

	offset = packer->indexOffset;
	count = indCount;
	elementCount = indCount;
	this->vertexLayout = vertexLayout;

	packer->vertexArray.Init(&vertexLayout);
	packer->AddVertexBuffer(vertexLayout.stride, vertices, vertCount);
	packer->AddIndexBuffer(indSize, indices, indCount);
	packer->vertexElementOffset += vertCount;
	
	return this;
}

OPmeshPacked* OPmeshPacked::Create(OPvertexLayout vertexLayout, OPindexSize indSize, OPuint vertCount, OPuint indCount, void* vertices, void* indices) {
	OPmeshPacked* result = (OPmeshPacked*)OPalloc(sizeof(OPmeshPacked));
	result->Init(vertexLayout, indSize, vertCount, indCount, vertices, indices);
	return result;
}

//-----------------------------------------------------------------------------
void OPmeshPacked::Render(){
	OPmeshPacker* packer = OPMESHPACKER_ACTIVE;
	packer->vertexArray.Bind();
	packer->VertexBuffer.Bind();
	packer->IndexBuffer.Bind();
	OPRENDERER_ACTIVE->VertexArray.DrawIndexed(&packer->vertexArray, elementCount, offset);
	//glDrawElements(GL_TRIANGLES, (GLsizei)elementCount, GL_UNSIGNED_SHORT, (void*)(offset));
}
