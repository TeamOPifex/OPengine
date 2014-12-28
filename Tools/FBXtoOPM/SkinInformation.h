#pragma once

#include "FBXHelpers.h"
#include "./Data/include/OPhashMap.h"
#include "./Data/include/OPlist.h"
#include "SkeletonInformation.h"

typedef struct {
	i32 c;
	i8* b1;
	i8* b2;
	i8* b3;
	i8* b4;
	f32 w1;
	f32 w2;
	f32 w3;
	f32 w4;
} ModelSkinBlendWeights;



void OPFBXmat4Log(const OPchar* msg, FbxAMatrix* mat) {

	FbxVector4 lRow0 = mat->GetRow(0);
	FbxVector4 lRow1 = mat->GetRow(1);
	FbxVector4 lRow2 = mat->GetRow(2);
	FbxVector4 lRow3 = mat->GetRow(3);
	OPlog("%s:\n\t%f,\t%f,\t%f,\t%f\n\t%f,\t%f,\t%f,\t%f\n\t%f,\t%f,\t%f,\t%f\n\t%f,\t%f,\t%f,\t%f",
		msg, 
		lRow0[0], lRow0[1], lRow0[2], lRow0[3],
		lRow1[0], lRow1[1], lRow1[2], lRow1[3],
		lRow2[0], lRow2[1], lRow2[2], lRow2[3],
		lRow3[0], lRow3[1], lRow3[2], lRow3[3]);
}

FbxAMatrix GetGeometryTransformation(FbxNode* inNode)
{
	if (!inNode)
	{

		//throw std::exception("Null for mesh geometry");
		//exit();
	}

	const FbxVector4 lT = inNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = inNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = inNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}

ModelSkinBlendWeights* GetSkin(FbxNode* inNode, ModelSkeletonData* skeleton) {
	FbxAMatrix geometryTransform = GetGeometryTransformation(inNode);
	FbxMesh* pMesh = inNode->GetMesh();

	int i;
	int verticeCount = pMesh->GetControlPointsCount();

	int deformerCount = pMesh->GetDeformerCount();
	OPlog("deformerCount: %d", deformerCount);
	if (deformerCount == 0) return NULL;

	ModelSkinBlendWeights* result = (ModelSkinBlendWeights*)OPallocZero(sizeof(ModelSkinBlendWeights)* verticeCount);
	if (skeleton == NULL) {
		OPlog("No Skeleton!");
		return result;
	}

	for (i = 0; i < deformerCount; i++) {

		FbxSkin* lSkinDeformer = (FbxSkin*)pMesh->GetDeformer(i, FbxDeformer::eSkin);

		int lClusterCount = lSkinDeformer->GetClusterCount();
		
		OPlog("lClusterCount: %d", lClusterCount);
		for (int lClusterIndex = 0; lClusterIndex < lClusterCount; ++lClusterIndex)
		{

			FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);
			FbxNode* boneNode = lCluster->GetLink();

			OPlog("Bone Cluster: %s", boneNode->GetName());
			ModelSkeletonBone* bone;
			OPhashMapGet(skeleton->bones, boneNode->GetName(), (void**)&bone);

			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix transformLinkMatrixInverse;
			FbxAMatrix globalBindposeInverseMatrix;

			//lCluster->GetTransformMatrix(transformMatrix);
			lCluster->GetTransformLinkMatrix(transformLinkMatrix);
			transformLinkMatrixInverse = transformLinkMatrix.Inverse();
			//globalBindposeInverseMatrix = transformLinkMatrixInverse * transformMatrix * geometryTransform;

			//OPFBXmat4Log("geometryTransform", &geometryTransform);
			//OPFBXmat4Log("transformMatrix", &transformMatrix);
			OPFBXmat4Log("transformLinkMatrixInverse", &transformLinkMatrixInverse);
			OPFBXmat4Log("transformLinkMatrix", &transformLinkMatrix);
			//OPFBXmat4Log("globalBindposeInverseMatrix", &globalBindposeInverseMatrix);

			FbxVector4 lRow = transformLinkMatrixInverse.GetRow(0);
			bone->bindPose.cols[0].x = lRow[0];
			bone->bindPose.cols[1].x = lRow[1];
			bone->bindPose.cols[2].x = lRow[2];
			bone->bindPose.cols[3].x = lRow[3];

			lRow = transformLinkMatrixInverse.GetRow(1);
			bone->bindPose.cols[0].y = lRow[0];
			bone->bindPose.cols[1].y = lRow[1];
			bone->bindPose.cols[2].y = lRow[2];
			bone->bindPose.cols[3].y = lRow[3];

			lRow = transformLinkMatrixInverse.GetRow(2);
			bone->bindPose.cols[0].z = lRow[0];
			bone->bindPose.cols[1].z = lRow[1];
			bone->bindPose.cols[2].z = lRow[2];
			bone->bindPose.cols[3].z = lRow[3];

			lRow = transformLinkMatrixInverse.GetRow(3);
			bone->bindPose.cols[0].w = lRow[0];
			bone->bindPose.cols[1].w = lRow[1];
			bone->bindPose.cols[2].w = lRow[2];
			bone->bindPose.cols[3].w = lRow[3];

			OPmat4Log("Bind Pose", &bone->bindPose);

			if (!boneNode)
				continue;

			int lVertexIndexCount = lCluster->GetControlPointIndicesCount();
			OPlog("lVertexIndexCount: %d", lVertexIndexCount);
			for (int k = 0; k < lVertexIndexCount; ++k)
			{
				int lIndex = lCluster->GetControlPointIndices()[k];
				double lWeight = lCluster->GetControlPointWeights()[k];
				if (lWeight == 0.0)
				{
					continue;
				}
				if (result[lIndex].c == 0) {
					result[lIndex].b1 = (i8*)boneNode->GetName();
					result[lIndex].w1 = lWeight;
					OPlog("w: %f", lWeight);
				}
				if (result[lIndex].c == 1) {
					result[lIndex].b2 = (i8*)boneNode->GetName();
					result[lIndex].w2 = lWeight;
					OPlog("w: %f", lWeight);
				}
				if (result[lIndex].c == 2) {
					result[lIndex].b3 = (i8*)boneNode->GetName();
					result[lIndex].w3 = lWeight;
					OPlog("w: %f", lWeight);
				}
				if (result[lIndex].c == 3) {
					result[lIndex].b4 = (i8*)boneNode->GetName();
					result[lIndex].w4 = lWeight;
					OPlog("w: %f", lWeight);
				}
				result[lIndex].c++;
			}

		}
	}

	return result;
}