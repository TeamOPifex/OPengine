#ifndef OPENGINE_HUMAN_RENDERER_MESH_PACKER
#define OPENGINE_HUMAN_RENDERER_MESH_PACKER

#include "./Human/include/Rendering/OPrenderBuffer.h"
#include "./Human/include/Rendering/OPvertexArray.h"
#include "./Human/include/Rendering/OPvertexLayout.h"
#include "./Data/include/OPstream.h"
#include "./Core/include/OPmemory.h"

struct OPmeshPacker;
typedef struct OPmeshPacker OPmeshPacker;

extern OPmeshPacker* OPMESHPACKER_ACTIVE;

struct OPmeshPacker{
	OPvertexArray vertexArray;
	OPvertexBuffer VertexBuffer;
	OPindexBuffer IndexBuffer;
	OPuint vertexElementOffset;
	OPuint vertexOffset;
	OPuint indexOffset;
	OPstream vertices;
	OPstream indices;
	OPint built;

	static OPmeshPacker* Create();
	OPmeshPacker* Init(); 
	OPuint AddVertexBuffer(ui32 vertexSize, void* verticesData, OPuint vertexCount);
	OPuint AddIndexBuffer(OPindexSize indexSize, void* indicesData, OPuint indexCount);
	void Build();
	void Bind();
	OPint Destroy();
};

#endif
