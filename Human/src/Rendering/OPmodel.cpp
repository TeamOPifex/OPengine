#include "./Human/include/Rendering/OPmodel.h"

void OPmodel::Init(OPuint meshCount, OPvertexLayout vertexLayout) {
	this->vertexLayout = vertexLayout;
	this->meshCount = meshCount;
	this->meshes = OPALLOC(OPmesh, meshCount);
	vertexArray.Init(&vertexLayout)->Bind();
	vertexBuffer.Init()->Bind();
	indexBuffer.Init()->Bind(); 
	vertexArray.SetLayout(&vertexLayout);
}

void OPmodel::Draw(OPmat4* world, OPmaterial* material, OPcam* cam) {
	material->Bind();
	OPeffectSet(cam);
	for (ui32 i = 0; i < meshCount; i++) {
		meshes[i].Bind();
		meshes[i].Draw();
	}
}

void OPmodel::Bind() {
	vertexArray.Bind();
	vertexBuffer.Bind();
	indexBuffer.Bind();
}

void OPmodel::Build(ui32 vertexCount, ui32 indexCount, OPindexSize indexSize, void* vertices, void* indices) {
	vertexArray.Bind();
#ifdef _DEBUG
	vertexArray.attachedMeshName = name;
#endif
	vertexBuffer.SetData(vertexLayout.stride, vertexCount, vertices);
	indexBuffer.SetData(indexSize, indexCount, indices);
	this->vertexLayout = vertexLayout;
}

void OPmodel::Destroy() {
	indexBuffer.Destroy();
	vertexBuffer.Destroy();
	vertexArray.Destroy();
	OPfree(meshes);
}