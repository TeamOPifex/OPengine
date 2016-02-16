#ifndef OPENGINE_HUMAN_RENDERING_COMMAND_DRAW_INDEX
#define OPENGINE_HUMAN_RENDERING_COMMAND_DRAW_INDEX

struct OPcommandDrawIndexed;
typedef struct OPcommandDrawIndexed OPcommandDrawIndexed;

inline void OPcommandBucketDrawIndexedSet(OPcommandDrawIndexed* result, OPmodel* model, OPmaterial* material);
inline void OPcommandBucketDrawIndexedSet(OPcommandDrawIndexed* result, OPmodel* model, OPmaterial* material, OPtexture* texture);


struct OPcommandDrawIndexed
{
    ui64 key;
    void(*dispatch)(void*, OPcam*);
    
    ui32 indexCount;
    ui32 startIndex;
    ui32 baseVertex;
    
    ui32 stride; // TODO: (garrett) Stride should come out of the vertex layout
    OPvertexLayout* vertexLayout;
    OPrenderBuffer* vertexBuffer;
    OPrenderBuffer* indexBuffer;
    OPmaterial* material;
    
    OPmat4* world;
    OPtexture* texture;
    
    ui64 Key() {
        return key;
    }
    
    void Set(OPmodel* model, OPmaterial* material) {
        OPcommandBucketDrawIndexedSet(this, model, material);
    }
    
    void Set(OPmodel* model, OPmaterial* material, OPtexture* texture) {
        OPcommandBucketDrawIndexedSet(this, model, material, texture);
    }
};

inline void OPcommandBucketCommandDrawIndex(void* data, OPcam* camera) {
    OPcommandDrawIndexed* dc = (OPcommandDrawIndexed*)data;
    
    OPrenderBindBuffer(dc->vertexBuffer);
    OPrenderBindBuffer(dc->indexBuffer);
    OPmaterialBind(dc->material, dc->stride);
	OPeffectParam(*camera);
    OPeffectParam("uWorld", *dc->world);
	OPmeshRender();
}

inline void OPcommandBucketCommandDrawIndexTexture(void* data, OPcam* camera) {
    OPcommandDrawIndexed* dc = (OPcommandDrawIndexed*)data;
    
    OPrenderBindBuffer(dc->vertexBuffer);
    OPrenderBindBuffer(dc->indexBuffer);
    OPmaterialBind(dc->material, dc->stride);
	OPeffectParam(*camera);
    OPeffectParam("uWorld", *dc->world);
    OPtextureClearActive();
    OPeffectParam("uColorTexture", OPtextureBind(dc->texture));
	OPmeshRender();
}

inline void OPcommandBucketDrawIndexedSet(OPcommandDrawIndexed* result, OPmodel* model, OPmaterial* material) {

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
    result->stride = model->mesh->VertexSize;
    result->dispatch = OPcommandBucketCommandDrawIndex;
}

inline void OPcommandBucketDrawIndexedSet(OPcommandDrawIndexed* result, OPmodel* model, OPmaterial* material, OPtexture* texture) {

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
    result->stride = model->mesh->VertexSize;
    result->dispatch = OPcommandBucketCommandDrawIndexTexture;
}
#endif