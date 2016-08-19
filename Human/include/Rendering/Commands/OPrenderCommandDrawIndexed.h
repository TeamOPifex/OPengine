#pragma once

struct OPrenderCommandDrawIndexed;
typedef struct OPrenderCommandDrawIndexed OPrenderCommandDrawIndexed;

#include "./Human/include/Rendering/OPrenderCommandBucket.h"
#include "./Human/include/Rendering/OPmodel.h"

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
	OPrenderCommandDrawIndexed* Set(OPmesh* mesh, OPmat4* world, OPmaterialInstance* material);
	static void Submit(OPrenderCommandBucket* commandBucket, OPmodel* model, OPmat4* world, OPmaterialInstance* material);
};