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
			ui32 vertSize, ui32 indSize,
			ui32 vertCount, ui32 indCount,
			void* vertices, void* indices){
	OPmeshPacker* packer = OPRENDER_CURR_PACKER;

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
void OPrenderMeshPacked(OPmeshPacked* mesh){
		glDrawElements(GL_TRIANGLES, mesh->elementCount, GL_UNSIGNED_SHORT, (void*)(0 + mesh->offset));
	//glDrawRangeElements(
	//	GL_TRIANGLES,
	//	mesh->offset / 2,
	//	mesh->offset / 2 + mesh->elementCount,
	//	mesh->elementCount,
	//	GL_UNSIGNED_SHORT,
	//	(void*)NULL
	//);

}
