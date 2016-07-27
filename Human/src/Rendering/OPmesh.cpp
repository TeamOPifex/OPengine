#include "./Human/include/Rendering/OPmesh.h"

OPmesh* OPMESH_ACTIVE;
void* OPMESH_ACTIVE_PTR = NULL;
ui64 OPMESH_GLOBAL_ID = 1;

#include "./Human/include/Rendering/OPrender.h"

//-----------------------------------------------------------------------------
// ______                _   _
//|  ____|              | | (_)
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPmesh OPmeshCreate(){
	OPmesh out;

	OPRENDERER_ACTIVE->VertexBuffer.Init(&out.vertexBuffer);
	OPRENDERER_ACTIVE->IndexBuffer.Init(&out.indexBuffer);

	out.Id = OPMESH_GLOBAL_ID++;

	return out;
}

OPmesh* OPmeshCreate(OPmeshDesc desc) {
	OPmesh* mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
    mesh->Id = OPMESH_GLOBAL_ID++;
	(*mesh) = OPmeshCreate();
	mesh->Bind();

	OPmeshBuild(desc.VertexSize, desc.IndexSize, desc.VertexCount, desc.IndexCount, desc.Vertices, desc.Indices);

	return mesh;
}

//-----------------------------------------------------------------------------
void OPmeshBuild(ui32 vertSize, OPindexSize indSize,
						 OPuint vertCount, OPuint indCount,
						 void* vertices, void* indices){
	OPMESH_ACTIVE->indexBuffer.SetData(indSize, indCount, indices);
	OPMESH_ACTIVE->vertexBuffer.SetData(vertSize, vertCount, vertices);

	OPMESH_ACTIVE->Vertices = vertices;
	OPMESH_ACTIVE->Indicies = indices;
}

//-----------------------------------------------------------------------------
void OPmesh::Bind(){
	OPrenderBindBuffer(&vertexBuffer);
	OPRENDERER_ACTIVE->VertexBuffer.SetLayout(&vertexBuffer, &vertexLayout);
	OPrenderBindBuffer(&indexBuffer);

	OPMESH_ACTIVE_PTR = OPMESH_ACTIVE = this;
}

//-----------------------------------------------------------------------------
void OPmeshRender(){
	OPrenderDrawBufferIndexed(0);
}

void OPmeshDestroy(OPmesh* mesh) {
	mesh->vertexBuffer.Destroy();
	mesh->indexBuffer.Destroy();
}
