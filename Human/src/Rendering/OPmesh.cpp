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
OPmesh OPmeshCreate(OPvertexLayout vertexLayout){
	OPmesh out;

	out.vertexLayout = vertexLayout;
	out.vertexArray.Init(&vertexLayout)->Bind();
	out.vertexBuffer.Init()->Bind();
	out.vertexBuffer.SetLayout(&vertexLayout);
	out.indexBuffer.Init()->Bind();

	out.Id = OPMESH_GLOBAL_ID++;

	return out;
}

OPmesh* OPmeshCreate(OPmeshDesc desc) {
	OPmesh* mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
    mesh->Id = OPMESH_GLOBAL_ID++;
	(*mesh) = OPmeshCreate(desc.VertexSize);
	mesh->Bind();

	OPmeshBuild(desc.VertexSize, desc.IndexSize, desc.VertexCount, desc.IndexCount, desc.Vertices, desc.Indices);

	return mesh;
}

void OPmesh::SetVertexLayout(OPvertexLayout* vertexLayoutUpdate) {
	vertexArray.Bind();
	vertexBuffer.Bind();
	vertexLayout = *vertexLayoutUpdate;
	vertexBuffer.SetLayout(&vertexLayout);
}

//-----------------------------------------------------------------------------
void OPmeshBuild(OPvertexLayout vertexLayout, OPindexSize indSize,
						 OPuint vertCount, OPuint indCount,
						 void* vertices, void* indices){
	
	OPMESH_ACTIVE->vertexArray.Bind();
	OPMESH_ACTIVE->indexBuffer.SetData(indSize, indCount, indices);
	OPMESH_ACTIVE->vertexBuffer.SetData(vertexLayout.stride, vertCount, vertices);
	OPMESH_ACTIVE->vertexLayout = vertexLayout;

	OPMESH_ACTIVE->Vertices = vertices;
	OPMESH_ACTIVE->Indicies = indices;
}

//-----------------------------------------------------------------------------
void OPmesh::Bind(){
	OPRENDERER_ACTIVE->VertexArray.Bind(&vertexArray);
	OPrenderBindBuffer(&vertexBuffer);
	//OPRENDERER_ACTIVE->VertexBuffer.SetLayout(&vertexBuffer, &vertexLayout);
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
