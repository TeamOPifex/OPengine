#include "./Human/include/Rendering/OPcommandBucket.h"


OPcommandDrawIndexed* OPcommandBucket::CreateDrawIndexed() {
	return (OPcommandDrawIndexed*)allocator->alloc(allocator, sizeof(OPcommandDrawIndexed));
}

void OPcommandBucket::Init(OPuint bucketSize, OPcam* camera) {
	controlOfAllocator = 1;
	Init(bucketSize, camera, OPallocatorLinearCreate(KB(bucketSize)));
}

void OPcommandBucket::Init(OPuint bucketSize, OPcam* camera, OPallocator* allocator) {

	bucketSize = bucketSize;
	camera = camera;

	keys = (OPcommandBucketKey*)OPalloc(sizeof(OPcommandBucketKey) * bucketSize);
	copykeys = (OPcommandBucketKey*)OPalloc(sizeof(OPcommandBucketKey) * bucketSize);

	commands = (OPcommandDrawCommand*)OPalloc(sizeof(OPcommandDrawCommand) * bucketSize);
	keyIndex = 0;

	allocator = allocator;
}

OPcommandBucket* OPcommandBucket::Create(OPuint bucketSize, OPcam* camera, OPallocator* allocator) {
	OPcommandBucket* result = (OPcommandBucket*)OPalloc(sizeof(OPcommandBucket));
	result->Init(bucketSize, camera, allocator);
	return result;
}

OPcommandBucket* OPcommandBucket::Create(OPuint bucketSize, OPcam* camera) {
	OPcommandBucket* result = (OPcommandBucket*)OPalloc(sizeof(OPcommandBucket));
	result->Init(bucketSize, camera);
	return result;
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

void OPcommandBucket::Sort() {
	// Sort the keys from lower order to higher order
	OPradixSort(
		keys,
		copykeys,
		keyIndex,
		OPcommandBucketSortGetKey,
		OPcommandBucketSortGetEnitity,
		OPcommandBucketSortSetEntity);
}

void OPcommandBucket::Flush() {

	for (ui64 i = 0; i < keyIndex; i++) {
		keys[i].command->dispatch(keys[i].command->data, camera);
	}

	keyIndex = 0;
	allocator->clear(allocator);
}

void OPcommandBucket::Render() {
	Sort();
	Flush();
}

void OPcommandBucket::Submit(ui64 key, void(*dispatch)(void*, OPcam*), void* data, void* next) {
	commands[keyIndex].data = data;
	commands[keyIndex].dispatch = dispatch;
	commands[keyIndex].next = next;
	keys[keyIndex].key = key;
	keys[keyIndex].command = &commands[keyIndex];

	keyIndex++;
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
