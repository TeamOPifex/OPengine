#include "./Human/include/Rendering/OPmeshBuilder.h"

void OPmeshBuilder::Init(OPvertexLayout vertexLayout) {
	VertexLayout = vertexLayout;
	Vertices = OPvector::Create(vertexLayout.stride);
	Indices = OPvector::Create(sizeof(ui16));
}

OPmeshBuilder* OPmeshBuilder::Create(OPvertexLayout vertexLayout) {
	OPmeshBuilder* result = (OPmeshBuilder*)OPalloc(sizeof(OPmeshBuilder));
	result->Init(vertexLayout);
	return result;
}

void OPmeshBuilder::Destroy() {
	Vertices->Destroy();
	OPfree(Vertices);
	Indices->Destroy();
	OPfree(Indices);
}

void OPmeshBuilder::Add(void* one, void* two, void* three) {

	ui16 ind0, ind1, ind2;
	ind0 = (ui16)Vertices->_size;
	ind1 = ind0 + 1;
	ind2 = ind0 + 2;
	OPlog("Triangle %d, %d, %d", ind0, ind1, ind2);

	Vertices->Push((ui8*)one);
	Vertices->Push((ui8*)two);
	Vertices->Push((ui8*)three);

	Indices->Push((ui8*)&ind0);
	Indices->Push((ui8*)&ind1);
	Indices->Push((ui8*)&ind2);
}

void OPmeshBuilder::Add(void* one, void* two, void* three, void* four) {

	ui16 ind0, ind1, ind2, ind3, ind4, ind5;
	ind0 = (ui16)Vertices->_size;
	ind1 = ind0 + 1;
	ind2 = ind0 + 2;
	ind3 = ind0;
	ind4 = ind0 + 2;
	ind5 = ind0 + 3;

	Vertices->Push((ui8*)one);
	Vertices->Push((ui8*)two);
	Vertices->Push((ui8*)three);
	Vertices->Push((ui8*)four);

	Indices->Push((ui8*)&ind0);
	Indices->Push((ui8*)&ind1);
	Indices->Push((ui8*)&ind2);
	Indices->Push((ui8*)&ind3);
	Indices->Push((ui8*)&ind4);
	Indices->Push((ui8*)&ind5);
}

void OPmeshBuilder::Add(void** vertices, ui16 vertCount, ui16* indices, ui16 indCount) {
	ui16 ind, indOffset = (ui16)Vertices->_size;

	for(ui16 i = 0; i < vertCount; i++) {
		Vertices->Push((ui8*)vertices[i]);
	}
	
	for(ui16 i = 0; i < indCount; i++) {
		ind = indices[i] + indOffset;
		Indices->Push((ui8*)&ind);
	}
}

OPmodel OPmeshBuilder::Build() {
	void* verts = Vertices->items;
	void* indicies = Indices->items;

	OPmodel mesh = OPmodel(1, VertexLayout);
	mesh.Build(
		(ui32)Vertices->_size,
		(ui32)Indices->_size, 
		OPindexSize::SHORT,
		verts, indicies
		);
	return mesh;
}