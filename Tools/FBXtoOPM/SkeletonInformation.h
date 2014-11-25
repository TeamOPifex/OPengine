#pragma once

#include "FBXHelpers.h"
#include "./Data/include/HashMap.h"
#include "./Data/include/List.h"
#include "./Math/include/Matrix4.h"
#include "./Data/include/String.h"

typedef struct {
	i8* name;
	OPlist* children;
	OPmat4 bindPose;
	i16 index;
} ModelSkeletonBone;

typedef struct {
	ModelSkeletonBone* rootBone;
	OPhashMap* bones;
	ui16 boneCount;
} ModelSkeletonData;

i16 pos;
OPint found;

ModelSkeletonBone* GetSkeletonBones(FbxNode* bone, OPhashMap* bones) {
	pos++;
	int i;
	ModelSkeletonBone* result = (ModelSkeletonBone*)OPallocZero(sizeof(ModelSkeletonBone));
	result->name = OPstringCreateMerged((char *)bone->GetName(), "");
	result->children = OPlistCreate(1, sizeof(ModelSkeletonBone));
	result->index = pos;
	OPhashMapPut(bones, result->name, result);
	
	for (i = 0; i < bone->GetChildCount(); i++) {
		ModelSkeletonBone* child = GetSkeletonBones(bone->GetChild(i), bones);
		OPlistPush(result->children, (ui8*)child);
	}

	return result;
}

ModelSkeletonData* GetSkeleton(FbxNode* node) {
	pos = -1;

	ModelSkeletonData* result = (ModelSkeletonData*)OPallocZero(sizeof(ModelSkeletonData));

	result->bones = OPhashMapCreate(128);

	result->rootBone = GetSkeletonBones(node, result->bones);

	ModelSkeletonBone* test;
	OPhashMapGet(result->bones, "test:head", (void**)&test);

	result->boneCount = pos;

	return result;
}

void _GetBonePosition(ModelSkeletonBone* bone, i8* name) {
	pos++;
	if (OPstringEquals(bone->name, name)) {
		found = 1;
		return;
	}
	for (i16 i = 0; i < OPlistSize(bone->children); i++) {
		_GetBonePosition((ModelSkeletonBone*)OPlistGet(bone->children, i), name);
		if (found) {
			return;
		}
	}
}

i16 GetBonePosition(ModelSkeletonData* skeleton, i8* name) {
	pos = -1;
	found = 0;
	_GetBonePosition(skeleton->rootBone, name);
	return pos;
}

#include "Helpers.h"

void WriteBone(ModelSkeletonBone* bone, ofstream* file) {
	writeI16(file, bone->index);
	ui32 len = strlen(bone->name);
	writeU32(file, len);
	write(file, bone->name, len);

	for (i32 c = 0; c < 4; c++) {
		writeF32(file, bone->bindPose.cols[c].x);
		writeF32(file, bone->bindPose.cols[c].y);
		writeF32(file, bone->bindPose.cols[c].z);
		writeF32(file, bone->bindPose.cols[c].w);
	}

	for (i32 i = 0; i < OPlistSize(bone->children); i++) {
		WriteBone((ModelSkeletonBone*)OPlistGet(bone->children, i), file);
	}
}

void WriteSkeleton(ModelSkeletonData* skeleton, ofstream* file) {
	writeU16(file, skeleton->boneCount);
	WriteBone(skeleton->rootBone, file);
}