#include "./Human/include/Rendering/OPmesh.h"

OPmesh* OPRENDER_CURR_MESH;

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPmesh OPrenderCreateMesh(){
	OPmesh out = {
		OPrenderGenBuffer(OPvertexBuffer),
		OPrenderGenBuffer(OPindexBuffer)
	};

	return out;
}
//-----------------------------------------------------------------------------
void OPrenderBuildMesh(ui32 vertSize, ui32 indSize,
						 ui32 vertCount, ui32 indCount,
						 void* vertices, void* indices){
	OPrenderSetBufferData(&OPRENDER_CURR_MESH->IndexBuffer, indSize, indCount, indices);
	OPrenderSetBufferData(&OPRENDER_CURR_MESH->VertexBuffer, vertSize, vertCount, vertices);
}
//-----------------------------------------------------------------------------
void OPrenderBindMesh(OPmesh* mesh){
	OPrenderBindBuffer(&mesh->VertexBuffer);
	OPrenderBindBuffer(&mesh->IndexBuffer);
	OPRENDER_CURR_MESH = mesh;
}
//-----------------------------------------------------------------------------
void OPrenderMesh(){
	OPrenderDrawBufferIndexed(0);
}
