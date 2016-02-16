#ifndef OPENGINE_HUMAN_RENDERING_COMMAND_BUCKET
#define OPENGINE_HUMAN_RENDERING_COMMAND_BUCKET

#include "./Data/include/OPradixSort.h"
#include "./Human/include/Rendering/Commands/OPcommandDrawIndexed.h"

struct OPcommandDraw
{
    ui32 vertexCount;
    ui32 startVertex;
    
    OPvertexLayout* vertexLayout;
    OPrenderBuffer* vertexBuffer;
    OPrenderBuffer* indexBuffer;
};



struct OPcommandDrawCommand {
    // void* next;
    void* data;
    void(*dispatch)(void*, OPcam*);
};



struct OPcommandBucket;
inline void OPcommandBucketInit(OPcommandBucket* commandBucket, OPuint bucketSize, OPcam* camera);
inline void OPcommandBucketFlush(OPcommandBucket* commandBucket);
inline void OPcommandBucketSortKeys(OPcommandBucket* commandBucket);
inline void OPcommandBucketSubmit(OPcommandBucket* commandBucket, ui64 key, void(*dispatch)(void*, OPcam*), void* data);

// ui64 OPCOMMAND_BUCKET_DEFAULT_KEY_GEN(OPcommandBucket* commandBucket, OPmodel* model, OPtexture* texture, OPmaterial* material) {
    
//     ui64 meshId = model->mesh->Id << 0;     // 00 - 06 bits
//     ui64 textureId = texture->Handle << 6;  // 07 - 12 bits
//     ui64 materialId = material->id << 12;   // 13 - 19 bits
//     ui64 renderTarget = 0 << 18;            // 20 - 26 bits
    
//     return meshId | textureId | materialId | renderTarget;
// }

struct OPcommandBucketKey {
  ui64 key;
  OPcommandDrawCommand* command;
};

inline OPcommandDrawIndexed* OPcommandBucketCreateDrawIndexed(OPcommandBucket* commandBucket);

struct OPcommandBucket {
    OPuint bucketSize;
    OPallocLinear* allocator;
        
    ui64 keyIndex;
    OPcommandBucketKey* keys;
    OPcommandBucketKey* copykeys;
    OPcommandDrawCommand* commands;
    
    OPcam* camera;
    OPframeBuffer* frameBuffer[4];
    
    void Init(OPuint bucketSize, OPcam* camera) {
        OPcommandBucketInit(this, bucketSize, camera);
    }
    
    void Flush() {
        OPcommandBucketFlush(this);
    }
    
    void Sort() {
        OPcommandBucketSortKeys(this);
    }
    
    OPcommandDrawIndexed* CreateDrawIndexed() {
        return OPcommandBucketCreateDrawIndexed(this);
    }
    
    void Submit(ui64 key, void(*dispatch)(void*, OPcam*), void* data) {
        OPcommandBucketSubmit(this, key, dispatch, data);
    }
};
typedef struct OPcommandBucket OPcommandBucket;

inline OPcommandDrawIndexed* OPcommandBucketCreateDrawIndexed(OPcommandBucket* commandBucket) {
    return (OPcommandDrawIndexed*)OPallocLinearAlloc(commandBucket->allocator, sizeof(OPcommandDrawIndexed));
}

inline void OPcommandBucketInit(
    OPcommandBucket* commandBucket,
    OPuint bucketSize,
    OPcam* camera
    ) {
        
    commandBucket->bucketSize = bucketSize;
    commandBucket->camera = camera;
    
    commandBucket->keys = (OPcommandBucketKey*)OPalloc(sizeof(OPcommandBucketKey) * bucketSize);
    commandBucket->copykeys = (OPcommandBucketKey*)OPalloc(sizeof(OPcommandBucketKey) * bucketSize);
    
    commandBucket->commands = (OPcommandDrawCommand*)OPalloc(sizeof(OPcommandDrawCommand) * bucketSize);
    commandBucket->keyIndex = 0;
    
    commandBucket->allocator = OPallocLinearCreate(KB(1));
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

inline void OPcommandBucketSubmit(OPcommandBucket* commandBucket, ui64 key, void(*dispatch)(void*, OPcam*), void* data) {
    commandBucket->commands[commandBucket->keyIndex].data = data;
    commandBucket->commands[commandBucket->keyIndex].dispatch = dispatch;
    commandBucket->keys[commandBucket->keyIndex].key = key;
    commandBucket->keys[commandBucket->keyIndex].command = &commandBucket->commands[commandBucket->keyIndex];
    
    commandBucket->keyIndex++;
}

#endif