#ifndef OPENGINE_HUMAN_RENDERER_MESHBUILDER
#define OPENGINE_HUMAN_RENDERER_MESHBUILDER

#include "./Human/include/Rendering/OPmesh.h"
#include "./Data/include/OPvector.h"

struct OPmeshBuilder {
	ui16 VertexSize;
	OPvector* Vertices;
	OPvector* Indices;
};

void OPmeshBuilderInit(struct OPmeshBuilder* builder, ui16 vertexSize);
OPmeshBuilder* OPmeshBuilderCreate(ui16 vertexSize);
void OPmeshBuilderDestroy(struct OPmeshBuilder* builder);
void OPmeshBuilderAdd(OPmeshBuilder* builder, void* one, void* two, void* three);
void OPmeshBuilderAdd(OPmeshBuilder* builder, void* one, void* two, void* three, void* four);
OPmesh OPmeshBuilderGen(OPmeshBuilder* builder);

#endif