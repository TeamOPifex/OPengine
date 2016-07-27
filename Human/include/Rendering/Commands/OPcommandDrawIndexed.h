#ifndef OPENGINE_HUMAN_RENDERING_COMMAND_DRAW_INDEX
#define OPENGINE_HUMAN_RENDERING_COMMAND_DRAW_INDEX

#include "./Human/include/Rendering/OPmodel.h"


//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _
// |  __ \                   |  __ \                |  __ \(_)             | | (_)
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/

struct OPcommandDrawIndexed;
typedef struct OPcommandDrawIndexed OPcommandDrawIndexed;


#include "./Human/include/Rendering/OPcommandBucket.h"

//-----------------------------------------------------------------------------
// ______                _   _
//|  ____|              | | (_)
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

void OPcommandDrawIndexedSet(OPcommandDrawIndexed* result, OPmodel* model, OPmaterial* material);
void OPcommandDrawIndexedSet(OPcommandDrawIndexed* result, OPmodel* model, OPmaterial* material, OPtexture* texture);
void OPcommandDrawIndexedSubmit(OPcommandBucket* commandBucket, OPmodel* model, OPmaterial* material, OPtexture* texture);
inline void OPcommandDrawIndexedSubmit(OPcommandBucket* commandBucket, OPmodelTextured* model, OPmaterial* material) {
	OPcommandDrawIndexedSubmit(commandBucket, &model->model, material, model->texture);
}

//-----------------------------------------------------------------------------
//   _____ _                   _
//  / ____| |                 | |
// | (___ | |_ _ __ _   _  ___| |_ ___
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/

// Draws a mesh that has both a vertex buffer and an index buffer
struct OPcommandDrawIndexed
{
	// The key that will be passed to the OPcommandBucket
    ui64 key;
	// The function that will handle the rendering
    void(*dispatch)(void*, OPcam*);
    
	// Draw Call Packet
	// ================
    ui32 indexCount;
    ui32 startIndex;
    ui32 baseVertex;

    ui32 stride; // TODO: (garrett) Stride should come out of the vertex layout
    OPvertexLayout* vertexLayout;
	//OPrenderBuffer* vertexBuffer;
	OPvertexBuffer* vertexBuffer;
	OPindexBuffer* indexBuffer;
	//OPrenderBuffer* indexBuffer;
    OPmaterial* material;
    
	// Per Mesh Data
    OPmat4* world;
    OPtexture* texture;


	// Helper/Wrapper functions
    inline void Set(OPmodel* model, OPmaterial* material) {
        OPcommandDrawIndexedSet(this, model, material);
    }
    
	inline void Set(OPmodel* model, OPmaterial* material, OPtexture* texture) {
        OPcommandDrawIndexedSet(this, model, material, texture);
    }
};

#endif