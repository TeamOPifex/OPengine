#include "./Human/include/Rendering/OPrenderCommandBucket.h"


OPrenderCommandDrawIndexed* OPrenderCommandBucket::CreateDrawIndexed() {
	return (OPrenderCommandDrawIndexed*)allocator->alloc(allocator, sizeof(OPrenderCommandDrawIndexed));
}

void OPrenderCommandBucket::Init(OPuint bucketSize, OPcam** camera) {
	internalAllocator = OPallocatorLinear::Create(KB(bucketSize));
	Init(bucketSize, camera, internalAllocator->_rootAlloc);
}

void OPrenderCommandBucket::Init(OPuint bucketSize, OPcam** camera, OPallocator* allocator) {

	this->bucketSize = bucketSize;
	this->camera = camera;

	keys = (OPrenderCommandBucketKey*)OPalloc(sizeof(OPrenderCommandBucketKey) * bucketSize);
	copykeys = (OPrenderCommandBucketKey*)OPalloc(sizeof(OPrenderCommandBucketKey) * bucketSize);

	commands = (OPrenderCommand*)OPalloc(sizeof(OPrenderCommand) * bucketSize);
	keyIndex = 0;

	this->allocator = allocator;
}

void OPrenderCommandBucket::Destroy() {
	if (internalAllocator != NULL) {
		internalAllocator->Destroy();
		OPfree(internalAllocator);
	}
	OPfree(keys);
	OPfree(copykeys);
	OPfree(commands);
}

OPrenderCommandBucket* OPrenderCommandBucket::Create(OPuint bucketSize, OPcam** camera, OPallocator* allocator) {
	OPrenderCommandBucket* result = (OPrenderCommandBucket*)OPalloc(sizeof(OPrenderCommandBucket));
	result->Init(bucketSize, camera, allocator);
	return result;
}

OPrenderCommandBucket* OPrenderCommandBucket::Create(OPuint bucketSize, OPcam** camera) {
	OPrenderCommandBucket* result = (OPrenderCommandBucket*)OPalloc(sizeof(OPrenderCommandBucket));
	result->Init(bucketSize, camera);
	return result;
}


i64 OPrenderCommandBucketSortGetKey(void* data, i64 index) {
	OPrenderCommandBucketKey* keys = (OPrenderCommandBucketKey*)data;
	return keys[index].key;
}

void* OPrenderCommandBucketSortGetEnitity(void* data, i64 index) {
	OPrenderCommandBucketKey* keys = (OPrenderCommandBucketKey*)data;
	return &keys[index];
}

void OPrenderCommandBucketSortSetEntity(void* data, i64 index, void* entity) {
	OPrenderCommandBucketKey* keys = (OPrenderCommandBucketKey*)data;
	OPrenderCommandBucketKey* key = (OPrenderCommandBucketKey*)entity;

	keys[index].key = key->key;
	keys[index].command = key->command;
}

void OPrenderCommandBucket::Sort() {
	// Sort the keys from lower order to higher order
	OPradixSort(
		keys,
		copykeys,
		keyIndex,
		OPrenderCommandBucketSortGetKey,
		OPrenderCommandBucketSortGetEnitity,
		OPrenderCommandBucketSortSetEntity);
}

void OPrenderCommandBucket::Flush(bool keep) {

	for (ui64 i = 0; i < keyIndex; i++) {
		keys[i].command->dispatch(keys[i].command->data, *camera);
	}

	keyIndex = 0;
	if (keep) return;
	allocator->clear(allocator);
}

void OPrenderCommandBucket::Render() {
	Sort();
	Flush();
}

void OPrenderCommandBucket::Submit(ui64 key, void(*dispatch)(void*, OPcam*), void* data, void* next) {
	commands[keyIndex].data = data;
	commands[keyIndex].dispatch = dispatch;
	commands[keyIndex].next = next;
	keys[keyIndex].key = key;
	keys[keyIndex].command = &commands[keyIndex];

	keyIndex++;
}

void OPrenderCommandBucket::Submit(OPmodel* model, OPmaterialInstance* material) {
	
	OPrenderCommandDrawIndexed::Submit(this, model, material);
}




// ui64 OPCOMMAND_BUCKET_DEFAULT_KEY_GEN(OPcommandBucket* commandBucket, OPmodel* model, OPtexture* texture, OPmaterial* material) {
//     ui64 meshId = model->mesh->Id << 0;     // 00 - 06 bits
//     ui64 textureId = texture->Handle << 6;  // 07 - 12 bits
//     ui64 materialId = material->id << 12;   // 13 - 19 bits
//     ui64 renderTarget = 0 << 18;            // 20 - 26 bits
//     return meshId | textureId | materialId | renderTarget;
// }
