#pragma once

#include <stdio.h>
#include "./Human/include/Resources/Model/Mesh.h"
#include "./Human/include/Resources/Model/MeshPacker.h"
#include "./Human/include/Resources/Model/PackedMesh.h"

#ifdef __cplusplus
extern "C" {
#endif
	OPint OPMLoad(const OPchar* filename, Mesh** image);
	OPint OPMUnload(void* image);
#ifdef __cplusplus
}
#endif

Mesh* LoadOPM(FILE* file);
PackedMesh* LoadOPM(FILE* file, MeshPacker* meshPacker);