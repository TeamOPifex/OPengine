#pragma once

#include <stdio.h>
#include "./Human/include/Resources/Model/Mesh.h"
//#include "./Human/include/Resources/Model/MeshPacker.h"
//#include "./Human/include/Resources/Model/PackedMesh.h"
//
typedef struct {
	void* vertices;
	ui32 vertexCount;
	ui32 vertexSize;
	void* indices;
	ui32 indexCount;
	ui32 indexSize;
} OPMData;

OPint OPMhasFeature(ui32 features, ui32 feature);
void OPMgenerateTangent(Vector3* tangent, MeshVertex* v1, MeshVertex* v2);
OPMData OPMloadData(OPstream* str);
OPint OPMload(const OPchar* filename, OPmesh** mesh);
OPint OPMloadPacked(const OPchar* filename, OPmeshPacked** mesh);
OPint OPMUnload(void* image);