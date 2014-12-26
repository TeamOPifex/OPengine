#pragma once

#include "./Math/include/OPvec2.h"
#include "./Math/include/OPvec3.h"
#include "./Data/include/OPlist.h"

typedef struct {
	OPlist* vertices;
	OPlist* normals;
	OPlist* uvs;
	OPlist* indices;
	OPlist* boneIndices;
	OPlist* boneWeights;

	i32 VertexCount;
	i32 ColorCount;
	i32 UVCount;
	i32 NormalCount;
	i32 TangentCount;
	i32 IndexCount;

	ui16 IndexPosition;
} OPModelData;

OPModelData* ModelDataCreate() {
	OPModelData* result = (OPModelData*)OPalloc(sizeof(OPModelData));

	result->VertexCount = 0;
	result->ColorCount = 0;
	result->IndexCount = 0;
	result->NormalCount = 0;
	result->TangentCount = 0;
	result->UVCount = 0;
	result->IndexPosition = 0;
	result->vertices = OPlistCreate(128, sizeof(OPvec3));
	result->normals = OPlistCreate(128, sizeof(OPvec3));
	result->uvs = OPlistCreate(128, sizeof(OPvec2));
	result->indices = OPlistCreate(128, sizeof(ui16));
	result->boneIndices = OPlistCreate(128, sizeof(i16));
	result->boneWeights = OPlistCreate(128, sizeof(f32));

	return result;
}