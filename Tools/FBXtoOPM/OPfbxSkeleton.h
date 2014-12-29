#pragma once
#include "OPfbxSdk.h"
#include "./Data/include/OPstring.h"

typedef struct {
	OPchar* Name;
	OPint ChildrenCount;
	OPmat4 BindPose;
	OPint Parent;
} OPfbxSkeletonBone;

typedef struct {
	OPint BoneCount;
	OPfbxSkeletonBone* Bones;
} OPfbxSkeleton;

OPint _skeletonBoneCount(FbxNode* node) {
	OPint total = 1; // Count this bone

	// Add each child to the total
	OPint children = node->GetChildCount();
	for (OPint i = 0; i < children; i++) {
		total += _skeletonBoneCount(node->GetChild(i));
	}

	return total;
}

void _skeletonFillBones(FbxNode* node, OPfbxSkeletonBone* bones, OPint* pos, OPint parent) {

	bones[*pos].Name = OPstringCopy(node->GetName());
	OPlog("Bone: %s", bones[*pos].Name);

	bones[*pos].Parent = parent;
	OPint index = *pos;

	(*pos)++;

	OPint children = node->GetChildCount();
	for (OPint i = 0; i < children; i++) {
		_skeletonFillBones(node->GetChild(i), bones, pos, index);
	}
}

OPfbxSkeletonBone* OPfbxSkeletonGet(OPfbxSkeleton* skeleton, const OPchar* name) {
	for(OPint i = 0; i < skeleton->BoneCount; i++) {
		if(OPstringEquals(skeleton->Bones[i].Name, name)) {
			return &skeleton->Bones[i];
		}
	}
	return NULL;
}

OPint OPfbxSkeletonGetPos(OPfbxSkeleton* skeleton, const OPchar* name) {	
	for(OPint i = 0; i < skeleton->BoneCount; i++) {
		if(OPstringEquals(skeleton->Bones[i].Name, name)) {
			return i;
		}
	}
	return -1;
}

OPint OPfbxSkeletonGet(OPfbxSkeleton* skeleton, OPfbxScene* scene) {
	// Loop through each child in the root node of the scene until 
	// we find the first available skeleton (Only supporting 1 skeleton)
	// TODO: Support multiple skeletons
	FbxNodeAttribute::EType attributeType;
	FbxNode* node;

	for (int i = 0; i < scene->RootNode->GetChildCount(); i++) {
		node = scene->RootNode->GetChild(i);		
		attributeType = node->GetNodeAttribute()->GetAttributeType();
		if(attributeType == FbxNodeAttribute::eSkeleton) {

			OPint totalBoneCount = _skeletonBoneCount(node);
			OPlog("Total Bones in this model: %d", totalBoneCount);

			skeleton->BoneCount = totalBoneCount;
			skeleton->Bones = (OPfbxSkeletonBone*)OPalloc(sizeof(OPfbxSkeletonBone) * totalBoneCount);

			OPint pos = 0;
			_skeletonFillBones(node, skeleton->Bones, &pos, -1);

			// TODO(garrett): Remove Test Code
			// OPfbxSkeletonBone* test = OPfbxSkeletonGet(skeleton, "joint1");
			// if(test == NULL) {
			// 	OPlog("Failed to find joint1");
			// } else {
			// 	OPlog("Found %s", test->Name);
			// }

			return 0;
		}
	}

	// Failed to find a skeleton
	OPlog("Failed to find a skeleton in the scene provided.");
	return -1;
}

void OPfbxSkeletonDestroy(OPfbxSkeleton* skeleton) {

}