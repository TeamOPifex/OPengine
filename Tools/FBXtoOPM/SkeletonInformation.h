#pragma once

#include "FBXHelpers.h"
#include "./Data/include/OPhashMap.h"
#include "./Data/include/OPlist.h"
#include "./Math/include/OPmat4.h"
#include "./Data/include/OPstring.h"

typedef struct {
	i8* name;
	OPlist* children;
	OPmat4 bindPose;
	i16 index;
	i16 parent;
} ModelSkeletonBone;

typedef struct {
	ModelSkeletonBone* rootBone;
	OPhashMap* bones;
	ui16 boneCount;
} ModelSkeletonData;

i16 pos;
OPint found;

ModelSkeletonBone* GetSkeletonBones(FbxNode* bone, OPhashMap* bones, i16 parent) {
	pos++;
	int i;
	ModelSkeletonBone* result = (ModelSkeletonBone*)OPallocZero(sizeof(ModelSkeletonBone));
	result->name = OPstringCreateMerged((char *)bone->GetName(), "");
	result->children = OPlistCreate(1, sizeof(ModelSkeletonBone));
	result->index = pos;
	result->parent = parent;
	OPmat4Identity(&result->bindPose);
	

	for (i = 0; i < bone->GetChildCount(); i++) {
		ModelSkeletonBone* child = GetSkeletonBones(bone->GetChild(i), bones, result->index);
		OPint ind = OPlistPush(result->children, (ui8*)child);
		ModelSkeletonBone* r = (ModelSkeletonBone*)OPlistGet(result->children, ind);
		OPlog("Child: %s", r->name);
		OPhashMapPut(bones, child->name, r);
	}

	return result;
}

ModelSkeletonData* GetSkeleton(FbxNode* node) {
	pos = -1;

	ModelSkeletonData* result = (ModelSkeletonData*)OPallocZero(sizeof(ModelSkeletonData));

	result->bones = OPhashMapCreate(128);

	result->rootBone = GetSkeletonBones(node, result->bones, 0);
	OPhashMapPut(result->bones, result->rootBone->name, result->rootBone);

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

void WriteBone(ModelSkeletonData* skeleton, ModelSkeletonBone* bone, ofstream* file) {
	writeI16(file, bone->parent);
	ui32 len = strlen(bone->name);
	writeU32(file, len);
	write(file, bone->name, len);


	OPlog("Bone: %s", bone->name);

	ModelSkeletonBone* bonePtr;
	OPhashMapGet(skeleton->bones, bone->name, (void**)&bonePtr);

	OPlog("Bone: %s", bonePtr->name);
	OPmat4Log("Bind Pose 2", &bonePtr->bindPose);

	for (i32 c = 0; c < 4; c++) {
		writeF32(file, bonePtr->bindPose.cols[c].x);
		writeF32(file, bonePtr->bindPose.cols[c].y);
		writeF32(file, bonePtr->bindPose.cols[c].z);
		writeF32(file, bonePtr->bindPose.cols[c].w);
	}

	for (i32 i = 0; i < OPlistSize(bone->children); i++) {
		WriteBone(skeleton, (ModelSkeletonBone*)OPlistGet(bone->children, i), file);
	}
}

void WriteSkeleton(ModelSkeletonData* skeleton, ofstream* file) {
	writeU16(file, skeleton->boneCount);
	WriteBone(skeleton, skeleton->rootBone, file);
}