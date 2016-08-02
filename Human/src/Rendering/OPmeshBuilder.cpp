#include "./Human/include/Rendering/OPmeshBuilder.h"

void OPmeshBuilder::Init(OPvertexLayout vertexLayout) {
	VertexLayout = vertexLayout;
	Vertices = OPvectorCreate(vertexLayout.stride);
	Indices = OPvectorCreate(sizeof(ui16));
}

OPmeshBuilder* OPmeshBuilder::Create(OPvertexLayout vertexLayout) {
	OPmeshBuilder* result = (OPmeshBuilder*)OPalloc(sizeof(OPmeshBuilder));
	result->Init(vertexLayout);
	return result;
}

void OPmeshBuilder::Destroy() {
	OPvectorDestroy(Vertices);
	OPfree(Vertices);
	OPvectorDestroy(Indices);
	OPfree(Indices);
}

void OPmeshBuilder::Add(void* one, void* two, void* three) {

	ui16 ind0, ind1, ind2;
	ind0 = (ui16)Vertices->_size;
	ind1 = ind0 + 1;
	ind2 = ind0 + 2;
	OPlog("Triangle %d, %d, %d", ind0, ind1, ind2);

	OPvectorPush(Vertices, (ui8*)one);
	OPvectorPush(Vertices, (ui8*)two);
	OPvectorPush(Vertices, (ui8*)three);

	OPvectorPush(Indices, (ui8*)&ind0);
	OPvectorPush(Indices, (ui8*)&ind1);
	OPvectorPush(Indices, (ui8*)&ind2);
}

void OPmeshBuilder::Add(void* one, void* two, void* three, void* four) {

	ui16 ind0, ind1, ind2, ind3, ind4, ind5;
	ind0 = (ui16)Vertices->_size;
	ind1 = ind0 + 1;
	ind2 = ind0 + 2;
	ind3 = ind0;
	ind4 = ind0 + 2;
	ind5 = ind0 + 3;

	OPvectorPush(Vertices, (ui8*)one);
	OPvectorPush(Vertices, (ui8*)two);
	OPvectorPush(Vertices, (ui8*)three);
	OPvectorPush(Vertices, (ui8*)four);

	OPvectorPush(Indices, (ui8*)&ind0);
	OPvectorPush(Indices, (ui8*)&ind1);
	OPvectorPush(Indices, (ui8*)&ind2);
	OPvectorPush(Indices, (ui8*)&ind3);
	OPvectorPush(Indices, (ui8*)&ind4);
	OPvectorPush(Indices, (ui8*)&ind5);
}

void OPmeshBuilder::Add(void** vertices, ui16 vertCount, ui16* indices, ui16 indCount) {
	ui16 ind, indOffset = (ui16)Vertices->_size;

	for(ui16 i = 0; i < vertCount; i++) {
		OPvectorPush(Vertices, (ui8*)vertices[i]);
	}
	
	for(ui16 i = 0; i < indCount; i++) {
		ind = indices[i] + indOffset;
		OPvectorPush(Indices, (ui8*)&ind);
	}
}

OPmesh OPmeshBuilder::Build() {
	void* verts = Vertices->items;
	void* indicies = Indices->items;

	OPmesh mesh;
	mesh.Init(VertexLayout);
	mesh.Build(
		VertexLayout, OPindexSize::SHORT,
		Vertices->_size, Indices->_size,
		verts, indicies
		);
	return mesh;
}