#pragma once
#include "OPfbxSkeleton.h"
#include "OPfbxMeshData.h"
#include "OPfbxSkin.h"

typedef struct {
	OPint StartFrame;
	OPint EndFrame;
	OPint TotalFrames;
	OPchar* Name;
	OPmat4* JointTransform;
} OPfbxAnimationTrack;

typedef struct {
	OPint AnimationTrackCount;
	OPfbxAnimationTrack* Animations;
} OPfbxAnimation;

OPfbxAnimation* OPfbxAnimationGet(OPfbxMeshData* meshData, OPfbxSkeleton* skeleton, OPfbxScene* scene) {
	OPint deformerCount = meshData->Mesh->GetDeformerCount();
	OPfbxAnimation* result = (OPfbxAnimation*)OPalloc(sizeof(OPfbxAnimation));
	result->AnimationTrackCount = 1;
	result->Animations = (OPfbxAnimationTrack*)OPallocZero(sizeof(OPfbxAnimationTrack)* result->AnimationTrackCount);


	// Only 1 take right now
	FbxAnimStack* currAnimStack = scene->Scene->GetSrcObject<FbxAnimStack>(0);
	FbxString animStackName = currAnimStack->GetName();
	OPlog("Anim Name: %s", animStackName.Buffer());

	FbxTakeInfo* takeInfo = scene->Scene->GetTakeInfo(animStackName);
	FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
	FbxTime end = takeInfo->mLocalTimeSpan.GetStop();

	FbxLongLong mAnimationLength = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;
	OPlog("Anim Start: %d", (start.GetFrameCount(FbxTime::eFrames24) + 1));
	OPlog("Anim End: %d", (end.GetFrameCount(FbxTime::eFrames24)));
	OPlog("Anim Frames: %d", mAnimationLength);
	OPlog("Bone Count: %d", skeleton->BoneCount);

	result->Animations[0].StartFrame = (start.GetFrameCount(FbxTime::eFrames24) + 1);
	result->Animations[0].EndFrame = end.GetFrameCount(FbxTime::eFrames24);
	result->Animations[0].TotalFrames = mAnimationLength;
	result->Animations[0].Name = OPstringCopy(animStackName.Buffer());
	result->Animations[0].JointTransform = (OPmat4*)OPalloc(sizeof(OPmat4)* skeleton->BoneCount * mAnimationLength);


	for (OPint i = 0; i < deformerCount; i++) {
		FbxSkin* skinDeformer = (FbxSkin*)meshData->Mesh->GetDeformer(i, FbxDeformer::eSkin);
		OPint clusterCount = skinDeformer->GetClusterCount();
		for (OPint j = 0; j < clusterCount; ++j) {
			FbxCluster* cluster = skinDeformer->GetCluster(j);
			FbxNode* boneNode = cluster->GetLink();

			for (FbxLongLong f = start.GetFrameCount(FbxTime::eFrames24); f <= end.GetFrameCount(FbxTime::eFrames24); ++f)
			{
				FbxTime currTime;
				currTime.SetFrame(f, FbxTime::eFrames24);
				OPint currFrame = f;

				FbxAMatrix boneLocalTransform = boneNode->EvaluateLocalTransform(currTime);

				OPint ind = OPfbxSkeletonGetPos(skeleton, boneNode->GetName());
				OPlog("Bone %s at Index %d", boneNode->GetName(), ind);
				OPint indAnimJoint = skeleton->BoneCount * currFrame + ind;
				OPlog("Curr Frame: %d\nindAnimJoint: %d", f, indAnimJoint);
				_fbxmat4Log("Local Bone Transform", &boneLocalTransform);
				
				
				OPmat4 tmp;

				FbxVector4 lRow = boneLocalTransform.GetRow(0);
				tmp[0][0] = lRow[0];
				tmp[1][0] = lRow[1];
				tmp[2][0] = lRow[2];
				tmp[3][0] = lRow[3];

				lRow = boneLocalTransform.GetRow(1);
				tmp[0][1] = lRow[0];
				tmp[1][1] = lRow[1];
				tmp[2][1] = lRow[2];
				tmp[3][1] = lRow[3];

				lRow = boneLocalTransform.GetRow(2);
				tmp[0][2] = lRow[0];
				tmp[1][2] = lRow[1];
				tmp[2][2] = lRow[2];
				tmp[3][2] = lRow[3];

				lRow = boneLocalTransform.GetRow(3);
				tmp[0][3] = lRow[0];
				tmp[1][3] = lRow[1];
				tmp[2][3] = lRow[2];
				tmp[3][3] = lRow[3];

				result->Animations[0].JointTransform[indAnimJoint] = tmp;
				OPmat4Log("Anim Joint", result->Animations[0].JointTransform[indAnimJoint]);
			}
		}
	}

	return result;
}