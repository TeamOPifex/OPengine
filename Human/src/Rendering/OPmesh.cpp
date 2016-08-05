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
OPmesh* OPmesh::Create(OPvertexLayout vertexLayout){
	OPmesh* mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
	mesh->Init(vertexLayout);
	return mesh;
}

OPmesh* OPmesh::Create(OPmeshDesc desc) {
	OPmesh* mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
	mesh->Init(desc);
	return mesh;
}

OPmesh* OPmesh::Init(OPvertexLayout vertexLayout) {
	this->vertexLayout = vertexLayout;
	vertexArray.Init(&vertexLayout)->Bind();
	vertexBuffer.Init()->Bind();
	indexBuffer.Init()->Bind();
	vertexArray.SetLayout(&vertexLayout);
	Id = OPMESH_GLOBAL_ID++;
	return this;
}

OPmesh* OPmesh::Init(OPmeshDesc desc) {
	Init(desc.VertexSize);
	Bind();
	Build(desc.VertexSize, desc.IndexSize, desc.VertexCount, desc.IndexCount, desc.Vertices, desc.Indices);
	return this;
}

void OPmesh::SetVertexLayout(OPvertexLayout* vertexLayoutUpdate) {
	vertexArray.Bind();
	vertexBuffer.Bind();
	indexBuffer.Bind();
	vertexArray.SetLayout(&vertexLayout);
	vertexLayout = *vertexLayoutUpdate;
}

//-----------------------------------------------------------------------------
void OPmesh::Build(OPvertexLayout vertexLayout, OPindexSize indSize,
						 OPuint vertCount, OPuint indCount,
						 void* vertices, void* indices){
	vertexArray.Bind();
	vertexBuffer.SetData(vertexLayout.stride, vertCount, vertices);
	indexBuffer.SetData(indSize, indCount, indices);
	this->vertexLayout = vertexLayout;

	Vertices = vertices;
	Indicies = indices;
}

//-----------------------------------------------------------------------------
void OPmesh::Bind(){
	vertexArray.Bind();
	vertexBuffer.Bind();
	indexBuffer.Bind();
	//OPRENDERER_ACTIVE->VertexArray.Bind(&vertexArray);
	//OPrenderBindBuffer(&vertexBuffer);
	//OPrenderBindBuffer(&indexBuffer);

	OPMESH_ACTIVE_PTR = OPMESH_ACTIVE = this;
}

//-----------------------------------------------------------------------------
void OPmeshRender(){
	OPrenderDrawBufferIndexed(0);
}

void OPmesh::Destroy() {
	vertexBuffer.Destroy();
	indexBuffer.Destroy();
}