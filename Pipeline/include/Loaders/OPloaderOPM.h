#ifndef OPENGINE_HUMAN_UTILITIES_LOADER_OPM
#define OPENGINE_HUMAN_UTILITIES_LOADER_OPM

#include "./Core/include/OPmemory.h"
#include "./Core/include/OPlog.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPhashMap.h"
#include "./Data/include/OPlinkedList.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPmeshPacked.h"
#include "./Human/include/Rendering/OPmeshPacker.h"
#include "./Human/include/Rendering/OPMvertex.h"

typedef struct { 
	void* vertices;
	ui32 vertexCount;
	ui32 vertexSize;

	void* indices;
	ui32 indexCount;
	ui32 indexSize;

	OPboundingBox3D bounds;

	i16* hierarchy;
	ui16 hierarchyCount;
	OPmat4* pose;

	ui16 metaCount;
	OPMmeta* meta;
} OPMData;

struct OPMPartNode_def;
typedef struct OPMPartNode_def OPMPartNode;

struct OPMPartNode_def{
	OPuint From;
	OPuint To;
	OPint ChildCount;
	OPMPartNode* Children;
};

void OPMgenerateTangent(OPvec3* tangent, OPMvertex* v1, OPMvertex* v2);
OPMData OPMloadData(OPstream* str);
OPint OPMPartitionedLoad(const OPchar* filename, OPmesh** mesh);
OPint OPMload(const OPchar* filename, OPmesh** mesh);
OPint OPMloadPacked(const OPchar* filename, OPmeshPacked** mesh);
OPint OPMReload(const OPchar* filename, OPmesh** mesh);
OPint OPMUnload(void* image);

#endif