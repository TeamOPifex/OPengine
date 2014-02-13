#ifndef OPENGINE_HUMAN_UTILITIES_LOADER_OPM
#define OPENGINE_HUMAN_UTILITIES_LOADER_OPM

#include "./Core/include/DynamicMemory.h"
#include "./Core/include/Log.h"
#include "./Data/include/File.h"
#include "./Data/include/HashMap.h"
#include "./Data/include/LinkedList.h"
#include "./Human/include/Rendering/Mesh.h"
#include "./Human/include/Rendering/MeshPacked.h"
#include "./Human/include/Rendering/MeshPacker.h"
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