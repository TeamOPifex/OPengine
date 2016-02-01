#ifndef OPENGINE_HUMAN_RENDERING_COMMAND_BUCKET
#define OPENGINE_HUMAN_RENDERING_COMMAND_BUCKET

struct OPvertexLayout {
    
};
typedef OPvertexLayout OPvertexLayout;



struct OPcommandDraw
{
    ui32 vertexCount;
    ui32 startVertex;
    
    OPvertexLayout* vertexLayout;
    OPvertexBuffer* vertexBuffer;
    OPindexBuffer* indexBuffer;
};

struct OPcommandDrawIndexed
{
    ui32 indexCount;
    ui32 startIndex;
    ui32 baseVertex;
    
    OPvertexLayout* vertexLayout;
    OPvertexBuffer* vertexBuffer;
    OPindexBuffer* indexBuffer;
};

struct OPcommandBucketSort {
    OPuint keySize;
    void(sort*)(void** data);
};

struct OPcommandBucket {
    OPcommandBucketSort sort;
    OPuint bucketSize;
    OPallocLinear allocator;
    OPcam* camera;
    
    OPframeBuffer* frameBuffer1;
    OPframeBuffer* frameBuffer2;
    OPframeBuffer* frameBuffer3;
    OPframeBuffer* frameBuffer4;
    
    void* keys;
    void** data;
    
    
};
typedef OPcommandBucket OPcommandBucket;


inline void OPcommandBucketInit(
    OPcommandBucket* commandBucket, 
    OPcommandBucketSort sort,
    OPuint bucketSize,
    OPframeBuffer* frameBuffer1,
    OPframeBuffer* frameBuffer2,
    OPframeBuffer* frameBuffer3,
    OPframeBuffer* frameBuffer4,
    OPcam* camera) {
        
    commandBucket->sort = sort;
    commandBucket->bucketSize = bucketSize;
    commandBucket->frameBuffer1 = frameBuffer1;
    commandBucket->frameBuffer2 = frameBuffer2;
    commandBucket->frameBuffer3 = frameBuffer3;
    commandBucket->frameBuffer4 = frameBuffer4;
    commandBucket->camera = camera;
    
    OPallocLinearInit(&commandBucket->allocator, MB(4));
}

inline void OPcommandBucketAdd(OPcommandBucket* commandBucket, OPuint key, OPuint commandSize) {
    commandBucket->
    
    // AddKey
    // AddData
    
}

void OPcommandBucketSort();
void OPcommandBucketFlush();

// struct OPcommandCopyConstantBufferData
// {
//     ConstantBufferHandle constantBuffer;
//     void* data;
//     ui32 size;
// };

#endif