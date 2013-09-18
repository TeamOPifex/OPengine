#include "./Human/include/Rendering/OPmeshPacked.h"

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//-----------------------------------------------------------------------------
OPmeshPacked OPrenderCreateMeshPacked(
			OPmeshPacker* packer,
			ui32 vertSize, ui32 indSize,
			ui32 vertCount, ui32 indCount,
			void* vertices, void* indices){

	OPmeshPacked out = {
		packer->indexOffset,
		indCount,
		indCount
	};

	OPmeshPackerAddVB(packer, vertSize, vertices, vertCount);
	OPmeshPackerAddIB(packer, indSize, indices, indCount);

	return out;
}
//-----------------------------------------------------------------------------
void OPrenderMeshPacked(OPmeshPacked* mesh){
		glDrawRangeElements(
			GL_TRIANGLES,
			mesh->offset,
			mesh->offset + mesh->elementCount,
			mesh->elementCount,
			GL_UNSIGNED_SHORT,
			(void*)0
		);
}
