#pragma once

#include "./Core/include/OPtypes.h"
#include "ModelData.h"
#include "./Math/include/OPvec2.h"
#include "./Math/include/OPvec3.h"
#include "./Data/include/OPlist.h"
#include "./Core/include/OPlog.h"

enum ModelFeatures {
	Model_Positions = 0,
	Model_Normals = 1,
	Model_UVs = 2,
	Model_Colors = 3,
	Model_Indices = 4,
	Model_Tangents = 5,
	Model_Bones = 6,
	Model_Skinning = 7,
	Model_Animations = 8
};


#include <iostream>
#include <fstream>
using namespace std;

void write(ofstream* stream, void* data, i32 size) {
	stream->write((char*)data, size);
}
void writeI8(ofstream* stream, i8 val) {
	write(stream, &val, sizeof(i8));
}
void writeF32(ofstream* stream, f32 val) {
	write(stream, &val, sizeof(f32));
}
void writeI16(ofstream* stream, i16 val) {
	write(stream, &val, sizeof(i16));
}
void writeU16(ofstream* stream, ui16 val) {
	write(stream, &val, sizeof(ui16));
}
void writeI32(ofstream* stream, i32 val) {
	write(stream, &val, sizeof(i32));
}
void writeU32(ofstream* stream, ui32 val) {
	write(stream, &val, sizeof(ui32));
}

OPint IsParam(char** argv, ui16 pos, i8* arg) {
	return strcmp(arg, argv[pos]) == 0;
}

i8* GetParameter(char** argv, ui16 argp, i8* arg, i8* shrt) {

	if (0 == strcmp(arg, argv[argp]) || 0 == strcmp(shrt, argv[argp]))
	{
		++arg;
		return argv[argp];
	}

	return NULL;
}


#include "SkeletonInformation.h"

void WriteFile(i8* output, OPint* features, OPModelData* ModelData, ModelSkeletonData* skel) {

	ofstream myFile(output, ios::binary);
	OPlog("Begin writing file...");

	// OPM File Format Version
	writeU16(&myFile, 1);

	OPlog("Feature[Bones]: %d", features[Model_Bones]);
	ui32 feature = 0;
	if (features[Model_Positions]) feature += 0x01;
	if (features[Model_Normals]) feature += 0x02;
	if (features[Model_UVs]) feature += 0x04;
	if (features[Model_Colors]) feature += 0x100;
	if (features[Model_Indices]) feature += 0x10;
	if (features[Model_Bones]) {
		OPlog("Features: Bones");
		feature += 0x20;
	}
	if (features[Model_Skinning]) feature += 0x40;
	if (features[Model_Animations]) feature += 0x80;

	// OPM File Features
	writeU32(&myFile, feature);

	// Vertex Count
	writeU32(&myFile, ModelData->VertexCount);

	for (i32 i = 0; i < ModelData->VertexCount; i++) {

		if (features[Model_Positions]) {
			OPvec3* pos = (OPvec3*)OPlistGet(ModelData->vertices, i);
			writeF32(&myFile, pos->x);
			writeF32(&myFile, pos->y);
			writeF32(&myFile, pos->z);
		}

		if (features[Model_Normals]) {
			OPvec3* norm = (OPvec3*)OPlistGet(ModelData->normals, i);
			writeF32(&myFile, norm->x);
			writeF32(&myFile, norm->y);
			writeF32(&myFile, norm->z);
		}

		if (features[Model_UVs]) {
			OPvec2* uv = (OPvec2*)OPlistGet(ModelData->uvs, i);
			writeF32(&myFile, uv->x);
			writeF32(&myFile, uv->y);
		}

		if (features[Model_Skinning]) {
			ui16 ind1 = *(ui16*)OPlistGet(ModelData->boneIndices, i * 4 + 0);
			ui16 ind2 = *(ui16*)OPlistGet(ModelData->boneIndices, i * 4 + 1);
			ui16 ind3 = *(ui16*)OPlistGet(ModelData->boneIndices, i * 4 + 2);
			ui16 ind4 = *(ui16*)OPlistGet(ModelData->boneIndices, i * 4 + 3);

			f32 w1 = *(f32*)OPlistGet(ModelData->boneWeights, i * 4 + 0);
			f32 w2 = *(f32*)OPlistGet(ModelData->boneWeights, i * 4 + 1);
			f32 w3 = *(f32*)OPlistGet(ModelData->boneWeights, i * 4 + 2);
			f32 w4 = *(f32*)OPlistGet(ModelData->boneWeights, i * 4 + 3);

			writeU16(&myFile, ind1);
			writeU16(&myFile, ind2);
			writeU16(&myFile, ind3);
			writeU16(&myFile, ind4);

			writeF32(&myFile, w1);
			writeF32(&myFile, w2);
			writeF32(&myFile, w3);
			writeF32(&myFile, w4);


			OPlog("%d %d %d %d %f %f %f %f", 
				ind1,
				ind2,
				ind3,
				ind4,
				w1,
				w2,
				w3,
				w4);
		}
	}

	if (features[Model_Indices]) {
		writeU32(&myFile, ModelData->IndexCount / 3);

		for (i32 i = 0; i < ModelData->IndexCount; i++) {
			ui16* ind = (ui16*)OPlistGet(ModelData->indices, i);
			writeU16(&myFile, *ind);
		}
	}

	if (features[Model_Bones]) {
		WriteSkeleton(skel, &myFile);
	}
}

void print_help()
{
	OPlog("Usage: FBXtoOPM [--help]");
}