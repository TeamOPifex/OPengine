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

void OPrenderCommandDrawIndexedSet(OPrenderCommandDrawIndexed* result, OPmodel* model, OPmaterialInstance* material) {

	ui64 meshId = model->mesh->Id << 0;     // 00 - 06 bits
	ui64 materialId = material->id << 12;   // 13 - 19 bits
	ui64 renderTarget = 0 << 18;            // 20 - 26 bits
	result->key = meshId | materialId | renderTarget;

	result->startIndex = 0;
	result->baseVertex = 0;
	result->material = material;
	result->indexCount = model->mesh->IndexCount;
	result->vertexArray = &model->mesh->vertexArray;
	result->vertexBuffer = &model->mesh->vertexBuffer;
	result->indexBuffer = &model->mesh->indexBuffer;
	result->world = &model->world;
	result->dispatch = OPrenderCommandDrawIndex;
}

void OPrenderCommandDrawIndexedSubmit(OPrenderCommandBucket* commandBucket, OPmodel* model, OPmaterialInstance* material) {
	OPrenderCommandDrawIndexed* dc = commandBucket->CreateDrawIndexed();
	dc->Set(model, material);
	commandBucket->Submit(dc->key, dc->dispatch, dc);
}
