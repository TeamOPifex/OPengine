#include "./Human/include/Rendering/OPmeshPacked.h"

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//-----------------------------------------------------------------------------

OPmeshPacked OPmeshPackedCreate(
			ui32 vertSize, ui32 indSize,
			ui32 vertCount, ui32 indCount,
			void* vertices, void* indices){
	OPmeshPacker* packer = OPMESHPACKER_ACTIVE;

	OPmeshPacked out = {
		packer->indexOffset,
		indCount,
		indCount
	};

	OPmeshPackerAddVB(vertSize, vertices, vertCount);
	OPmeshPackerAddIB(indSize, indices, indCount);
	packer->vertexElementOffset += vertCount;
	
	return out;
}
//-----------------------------------------------------------------------------
void OPmeshPackedRender(OPmeshPacked* mesh){
		glDrawElements(GL_TRIANGLES, mesh->elementCount, GL_UNSIGNED_SHORT, (void*)(mesh->offset));
}
