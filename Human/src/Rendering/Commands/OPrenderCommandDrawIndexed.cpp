#include "./Human/include/Rendering/Commands/OPrenderCommandDrawIndexed.h"
#include "./Human/include/Utilities/Errors.h"

void OPrenderCommandDrawIndex(void* data, OPcam* camera) {
	OPrenderCommandDrawIndexed* dc = (OPrenderCommandDrawIndexed*)data;

	dc->vertexArray->Bind();
	dc->vertexBuffer->Bind();
	dc->indexBuffer->Bind();
	dc->material->Bind();

	OPeffectSet(camera);

	// Per mesh Shader Data
	OPeffectSet("uWorld", dc->world);

	OPmeshRender();
}

OPrenderCommandDrawIndexed* OPrenderCommandDrawIndexed::Set(OPmodel* model, OPmaterialInstance* material) {

	ui64 meshId = model->mesh->Id << 0;     // 00 - 06 bits
	ui64 materialId = material->id << 12;   // 13 - 19 bits
	ui64 renderTarget = 0 << 18;            // 20 - 26 bits
	key = meshId | materialId | renderTarget;

	startIndex = 0;
	baseVertex = 0;
	this->material = material;
	indexCount = model->mesh->IndexCount;
	vertexArray = &model->mesh->vertexArray;
	vertexBuffer = &model->mesh->vertexBuffer;
	indexBuffer = &model->mesh->indexBuffer;
	world = &model->world;
	dispatch = OPrenderCommandDrawIndex;

	return this;
}

void OPrenderCommandDrawIndexed::Submit(OPrenderCommandBucket* commandBucket, OPmodel* model, OPmaterialInstance* material) {
	OPrenderCommandDrawIndexed* dc = commandBucket->CreateDrawIndexed();
	dc->Set(model, material);
	commandBucket->Submit(dc->key, dc->dispatch, dc);
}
