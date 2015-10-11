#include "./Human/include/Rendering/OPmesh.h"

OPmesh* OPMESH_ACTIVE;
void* OPMESH_ACTIVE_PTR = NULL;

//-----------------------------------------------------------------------------
// ______                _   _
//|  ____|              | | (_)
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPmesh OPmeshCreate(){
	OPmesh out = {
		OPrenderGenBuffer(OPvertexBuffer),
		OPrenderGenBuffer(OPindexBuffer)
	};

	return out;
}

OPmesh* OPmeshCreate(OPmeshDesc desc) {
	OPmesh* mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
	(*mesh) = OPmeshCreate();
	OPmeshBind(mesh);

	OPmeshBuild(desc.VertexSize, desc.IndexSize, desc.VertexCount, desc.IndexCount, desc.Vertices, desc.Indices);

	return mesh;
}

//-----------------------------------------------------------------------------
void OPmeshBuild(ui32 vertSize, ui32 indSize,
						 ui32 vertCount, ui32 indCount,
						 void* vertices, void* indices){
	OPrenderSetBufferData(&OPMESH_ACTIVE->IndexBuffer, indSize, indCount, indices);
	OPrenderSetBufferData(&OPMESH_ACTIVE->VertexBuffer, vertSize, vertCount, vertices);
	OPMESH_ACTIVE->VertexSize = vertSize;
	OPMESH_ACTIVE->Vertices = vertices;
	OPMESH_ACTIVE->Indicies = indices;
}

//-----------------------------------------------------------------------------
void OPmeshBind(OPmesh* mesh){
	OPrenderBindBuffer(&mesh->VertexBuffer);
	OPrenderBindBuffer(&mesh->IndexBuffer);
	OPMESH_ACTIVE_PTR = OPMESH_ACTIVE = mesh;
}

//-----------------------------------------------------------------------------
void OPmeshRender(){
	OPrenderDrawBufferIndexed(0);
}

void OPmeshDestroy(OPmesh* mesh) {
	OPrenderDelBuffer(&mesh->VertexBuffer);
	OPrenderDelBuffer(&mesh->IndexBuffer);
}
