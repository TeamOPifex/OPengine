#include "./Human/include/Rendering/OPmodel.h"

void OPmodel::Init(OPuint meshCount, OPvertexLayout vertexLayout) {
	this->vertexLayout = vertexLayout;
	this->meshCount = meshCount;
	this->meshes = OPALLOC(OPmesh, meshCount);
	vertexArray.Init(&vertexLayout)->Bind();
	vertexBuffer.Init()->Bind();
	indexBuffer.Init()->Bind(); 
	vertexArray.SetLayout(&vertexLayout);
	for (ui32 i = 0; i < meshCount; i++) {
		this->meshes[i].vertexArray = &vertexArray;
		this->meshes[i].vertexBuffer = &vertexBuffer;
		this->meshes[i].indexBuffer = &indexBuffer;
	}
}

void OPmodel::Draw(OPmat4* world, OPmaterial* material, OPcam* cam) {
	material->Bind();
	OPeffectSet(cam);
	OPeffectSet("uWorld", world);
	for (ui32 i = 0; i < meshCount; i++) {
		meshes[i].Bind();
		meshes[i].Draw();
	}
}

void OPmodel::Bind() {
	vertexArray.Bind();
	vertexBuffer.Bind();
	if (indexed) {
		indexBuffer.Bind();
	}
}

void OPmodel::Build(ui32 vertexCount, ui32 indexCount, OPindexSize::Enum indexSize, void* vertices, void* indices) {
	vertexArray.Bind();
#ifdef _DEBUG
	vertexArray.attachedMeshName = name;
#endif
	vertexBuffer.SetData(vertexLayout.stride, vertexCount, vertices);
	indexBuffer.SetData(indexSize, indexCount, indices);
	this->vertexLayout = vertexLayout;
	meshes[0].count = indexCount;
	meshes[0].offset = 0;
	meshes[0].materialDesc = NULL;
}

void OPmodel::Build(ui32 vertexCount, void* vertices) {
	indexed = false;
	vertexArray.Bind();
#ifdef _DEBUG
	vertexArray.attachedMeshName = name;
#endif
	vertexBuffer.SetData(vertexLayout.stride, vertexCount, vertices);
	this->vertexLayout = vertexLayout;
	meshes[0].offset = 0;
	meshes[0].materialDesc = NULL;
}


void OPmodel::Destroy() {
	indexBuffer.Destroy();
	vertexBuffer.Destroy();
	vertexArray.Destroy();
	OPfree(meshes);
}