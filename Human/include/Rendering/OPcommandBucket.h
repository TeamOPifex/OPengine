#ifndef OPENGINE_HUMAN_RENDERING_COMMAND_BUCKET
#define OPENGINE_HUMAN_RENDERING_COMMAND_BUCKET

#include "./Data/include/OPradixSort.h"


struct OPcommandDraw
{
    ui32 vertexCount;
    ui32 startVertex;
    
    OPvertexLayout* vertexLayout;
    OPrenderBuffer* vertexBuffer;
    OPrenderBuffer* indexBuffer;
};

struct OPcommandDrawIndexed
{
    ui32 indexCount;
    ui32 startIndex;
    ui32 baseVertex;
    
    ui32 stride;
    OPvertexLayout* vertexLayout;
    OPrenderBuffer* vertexBuffer;
    OPrenderBuffer* indexBuffer;
    OPmaterial* material;
    OPmat4* world;
};

struct OPcommandDrawCommand {
    // void* next;
    void* data;
    void(*dispatch)(void*, OPcam*);
};

struct OPcommandBucket;
inline void OPcommandBucketInit(OPcommandBucket* commandBucket, ui64(*keyGen)(OPcommandBucket*, OPmodel*, OPmaterial*), OPuint bucketSize, OPcam* camera);
inline void OPcommandBucketFlush(OPcommandBucket* commandBucket);
inline OPcommandDrawIndexed* OPcommandBucketDrawIndexed(OPcommandBucket* commandBucket, OPmodel* model, OPmaterial* material);
inline void OPcommandBucketSortKeys(OPcommandBucket* commandBucket);

inline void OPcommandBucketCommandDrawIndex(void* data, OPcam* camera) {
    OPcommandDrawIndexed* dc = (OPcommandDrawIndexed*)data;
    
    OPrenderBindBuffer(dc->vertexBuffer);
    OPrenderBindBuffer(dc->indexBuffer);
    OPmaterialBind(dc->material, dc->stride);
	OPeffectParam(*camera);
    OPeffectParam("uWorld", *dc->world);
	OPmeshRender();
}

struct OPcommandBucketKey {
  ui64 key;
  OPcommandDrawCommand* command;
};

struct OPcommandBucket {
    OPuint bucketSize;
    OPallocLinear* allocator;
        
    ui64 keyIndex;
    OPcommandBucketKey* keys;
    OPcommandBucketKey* copykeys;
    OPcommandDrawCommand* commands;
    
    OPcam* camera;
    OPframeBuffer* frameBuffer[4];
    
    ui64(*keyGen)(OPcommandBucket*, OPmodel*, OPmaterial*);

    
    void Init(ui64(*keyGen)(OPcommandBucket*, OPmodel*, OPmaterial*), OPuint bucketSize, OPcam* camera) {
        OPcommandBucketInit(this, keyGen, bucketSize, camera);
    }
    
    void Flush() {
        OPcommandBucketFlush(this);
    }
    
    OPcommandDrawIndexed* DrawIndexed(OPmodel* model, OPmaterial* material) {
        return OPcommandBucketDrawIndexed(this, model, material);
    }
    
    void Sort() {
        OPcommandBucketSortKeys(this);
    }
};
typedef struct OPcommandBucket OPcommandBucket;


inline void OPcommandBucketInit(
    OPcommandBucket* commandBucket, 
    ui64(*keyGen)(OPcommandBucket*, OPmodel*, OPmaterial*),
    OPuint bucketSize,
    OPcam* camera
    ) {
        
    commandBucket->bucketSize = bucketSize;
    commandBucket->camera = camera;
    
    commandBucket->keys = (OPcommandBucketKey*)OPalloc(sizeof(OPcommandBucketKey) * bucketSize);
    commandBucket->copykeys = (OPcommandBucketKey*)OPalloc(sizeof(OPcommandBucketKey) * bucketSize);
    
    commandBucket->commands = (OPcommandDrawCommand*)OPalloc(sizeof(OPcommandDrawCommand) * bucketSize);
    commandBucket->keyIndex = 0;
    
    commandBucket->keyGen = keyGen;
    
    commandBucket->allocator = OPallocLinearCreate(KB(1));
}

inline OPcommandDrawIndexed* OPcommandBucketDrawIndexed(OPcommandBucket* commandBucket, OPmodel* model, OPmaterial* material) {
    // TODO: Generate Key based on mesh and material
    ui64 key = commandBucket->keyGen(commandBucket, model, material);
    void* resultData = OPallocLinearAlloc(commandBucket->allocator, sizeof(OPcommandDrawIndexed));
    
    commandBucket->commands[commandBucket->keyIndex].data = resultData;
    commandBucket->commands[commandBucket->keyIndex].dispatch = OPcommandBucketCommandDrawIndex;
    commandBucket->keys[commandBucket->keyIndex].key = key;
    commandBucket->keys[commandBucket->keyIndex].command = &commandBucket->commands[commandBucket->keyIndex];
    
    OPcommandDrawIndexed* result = (OPcommandDrawIndexed*)resultData;
    
    result->startIndex = 0;
    result->baseVertex = 0;
    result->material = material;
    result->indexCount = model->mesh->IndexCount;
    result->vertexLayout = NULL;
    result->vertexBuffer = &model->mesh->VertexBuffer;
    result->indexBuffer = &model->mesh->IndexBuffer;
    result->world = &model->world;
    result->stride = model->mesh->VertexSize;
    
    commandBucket->keyIndex++;
    
    return result;
}

inline void OPcommandBucketAdd(OPcommandBucket* commandBucket, OPuint key, OPuint commandSize) {
    //commandBucket->
    
    // AddKey
    // AddData
    
}

inline OPcommandDraw* OPcommandBucketAddDraw(OPcommandBucket* commandBucket) {
    return NULL;
}

inline i64 OPcommandBucketSortGetKey(void* data, i64 index) {
    OPcommandBucketKey* keys = (OPcommandBucketKey*)data;
    return keys[index].key;
}

inline void* OPcommandBucketSortGetEnitity(void* data, i64 index) {
    OPcommandBucketKey* keys = (OPcommandBucketKey*)data;
    return &keys[index];
}

inline void OPcommandBucketSortSetEntity(void* data, i64 index, void* entity) {
    OPcommandBucketKey* keys = (OPcommandBucketKey*)data;
    OPcommandBucketKey* key = (OPcommandBucketKey*)entity;
    
    keys[index].key = key->key;
    keys[index].command = key->command;
}

inline void OPcommandBucketSortKeys(OPcommandBucket* commandBucket) {
    // Sort the keys from lower order to higher order
    OPradixSort(
        commandBucket->keys,
        commandBucket->copykeys,
        commandBucket->keyIndex,
        OPcommandBucketSortGetKey,
        OPcommandBucketSortGetEnitity,
        OPcommandBucketSortSetEntity);
}

inline void OPcommandBucketFlush(OPcommandBucket* commandBucket) {
    
    for(ui64 i = 0; i < commandBucket->keyIndex; i++) {
        commandBucket->keys[i].command->dispatch(commandBucket->keys[i].command->data, commandBucket->camera);
    }
    
    commandBucket->keyIndex = 0;
    OPallocLinearClear(commandBucket->allocator);
}

#endif