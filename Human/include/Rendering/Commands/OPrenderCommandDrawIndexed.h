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

struct OPrenderCommandDrawIndexed;

#include "./Human/include/Rendering/OPrenderCommandBucket.h"

//-----------------------------------------------------------------------------
// ______                _   _
//|  ____|              | | (_)
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

void OPrenderCommandDrawIndexedSet(OPrenderCommandDrawIndexed* result, OPmodel* model, OPmaterialInstance* material);
void OPrenderCommandDrawIndexedSubmit(OPrenderCommandBucket* commandBucket, OPmodel* model, OPmaterialInstance* material);
inline void OPrenderCommandDrawIndexedSubmit(OPrenderCommandBucket* commandBucket, OPmodelTextured* model, OPmaterialInstance* material) {
	OPrenderCommandDrawIndexedSubmit(commandBucket, &model->model, material);
}

//-----------------------------------------------------------------------------
//   _____ _                   _
//  / ____| |                 | |
// | (___ | |_ _ __ _   _  ___| |_ ___
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/

// Draws a mesh that has both a vertex buffer and an index buffer
struct OPrenderCommandDrawIndexed
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

	OPvertexArray* vertexArray;
	OPvertexBuffer* vertexBuffer;
	OPindexBuffer* indexBuffer;
	OPmaterialInstance* material;
    
	// Per Mesh Data
    OPmat4* world;

	// Helper/Wrapper functions
    inline OPrenderCommandDrawIndexed* Set(OPmodel* model, OPmaterialInstance* material) {
        OPrenderCommandDrawIndexedSet(this, model, material);
		return this;
    }
};

#endif