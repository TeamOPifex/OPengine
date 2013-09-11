#include "./Human/include/Rendering/OPmesh.h"

OPmesh* OPRENDER_CURR_MESH;

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPmesh OPrenderCreateMesh(OPrenderBuffer index, OPrenderBuffer vertex){
	OPmesh out = {
		index,
		vertex
	};

	return out;
}
//-----------------------------------------------------------------------------
OPmesh OPrenderBuildMesh(ui32 vertSize, ui32 indSize,
						 ui32 vertCount, ui32 indCount,
						 void* vertices, void* indices){
	OPmesh mesh = OPrenderCreateMesh(
		OPrenderGenBuffer(OPindexBuffer),
		OPrenderGenBuffer(OPvertexBuffer)
	);

	OPrenderSetBufferData(&mesh.IndexBuffer, indSize, indCount, indices);
	OPrenderSetBufferData(&mesh.VertexBuffer, vertSize, vertCount, vertices);

	return mesh;
}
//-----------------------------------------------------------------------------
void OPrenderBindMesh(OPmesh* mesh){
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VertexBuffer.Handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IndexBuffer.Handle);
	OPRENDER_CURR_MESH = mesh;
}
//-----------------------------------------------------------------------------
void OPrenderMesh(){
	//OPrenderIndexed(0, OPRENDER_CURR_IB->ElementCount);
}
