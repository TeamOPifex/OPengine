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

OPfbxAnimation* OPfbxAnimationGet(OPfbxMeshData** meshData, OPuint meshDataCount, OPfbxSkeleton* skeleton, OPfbxScene* scene, OPfloat scale) {
	OPint deformerCount = meshData[0]->Mesh->GetDeformerCount();
	OPfbxAnimation* result = (OPfbxAnimation*)OPalloc(sizeof(OPfbxAnimation));
	result->AnimationTrackCount = 1;
	result->Animations = (OPfbxAnimationTrack*)OPallocZero(sizeof(OPfbxAnimationTrack)* result->AnimationTrackCount);


	// Only 1 take right now
	FbxAnimStack* currAnimStack = scene->Scene->GetSrcObject<FbxAnimStack>(0);
	FbxString animStackName = currAnimStack->GetName();
	OPlogDebug("Anim Name: %s", animStackName.Buffer());

	FbxTakeInfo* takeInfo = scene->Scene->GetTakeInfo(animStackName);
	FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
	FbxTime end = takeInfo->mLocalTimeSpan.GetStop();

	FbxLongLong mAnimationLength = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;
	OPlogDebug("Anim Start: %d", (start.GetFrameCount(FbxTime::eFrames24) + 1));
	OPlogDebug("Anim End: %d", (end.GetFrameCount(FbxTime::eFrames24)));
	OPlogDebug("Anim Frames: %d", mAnimationLength);
	OPlogDebug("Bone Count: %d", skeleton->BoneCount);

	result->Animations[0].StartFrame = (start.GetFrameCount(FbxTime::eFrames24) + 1);
	result->Animations[0].EndFrame = end.GetFrameCount(FbxTime::eFrames24);
	result->Animations[0].TotalFrames = mAnimationLength;
	result->Animations[0].Name = OPstringCopy(animStackName.Buffer());
	result->Animations[0].JointTransform = (OPmat4*)OPalloc(sizeof(OPmat4)* skeleton->BoneCount * mAnimationLength);


	for (OPint i = 0; i < deformerCount; i++) {
		FbxSkin* skinDeformer = (FbxSkin*)meshData[0]->Mesh->GetDeformer(i, FbxDeformer::eSkin);
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
				OPlogDebug("Bone %s at Index %d", boneNode->GetName(), ind);
				OPint indAnimJoint = skeleton->BoneCount * currFrame + ind;
				OPlogDebug("Curr Frame: %d\nindAnimJoint: %d", f, indAnimJoint);
				_fbxmat4Log("Local Bone Transform", &boneLocalTransform);
				
				// FbxVector4 _scl = boneLocalTransform.GetS();
				// FbxVector4 _rot = boneLocalTransform.GetR();
				// FbxVector4 _trl = boneLocalTransform.GetT();
				
				// Scale only the root node
				//if(ind == 0) {
				 	//_scl *= scale;
					//_trl *= scale;
				//}

				//boneLocalTransform = FbxAMatrix(_trl, _rot, _scl);

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

				//tmp = OPmat4Transpose(tmp);

				// OPfloat x, y, z;
				// x = tmp[3][0];
				// y = tmp[3][1];
				// z = tmp[3][2];

				// if(ind == 0) {
				// 	tmp = tmp * OPmat4Scl(scale);
				// }

				// tmp[3][0] = x;
				// tmp[3][1] = y;
				// tmp[3][2] = z;

				result->Animations[0].JointTransform[indAnimJoint] = tmp;
				//OPmat4Log("Anim Joint", result->Animations[0].JointTransform[indAnimJoint]);
			}
		}
	}

	return result;
}