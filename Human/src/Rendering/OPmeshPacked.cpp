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

OPmeshPacked OPmeshPackedCreate(
			OPvertexLayout vertexLayout, OPindexSize indSize,
			OPuint vertCount, OPuint indCount,
			void* vertices, void* indices){
	OPmeshPacker* packer = OPMESHPACKER_ACTIVE;

	OPmeshPacked out;
	out.offset = packer->indexOffset;
	out.count = indCount;
	out.elementCount = indCount;
	out.vertexLayout = vertexLayout;

	OPmeshPackerAddVB(vertexLayout.stride, vertices, vertCount);
	OPmeshPackerAddIB(indSize, indices, indCount);
	packer->vertexElementOffset += vertCount;
	
	return out;
}

//-----------------------------------------------------------------------------
void OPmeshPackedRender(OPmeshPacked* mesh){
	glDrawElements(GL_TRIANGLES, (GLsizei)mesh->elementCount, GL_UNSIGNED_SHORT, (void*)(mesh->offset));
}
