#include "./Human/include/Rendering/OPmeshBuilder.h"

void OPmeshBuilderInit(struct OPmeshBuilder* builder, ui16 vertexSize) {
	builder->Vertices = OPvectorCreate(vertexSize);
	builder->Indices = OPvectorCreate(sizeof(ui16));
}

OPmeshBuilder* OPmeshBuilderCreate(ui16 vertexSize) {
	OPmeshBuilder* result = (OPmeshBuilder*)OPalloc(sizeof(OPmeshBuilder));
	OPmeshBuilderInit(result, vertexSize);
	return result;
}

void OPmeshBuilderDestroy(struct OPmeshBuilder* builder) {
	OPvectorDestroy(builder->Vertices);
	OPvectorDestroy(builder->Indices);
}

void OPmeshBuilderAdd(struct OPmeshBuilder* builder, void* one, void* two, void* three) {

	ui16 ind0, ind1, ind2;
	ind0 = builder->Vertices->_size;
	ind1 = ind0 + 1;
	ind2 = ind0 + 2;
	OPlog("Triangle %d, %d, %d", ind0, ind1, ind2);

	OPvectorPush(builder->Vertices, (ui8*)one);
	OPvectorPush(builder->Vertices, (ui8*)two);
	OPvectorPush(builder->Vertices, (ui8*)three);

	OPvectorPush(builder->Indices, (ui8*)&ind0);
	OPvectorPush(builder->Indices, (ui8*)&ind1);
	OPvectorPush(builder->Indices, (ui8*)&ind2);
}

void OPmeshBuilderAdd(struct OPmeshBuilder* builder, void* one, void* two, void* three, void* four) {

	ui16 ind0, ind1, ind2, ind3, ind4, ind5;
	ind0 = builder->Vertices->_size;
	ind1 = ind0 + 1;
	ind2 = ind0 + 2;
	ind3 = ind0;
	ind4 = ind0 + 2;
	ind5 = ind0 + 3;

	OPvectorPush(builder->Vertices, (ui8*)one);
	OPvectorPush(builder->Vertices, (ui8*)two);
	OPvectorPush(builder->Vertices, (ui8*)three);
	OPvectorPush(builder->Vertices, (ui8*)four);

	OPvectorPush(builder->Indices, (ui8*)&ind0);
	OPvectorPush(builder->Indices, (ui8*)&ind1);
	OPvectorPush(builder->Indices, (ui8*)&ind2);
	OPvectorPush(builder->Indices, (ui8*)&ind3);
	OPvectorPush(builder->Indices, (ui8*)&ind4);
	OPvectorPush(builder->Indices, (ui8*)&ind5);
}

void OPmeshBuilderAdd(struct OPmeshBuilder* builder, void** vertices, ui16 vertCount, ui16* indices, ui16 indCount) {
	ui16 ind, indOffset = builder->Vertices->_size;

	for(ui16 i = 0; i < vertCount; i++) {
		OPvectorPush(builder->Vertices, (ui8*)vertices[i]);
	}
	
	for(ui16 i = 0; i < indCount; i++) {
		ind = indices[i] + indOffset;
		OPvectorPush(builder->Indices, (ui8*)&ind);
	}
}

OPmesh OPmeshBuilderGen(struct OPmeshBuilder* builder) {
	void* verts = builder->Vertices->items;
	void* indicies = builder->Indices->items;

	OPmesh mesh = OPmeshCreate();
	OPmeshBind(&mesh);
	OPmeshBuild(
		builder->Vertices->_elementSize, sizeof(ui16),
		builder->Vertices->_size, builder->Indices->_size,
		verts, indicies
		);
	return mesh;
}