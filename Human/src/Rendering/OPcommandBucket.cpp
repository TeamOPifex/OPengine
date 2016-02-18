#include "./Human/include/Rendering/OPcommandBucket.h"


OPcommandDrawIndexed* OPcommandBucketCreateDrawIndexed(OPcommandBucket* commandBucket) {
	return (OPcommandDrawIndexed*)OPallocLinearAlloc(commandBucket->allocator, sizeof(OPcommandDrawIndexed));
}

void OPcommandBucketInit(OPcommandBucket* commandBucket, OPuint bucketSize, OPcam* camera) {

	commandBucket->bucketSize = bucketSize;
	commandBucket->camera = camera;

	commandBucket->keys = (OPcommandBucketKey*)OPalloc(sizeof(OPcommandBucketKey) * bucketSize);
	commandBucket->copykeys = (OPcommandBucketKey*)OPalloc(sizeof(OPcommandBucketKey) * bucketSize);

	commandBucket->commands = (OPcommandDrawCommand*)OPalloc(sizeof(OPcommandDrawCommand) * bucketSize);
	commandBucket->keyIndex = 0;

	commandBucket->allocator = OPallocLinearCreate(KB(1));
}

i64 OPcommandBucketSortGetKey(void* data, i64 index) {
	OPcommandBucketKey* keys = (OPcommandBucketKey*)data;
	return keys[index].key;
}

void* OPcommandBucketSortGetEnitity(void* data, i64 index) {
	OPcommandBucketKey* keys = (OPcommandBucketKey*)data;
	return &keys[index];
}

void OPcommandBucketSortSetEntity(void* data, i64 index, void* entity) {
	OPcommandBucketKey* keys = (OPcommandBucketKey*)data;
	OPcommandBucketKey* key = (OPcommandBucketKey*)entity;

	keys[index].key = key->key;
	keys[index].command = key->command;
}

void OPcommandBucketSortKeys(OPcommandBucket* commandBucket) {
	// Sort the keys from lower order to higher order
	OPradixSort(
		commandBucket->keys,
		commandBucket->copykeys,
		commandBucket->keyIndex,
		OPcommandBucketSortGetKey,
		OPcommandBucketSortGetEnitity,
		OPcommandBucketSortSetEntity);
}

void OPcommandBucketFlush(OPcommandBucket* commandBucket) {

	for (ui64 i = 0; i < commandBucket->keyIndex; i++) {
		commandBucket->keys[i].command->dispatch(commandBucket->keys[i].command->data, commandBucket->camera);
	}

	commandBucket->keyIndex = 0;
	OPallocLinearClear(commandBucket->allocator);
}

void OPcommandBucketSubmit(OPcommandBucket* commandBucket, ui64 key, void(*dispatch)(void*, OPcam*), void* data, void* next) {
	commandBucket->commands[commandBucket->keyIndex].data = data;
	commandBucket->commands[commandBucket->keyIndex].dispatch = dispatch;
	commandBucket->commands[commandBucket->keyIndex].next = next;
	commandBucket->keys[commandBucket->keyIndex].key = key;
	commandBucket->keys[commandBucket->keyIndex].command = &commandBucket->commands[commandBucket->keyIndex];

	commandBucket->keyIndex++;
}

void OPcommandBucket::CreateDrawIndexedSubmit(OPmodel* model, OPmaterial* material, OPtexture* texture) {
	OPcommandDrawIndexedSubmit(this, model, material, texture);
}

void OPcommandBucket::CreateDrawIndexedSubmit(OPmodelTextured* model, OPmaterial* material) {
	OPcommandDrawIndexedSubmit(this, model, material);
}

// ui64 OPCOMMAND_BUCKET_DEFAULT_KEY_GEN(OPcommandBucket* commandBucket, OPmodel* model, OPtexture* texture, OPmaterial* material) {
//     ui64 meshId = model->mesh->Id << 0;     // 00 - 06 bits
//     ui64 textureId = texture->Handle << 6;  // 07 - 12 bits
//     ui64 materialId = material->id << 12;   // 13 - 19 bits
//     ui64 renderTarget = 0 << 18;            // 20 - 26 bits
//     return meshId | textureId | materialId | renderTarget;
// }