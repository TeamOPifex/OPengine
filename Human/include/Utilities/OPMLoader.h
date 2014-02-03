#pragma once
#ifndef OPEngine_Human_Renderer_OPM
#define OPEngine_Human_Renderer_OPM

#include "./Core/include/DynamicMemory.h"
#include "./Core/include/Log.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPhashMap.h"
#include "./Data/include/OPlinkedList.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPmeshPacked.h"
#include "./Human/include/Rendering/OPmeshPacker.h"
#include "./Human/include/Rendering/OPMvertex.h"

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif
 
typedef struct { 
	void* vertices;
	ui32 vertexCount;
	ui32 vertexSize;
	void* indices;
	ui32 indexCount;
	ui32 indexSize;
} OPMData;

struct OPMPartNode_def;
typedef struct OPMPartNode_def OPMPartNode;

struct OPMPartNode_def{
	OPuint From;
	OPuint To;
	OPint ChildCount;
	OPMPartNode* Children;
};

OPint OPMhasFeature(ui32 features, ui32 feature);
void OPMgenerateTangent(OPvec3* tangent, OPMvertex* v1, OPMvertex* v2);
OPMData OPMloadData(OPstream* str);
OPint OPMPartitionedLoad(const OPchar* filename, OPmesh** mesh);
OPint OPMload(const OPchar* filename, OPmesh** mesh);
OPint OPMloadPacked(const OPchar* filename, OPmeshPacked** mesh);
OPint OPMUnload(void* image);

// prevent name mangling if compiling with c++
#ifdef __cplusplus
};
#endif
#endif