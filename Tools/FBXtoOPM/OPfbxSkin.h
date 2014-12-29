#pragma once
#include "OPfbxSkeleton.h"
#include "OPfbxMeshData.h"

typedef struct {
	i32 c;
	OPchar* b1;
	OPchar* b2;
	OPchar* b3;
	OPchar* b4;
	f32 w1;
	f32 w2;
	f32 w3;
	f32 w4;
} OPfbxSkinBlendWeight;

typedef struct {
	OPint* BoneIndices;
	f32* BoneWeights;
} OPfbxSkin;


void _fbxmat4Log(const OPchar* msg, FbxAMatrix* mat) {

	FbxVector4 lRow0 = mat->GetRow(0);
	FbxVector4 lRow1 = mat->GetRow(1);
	FbxVector4 lRow2 = mat->GetRow(2);
	FbxVector4 lRow3 = mat->GetRow(3);
	OPlog("\t%s:\n\t\t%f,\t%f,\t%f,\t%f\n\t\t%f,\t%f,\t%f,\t%f\n\t\t%f,\t%f,\t%f,\t%f\n\t\t%f,\t%f,\t%f,\t%f",
		msg, 
		lRow0[0], lRow0[1], lRow0[2], lRow0[3],
		lRow1[0], lRow1[1], lRow1[2], lRow1[3],
		lRow2[0], lRow2[1], lRow2[2], lRow2[3],
		lRow3[0], lRow3[1], lRow3[2], lRow3[3]);
}

OPfbxSkinBlendWeight* _skinBlendWeights(OPfbxMeshData* meshData, OPfbxSkeleton* skeleton) {
	FbxAMatrix geometryTransform = GetGeometryTransformation(meshData->Node);


	int controlPointCount = meshData->Mesh->GetControlPointsCount();
	int deformerCount = meshData->Mesh->GetDeformerCount();
	if (deformerCount == 0) {
		OPlog("This mesh doesn't have a skin.");
		return NULL;
	}


	OPlog("Generating Skin:");

	OPfbxSkinBlendWeight* result = (OPfbxSkinBlendWeight*)OPallocZero(sizeof(OPfbxSkinBlendWeight) * controlPointCount);
	
	for (OPint i = 0; i < deformerCount; i++) {	
		FbxSkin* skinDeformer = (FbxSkin*)meshData->Mesh->GetDeformer(i, FbxDeformer::eSkin);

		OPint clusterCount = skinDeformer->GetClusterCount();
		OPlog("\tclusterCount: %d", clusterCount);
		for (OPint j = 0; j < clusterCount; ++j)
		{
			FbxCluster* cluster = skinDeformer->GetCluster(j);

			FbxNode* boneNode = cluster->GetLink();
			OPlog("Bone: %s", boneNode->GetName());

			OPfbxSkeletonBone* bone = OPfbxSkeletonGet(skeleton, boneNode->GetName());
			OPlog("\tFound Bone: %s", bone->Name);

			FbxAMatrix transformLinkMatrix;
			FbxAMatrix transformLinkMatrixInverse;

			cluster->GetTransformLinkMatrix(transformLinkMatrix);
			transformLinkMatrixInverse = transformLinkMatrix.Inverse();

			_fbxmat4Log("transformLinkMatrix", &transformLinkMatrix);
			_fbxmat4Log("transformLinkMatrixInverse", &transformLinkMatrixInverse);


			FbxVector4 lRow = transformLinkMatrixInverse.GetRow(0);
			bone->BindPose[0][0] = lRow[0];
			bone->BindPose[1][0] = lRow[1];
			bone->BindPose[2][0] = lRow[2];
			bone->BindPose[3][0] = lRow[3];

			lRow = transformLinkMatrixInverse.GetRow(1);
			bone->BindPose[0][1] = lRow[0];
			bone->BindPose[1][1] = lRow[1];
			bone->BindPose[2][1] = lRow[2];
			bone->BindPose[3][1] = lRow[3];

			lRow = transformLinkMatrixInverse.GetRow(2);
			bone->BindPose[0][2]= lRow[0];
			bone->BindPose[1][2] = lRow[1];
			bone->BindPose[2][2] = lRow[2];
			bone->BindPose[3][2] = lRow[3];

			lRow = transformLinkMatrixInverse.GetRow(3);
			bone->BindPose[0][3] = lRow[0];
			bone->BindPose[1][3] = lRow[1];
			bone->BindPose[2][3] = lRow[2];
			bone->BindPose[3][3] = lRow[3];

			OPmat4Log("\tBind Pose", &bone->BindPose);


			if(!boneNode) {
				OPlog("No Cluster link on bone");
				continue;
			}


			int vertexIndexCount = cluster->GetControlPointIndicesCount();
			OPlog("vertexIndexCount: %d", vertexIndexCount);
			for (int k = 0; k < vertexIndexCount; ++k)
			{
				int index = cluster->GetControlPointIndices()[k];
				OPlog("Skin Index: %d", index);
				double weight = cluster->GetControlPointWeights()[k];

				if (weight == 0.0)
				{
					continue;
				}
				if (result[index].c == 0) {
					result[index].b1 = OPstringCopy(boneNode->GetName());
					result[index].w1 = weight;
					OPlog("w0: %f", weight);
				}
				if (result[index].c == 1) {
					result[index].b2 = OPstringCopy(boneNode->GetName());
					result[index].w2 = weight;
					OPlog("w1: %f", weight);
				}
				if (result[index].c == 2) {
					result[index].b3 = OPstringCopy(boneNode->GetName());
					result[index].w3 = weight;
					OPlog("w2: %f", weight);
				}
				if (result[index].c == 3) {
					result[index].b4 = OPstringCopy(boneNode->GetName());
					result[index].w4 = weight;
					OPlog("w3: %f", weight);
				}
				result[index].c++;

			}

		}
	}

	return result;
}



OPint OPfbxSkinGet(OPfbxSkin* skin, OPfbxMeshData* meshData, OPfbxSkeleton* skeleton) {
	OPfbxSkinBlendWeight* blendWeights = _skinBlendWeights(meshData, skeleton);
	OPlog("Step: Blend Weights");

	f32 size = meshData->VertexCount;
	skin->BoneIndices = (OPint*)OPallocZero(sizeof(OPint) * size);
	skin->BoneWeights = (f32*)OPallocZero(sizeof(f32) * size);

	OPint pos = 0;
	OPint polygonCount = meshData->Mesh->GetPolygonCount();

	for (OPint i = 0; i < polygonCount; i++)
	{
		OPint polygonSize = meshData->Mesh->GetPolygonSize(i);

		for (OPint j = 0; j < polygonSize; j++)
		{
			OPint controlPointIndex = meshData->Mesh->GetPolygonVertex(i, j);
			OPint boneIndex;
			f32 boneWeight;

			if (blendWeights[controlPointIndex].c > 0) {
				boneIndex = OPfbxSkeletonGetPos(skeleton, blendWeights[controlPointIndex].b1);
				skin->BoneIndices[pos] = boneIndex;
				skin->BoneWeights[pos] = blendWeights[controlPointIndex].w1;
				pos++;
			}
			if (blendWeights[controlPointIndex].c > 1) {
				boneIndex = OPfbxSkeletonGetPos(skeleton, blendWeights[controlPointIndex].b2);
				skin->BoneIndices[pos] = boneIndex;
				skin->BoneWeights[pos] = blendWeights[controlPointIndex].w2;
				pos++;
			}
			if (blendWeights[controlPointIndex].c > 2) {
				boneIndex = OPfbxSkeletonGetPos(skeleton, blendWeights[controlPointIndex].b3);
				skin->BoneIndices[pos] = boneIndex;
				skin->BoneWeights[pos] = blendWeights[controlPointIndex].w3;
				pos++;
			}
			if (blendWeights[controlPointIndex].c > 3) {
				boneIndex = OPfbxSkeletonGetPos(skeleton, blendWeights[controlPointIndex].b4);
				skin->BoneIndices[pos] = boneIndex;
				skin->BoneWeights[pos] = blendWeights[controlPointIndex].w4;
				pos++;
			}

			for (OPint b = 0; b < 4 - blendWeights[controlPointIndex].c; b++) {
				boneIndex = -1;
				boneWeight = 0.0f;
				skin->BoneIndices[pos] = boneIndex;
				skin->BoneWeights[pos] = boneWeight;
				pos++;			
			}

		}

	}

	OPfree(blendWeights);
	return 0; // Success
}