#include "./Human/include/Rendering/Commands/OPcommandDrawIndexed.h"
#include "./Human/include/Utilities/Errors.h"

void OPcommandDrawIndex(void* data, OPcam* camera) {
	OPcommandDrawIndexed* dc = (OPcommandDrawIndexed*)data;

	dc->vertexBuffer->Bind();
	dc->indexBuffer->Bind();
	//OPrenderBindBuffer(dc->vertexBuffer);
	//OPrenderBindBuffer(dc->indexBuffer);
	OPmaterialBind(dc->material);// , dc->stride);
	OPeffectSet(camera);

	// Per mesh Shader Data
	OPeffectSet("uWorld", dc->world);

	OPmeshRender();
}

void OPcommandDrawIndexTexture(void* data, OPcam* camera) {
	OPcommandDrawIndexed* dc = (OPcommandDrawIndexed*)data;

	
	dc->vertexBuffer->Bind();
	dc->indexBuffer->Bind();
	dc->material->Bind();

	OPeffectSet(camera);


	OPeffectSet("uColorTexture", dc->texture);

	// Per mesh Shader Data
	OPeffectSet("uWorld", dc->world);

	OPmeshRender();
}

void OPcommandDrawIndexedSet(OPcommandDrawIndexed* result, OPmodel* model, OPmaterial* material) {

	ui64 meshId = model->mesh->Id << 0;     // 00 - 06 bits
	ui64 materialId = material->id << 12;   // 13 - 19 bits
	ui64 renderTarget = 0 << 18;            // 20 - 26 bits
	result->key = meshId | materialId | renderTarget;

	result->startIndex = 0;
	result->baseVertex = 0;
	result->material = material;
	result->indexCount = model->mesh->IndexCount;
	result->vertexLayout = NULL;
	result->vertexBuffer = &model->mesh->vertexBuffer;
	//result->vertexBuffer = &model->mesh->VertexBuffer;
	result->indexBuffer = &model->mesh->indexBuffer;
	//result->indexBuffer = &model->mesh->IndexBuffer;
	result->world = &model->world;
	result->stride = model->mesh->vertexLayout.stride;
	result->dispatch = OPcommandDrawIndex;
}

void OPcommandDrawIndexedSet(OPcommandDrawIndexed* result, OPmodel* model, OPmaterial* material, OPtexture* texture) {
	
	ui64 meshId = model->mesh->Id << 0;     // 00 - 06 bits
	ui64 textureId = texture->guid << 6;  // 07 - 12 bits
	ui64 materialId = material->id << 12;   // 13 - 19 bits
	ui64 renderTarget = 0 << 18;            // 20 - 26 bits
	result->key = meshId | textureId | materialId | renderTarget;

	result->startIndex = 0;
	result->baseVertex = 0;
	result->material = material;
	result->indexCount = model->mesh->IndexCount;
	result->vertexLayout = NULL;
	result->vertexBuffer = &model->mesh->vertexBuffer;
	//result->vertexBuffer = &model->mesh->VertexBuffer;
	result->indexBuffer = &model->mesh->indexBuffer;
	//result->indexBuffer = &model->mesh->IndexBuffer;
	result->world = &model->world;
	result->texture = texture;
	result->stride = model->mesh->vertexLayout.stride;
	result->dispatch = OPcommandDrawIndexTexture;
}

void OPcommandDrawIndexedSubmit(OPcommandBucket* commandBucket, OPmodel* model, OPmaterial* material, OPtexture* texture) {
	OPcommandDrawIndexed* dc = commandBucket->CreateDrawIndexed();
	dc->Set(model, material, texture);
	commandBucket->Submit(dc->key, dc->dispatch, dc);
}
