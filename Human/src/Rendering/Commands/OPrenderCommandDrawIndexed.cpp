#include "./Human/include/Rendering/Commands/OPrenderCommandDrawIndexed.h"

void OPrenderCommandDrawIndex(void* data, OPcam* camera) {
	OPrenderCommandDrawIndexed* dc = (OPrenderCommandDrawIndexed*)data;

	dc->vertexArray->Bind();
	dc->vertexBuffer->Bind();
	dc->indexBuffer->Bind();
	dc->material->Bind();

	OPeffectSet(camera);

	// Per mesh Shader Data
	OPeffectSet("uWorld", dc->world);

	//OPrenderDrawBufferIndexed(dc->startIndex);

	OPRENDERER_ACTIVE->VertexArray.DrawIndexed(OPRENDERER_ACTIVE->OPVERTEXARRAY_ACTIVE, dc->indexCount, dc->startIndex);
	//glDrawElements(GL_TRIANGLES, (GLsizei)OPRENDER_CURR_IB->ElementCount, indType, (void*)(offset * sizeof(GLuint)));
}

OPrenderCommandDrawIndexed* OPrenderCommandDrawIndexed::Set(OPmesh* mesh, OPmat4* world, OPmaterialInstance* material) {
	ui64 meshId = mesh->id << 0;     // 00 - 06 bits
	ui64 materialId = material->id << 12;   // 13 - 19 bits
	ui64 renderTarget = 0 << 18;            // 20 - 26 bits
	key = meshId | materialId | renderTarget;

	this->startIndex = mesh->offset;
	this->baseVertex = 0;
	this->material = material;
	this->indexCount = mesh->count;
	this->vertexArray = mesh->vertexArray;
	this->vertexBuffer = mesh->vertexBuffer;
	this->indexBuffer = mesh->indexBuffer;
	this->world = world;
	this->dispatch = OPrenderCommandDrawIndex;

	return this;
}

void OPrenderCommandDrawIndexed::Submit(OPrenderCommandBucket* commandBucket, OPmodel* model, OPmat4* world, OPmaterialInstance** material, bool materialPerMesh) {
	if (!materialPerMesh) {
		Submit(commandBucket, model, world, (OPmaterialInstance*)material);
		return;
	}

	for (ui32 i = 0; i < model->meshCount; i++) {
		if (!material[i]->rootMaterial->visible || !material[i]->visible) continue;
		OPrenderCommandDrawIndexed* dc = commandBucket->CreateDrawIndexed();
		dc->Set(&model->meshes[i], world, material[i]);
		commandBucket->Submit(dc->key, dc->dispatch, dc);
	}
}

void OPrenderCommandDrawIndexed::Submit(OPrenderCommandBucket* commandBucket, OPmodel* model, OPmat4* world, OPmaterialInstance* material) {
	if (!material->rootMaterial->visible || !material->visible) return;
	for (ui32 i = 0; i < model->meshCount; i++) {
		OPrenderCommandDrawIndexed* dc = commandBucket->CreateDrawIndexed();
		dc->Set(&model->meshes[i], world, material);
		commandBucket->Submit(dc->key, dc->dispatch, dc);
	}
}

void OPrenderCommandDrawIndexed::Submit(OPrenderCommandBucket* commandBucket, OPmesh* mesh, OPmat4* world, OPmaterialInstance* material) {
	if (!material->rootMaterial->visible || !material->visible) return;
	OPrenderCommandDrawIndexed* dc = commandBucket->CreateDrawIndexed();
	dc->Set(mesh, world, material);
	commandBucket->Submit(dc->key, dc->dispatch, dc);
}
