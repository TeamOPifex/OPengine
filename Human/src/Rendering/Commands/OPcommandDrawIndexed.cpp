#include "./Human/include/Rendering/Commands/OPcommandDrawIndexed.h"
#include "./Human/include/Utilities/Errors.h"

void OPcommandDrawIndex(void* data, OPcam* camera) {
	OPcommandDrawIndexed* dc = (OPcommandDrawIndexed*)data;

	OPrenderBindBuffer(dc->vertexBuffer);
	OPrenderBindBuffer(dc->indexBuffer);
	OPmaterialBind(dc->material, dc->stride);
	OPeffectParam(camera);

	// Per mesh Shader Data
	OPeffectParam("uWorld", *dc->world);

	OPmeshRender();
}

void OPcommandDrawIndexTexture(void* data, OPcam* camera) {
	OPcommandDrawIndexed* dc = (OPcommandDrawIndexed*)data;

	OPglError("ERROR PRIOR TO BUFFER BIND");
	OPrenderBindBuffer(dc->vertexBuffer);
	OPrenderBindBuffer(dc->indexBuffer);

	OPglError("ERROR PRIOR TO MATERIAL BIND");
	OPmaterialBind(dc->material, dc->stride);

	OPglError("ERROR PRIOR TO CAMERA BIND");
	OPeffectParam(camera);

	OPglError("ERROR PRIOR TO WORLD BIND");

	// Per mesh Shader Data
	OPeffectParam("uWorld", *dc->world);

	OPglError("ERROR PRIOR TO CLEAR ACTIVE");

	OPtextureClearActive();
	OPglError("ERROR PRIOR TO TEXTURE BIND");
	ui32 tex = OPtextureBind(dc->texture);
	OPglError("ERROR PRIOR TO COLOR TEX BIND");
	OPeffectParam("uColorTexture", tex);

	OPglError("ERROR PRIOR TO RENDER");

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
	result->vertexBuffer = &model->mesh->VertexBuffer;
	result->indexBuffer = &model->mesh->IndexBuffer;
	result->world = &model->world;
	result->stride = model->mesh->vertexLayout.stride;
	result->dispatch = OPcommandDrawIndex;
}

void OPcommandDrawIndexedSet(OPcommandDrawIndexed* result, OPmodel* model, OPmaterial* material, OPtexture* texture) {
	
	ui64 meshId = model->mesh->Id << 0;     // 00 - 06 bits
	ui64 textureId = texture->Handle << 6;  // 07 - 12 bits
	ui64 materialId = material->id << 12;   // 13 - 19 bits
	ui64 renderTarget = 0 << 18;            // 20 - 26 bits
	result->key = meshId | textureId | materialId | renderTarget;

	result->startIndex = 0;
	result->baseVertex = 0;
	result->material = material;
	result->indexCount = model->mesh->IndexCount;
	result->vertexLayout = NULL;
	result->vertexBuffer = &model->mesh->VertexBuffer;
	result->indexBuffer = &model->mesh->IndexBuffer;
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
