#include "Helpers.h"
#include "FBXHelpers.h"
#include "SkinInformation.h"
#include "SkeletonInformation.h"
#include "PoseInformation.h"
#include "MenuHelper.h"
#include "ModelData.h"

#include "./Math/include/Vector3.h"
#include "./Math/include/Vector2.h"
#include "./Data/include/List.h"

OPModelData* ModelData;
ModelSkeletonData* GlobalSkeleton;

void FillPolygons(FbxNode* pNode)
{
	FbxMesh* pMesh = pNode->GetMesh();

	int i, j, lPolygonCount = pMesh->GetPolygonCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	char header[100];

	ModelSkinBlendWeights* skin = GetSkin(pNode, GlobalSkeleton);

	ui16 indexOffset = ModelData->VertexCount;

	for (i = 0; i < lPolygonCount; i++)
	{
		int l;
		int lPolygonSize = pMesh->GetPolygonSize(i);
		if (lPolygonSize == 3) {
			ModelData->IndexCount += 3;
		}
		else if (lPolygonSize == 4) {
			ModelData->IndexCount += 6;
		}

		ModelData->VertexCount += lPolygonSize;
		ModelData->ColorCount += lPolygonSize * pMesh->GetElementVertexColorCount();
		ModelData->UVCount += lPolygonSize * pMesh->GetElementUVCount();
		ModelData->NormalCount += lPolygonSize * pMesh->GetElementNormalCount();
		ModelData->TangentCount += lPolygonSize * pMesh->GetElementTangentCount();
	}

	int vertexId = 0;
	i32 pos = -1;
	i32 ind = 0;
	ui16 ind0, ind1, ind2, ind3;
	i16 boneIndex;
	f32 boneWeight = 0;
	OPvec3 vertexPos;
	OPvec3 vertexNorm;
	OPvec2 vertexUV;
	for (i = 0; i < lPolygonCount; i++)
	{
		//DisplayInt("        Polygon ", i);
		int l;

		//for (l = 0; l < pMesh->GetElementPolygonGroupCount(); l++)
		//{
		//	FbxGeometryElementPolygonGroup* lePolgrp = pMesh->GetElementPolygonGroup(l);
		//	switch (lePolgrp->GetMappingMode())
		//	{
		//	case FbxGeometryElement::eByPolygon:
		//		if (lePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex)
		//		{
		//			FBXSDK_sprintf(header, 100, "        Assigned to group: ");
		//			int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
		//			DisplayInt(header, polyGroupId);
		//			break;
		//		}
		//	default:
		//		// any other mapping modes don't make sense
		//		DisplayString("        \"unsupported group assignment\"");
		//		break;
		//	}
		//}

		int lPolygonSize = pMesh->GetPolygonSize(i);

		if (lPolygonSize == 3) {
			ind0 = pos + 1 + indexOffset;
			ind1 = pos + 2 + indexOffset;
			ind2 = pos + 3 + indexOffset;
			OPlistPush(ModelData->indices, (ui8*)&ind0);
			OPlistPush(ModelData->indices, (ui8*)&ind1);
			OPlistPush(ModelData->indices, (ui8*)&ind2);
			ind += 3;
		}
		else if (lPolygonSize == 4) {
			ind0 = pos + 1 + indexOffset;
			ind1 = pos + 2 + indexOffset;
			ind2 = pos + 3 + indexOffset;
			ind3 = pos + 4 + indexOffset;

			OPlistPush(ModelData->indices, (ui8*)&ind0);
			OPlistPush(ModelData->indices, (ui8*)&ind1);
			OPlistPush(ModelData->indices, (ui8*)&ind2);

			OPlistPush(ModelData->indices, (ui8*)&ind0);
			OPlistPush(ModelData->indices, (ui8*)&ind2);
			OPlistPush(ModelData->indices, (ui8*)&ind3);
			ind += 6;
		}

		for (j = 0; j < lPolygonSize; j++)
		{
			pos++;

			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);

			//Display3DVector("            Coordinates: ", lControlPoints[lControlPointIndex]);
			vertexPos = OPvec3Create(
					(float)lControlPoints[lControlPointIndex][0],
					(float)lControlPoints[lControlPointIndex][1],
					(float)lControlPoints[lControlPointIndex][2]
				);

			if (skin[lControlPointIndex].c > 0) {
				boneIndex = GetBonePosition(GlobalSkeleton, skin[lControlPointIndex].b1);
				OPlistPush(ModelData->boneIndices, (ui8*)&boneIndex);
				OPlistPush(ModelData->boneWeights, (ui8*)&skin[lControlPointIndex].w1);
			}
			if (skin[lControlPointIndex].c > 1) {
				boneIndex = GetBonePosition(GlobalSkeleton, skin[lControlPointIndex].b2);
				OPlistPush(ModelData->boneIndices, (ui8*)&boneIndex);
				OPlistPush(ModelData->boneWeights, (ui8*)&skin[lControlPointIndex].w2);
			}
			if (skin[lControlPointIndex].c > 2) {
				boneIndex = GetBonePosition(GlobalSkeleton, skin[lControlPointIndex].b3);
				OPlistPush(ModelData->boneIndices, (ui8*)&boneIndex);
				OPlistPush(ModelData->boneWeights, (ui8*)&skin[lControlPointIndex].w3);
			}
			if (skin[lControlPointIndex].c > 3) {
				boneIndex = GetBonePosition(GlobalSkeleton, skin[lControlPointIndex].b4);
				OPlistPush(ModelData->boneIndices, (ui8*)&boneIndex);
				OPlistPush(ModelData->boneWeights, (ui8*)&skin[lControlPointIndex].w4);
			}

			for (i32 b = 0; b < skin[lControlPointIndex].c - 4; b++) {
				boneIndex = -1;
				OPlistPush(ModelData->boneIndices, (ui8*)&boneIndex);
				OPlistPush(ModelData->boneWeights, (ui8*)&boneWeight);				
			}
			
			OPlistPush(ModelData->vertices, (ui8*)&vertexPos);

			//for (l = 0; l < pMesh->GetElementVertexColorCount(); l++)
			//{
			//	FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor(l);
			//	FBXSDK_sprintf(header, 100, "            Color vertex: ");
			//	switch (leVtxc->GetMappingMode())
			//	{
			//	default:
			//		break;
			//	case FbxGeometryElement::eByControlPoint:
			//		switch (leVtxc->GetReferenceMode())
			//		{
			//		case FbxGeometryElement::eDirect:
			//			DisplayColor(header, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
			//			break;
			//		case FbxGeometryElement::eIndexToDirect:
			//		{
			//			int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
			//			DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
			//		}
			//			break;
			//		default:
			//			break; // other reference modes not shown here!
			//		}
			//		break;
			//	case FbxGeometryElement::eByPolygonVertex:
			//	{
			//		switch (leVtxc->GetReferenceMode())
			//		{
			//		case FbxGeometryElement::eDirect:
			//			DisplayColor(header, leVtxc->GetDirectArray().GetAt(vertexId));
			//			break;
			//		case FbxGeometryElement::eIndexToDirect:
			//		{
			//			int id = leVtxc->GetIndexArray().GetAt(vertexId);
			//			DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
			//		}
			//			break;
			//		default:
			//			break; // other reference modes not shown here!
			//		}
			//	}
			//		break;
			//	case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
			//	case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
			//	case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
			//		break;
			//	}
			//}


			for (l = 0; l < pMesh->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV* leUV = pMesh->GetElementUV(l);
				//FBXSDK_sprintf(header, 100, "            Texture UV: ");

				FbxVector2 uv;

				switch (leUV->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						uv = leUV->GetDirectArray().GetAt(lControlPointIndex);
						vertexUV = OPvec2Create(uv[0], uv[1]);
						OPlistPush(ModelData->uvs, (ui8*)&vertexUV);
						//Display2DVector(header, leUV->GetDirectArray().GetAt(lControlPointIndex));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
						uv = leUV->GetDirectArray().GetAt(id);
						vertexUV = OPvec2Create(uv[0], uv[1]);
						OPlistPush(ModelData->uvs, (ui8*)&vertexUV);
						//Display2DVector(header, leUV->GetDirectArray().GetAt(id));
					}
						break;
					default:
						break; // other reference modes not shown here!
					}
					break;

					case FbxGeometryElement::eByPolygonVertex:
					{
						int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
						switch (leUV->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
						case FbxGeometryElement::eIndexToDirect:
						{
							uv = leUV->GetDirectArray().GetAt(lTextureUVIndex);
							vertexUV = OPvec2Create(uv[0], uv[1]);
							OPlistPush(ModelData->uvs, (ui8*)&vertexUV);
							//Display2DVector(header, leUV->GetDirectArray().GetAt(lTextureUVIndex));
						}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}
			for (l = 0; l < pMesh->GetElementNormalCount(); ++l)
			{
				FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(l);
				//FBXSDK_sprintf(header, 100, "            Normal: ");

				FbxVector4 normal;
				if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						normal = leNormal->GetDirectArray().GetAt(vertexId);
						vertexNorm = OPvec3Create(normal[0], normal[1], normal[2]);
						OPlistPush(ModelData->normals, (ui8*)&vertexNorm);
						//Display3DVector(header, normal);
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leNormal->GetIndexArray().GetAt(vertexId);
						normal = leNormal->GetDirectArray().GetAt(id);
						vertexNorm = OPvec3Create(normal[0], normal[1], normal[2]);
						OPlistPush(ModelData->normals, (ui8*)&vertexNorm);
						//Display3DVector(header, normal);
					}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}

			}
			//for (l = 0; l < pMesh->GetElementTangentCount(); ++l)
			//{
			//	FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(l);
			//	FBXSDK_sprintf(header, 100, "            Tangent: ");

			//	if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			//	{
			//		switch (leTangent->GetReferenceMode())
			//		{
			//		case FbxGeometryElement::eDirect:
			//			Display3DVector(header, leTangent->GetDirectArray().GetAt(vertexId));
			//			break;
			//		case FbxGeometryElement::eIndexToDirect:
			//		{
			//			int id = leTangent->GetIndexArray().GetAt(vertexId);
			//			Display3DVector(header, leTangent->GetDirectArray().GetAt(id));
			//		}
			//			break;
			//		default:
			//			break; // other reference modes not shown here!
			//		}
			//	}

			//}
			//for (l = 0; l < pMesh->GetElementBinormalCount(); ++l)
			//{

			//	FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal(l);

			//	FBXSDK_sprintf(header, 100, "            Binormal: ");
			//	if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			//	{
			//		switch (leBinormal->GetReferenceMode())
			//		{
			//		case FbxGeometryElement::eDirect:
			//			Display3DVector(header, leBinormal->GetDirectArray().GetAt(vertexId));
			//			break;
			//		case FbxGeometryElement::eIndexToDirect:
			//		{
			//												   int id = leBinormal->GetIndexArray().GetAt(vertexId);
			//												   Display3DVector(header, leBinormal->GetDirectArray().GetAt(id));
			//		}
			//			break;
			//		default:
			//			break; // other reference modes not shown here!
			//		}
			//	}
			//}
			vertexId++;
		} // for polygonSize
	} // for polygonCount

	OPlog("");
}

void FillNode(FbxNode* pNode) {
	FbxNodeAttribute::EType lAttributeType;
	int i;
	FbxMesh* lMesh;
	FbxSkeleton* lSkeleton;
	const char* lSkeletonTypes[] = { "Root", "Limb", "Limb Node", "Effector" };

	if (pNode->GetNodeAttribute() == NULL)
	{
		FBXSDK_printf("NULL Node Attribute\n\n");
	}
	else
	{
		lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());
		switch (lAttributeType)
		{
			default:
				break;

			case FbxNodeAttribute::eMesh:
				lMesh = (FbxMesh*)pNode->GetNodeAttribute();
				OPlog("Mesh Name: %s", (char *)pNode->GetName());
				FillPolygons(pNode);
				break;
			
			case FbxNodeAttribute::eSkeleton:
				GlobalSkeleton = GetSkeleton(pNode);
				break;
		}
	}
}

int main(int argc, char **argv) {
	int arg;
	i8* filename = NULL;
	i8* output = NULL;
	
	//
	// Fill in provided arguments
	//
	if (argc > 1) {
		for (arg = 1; arg < argc; ++arg)
		{
			if (IsParam(argv, arg, "--help") || IsParam(argv, arg, "-h"))
			{
				print_help();
				exit(1);
			}

			if (IsParam(argv, arg, "--file") || IsParam(argv, arg, "-f"))
			{
				++arg;
				filename = argv[arg];
				continue;
			}

			if (IsParam(argv, arg, "--out") || IsParam(argv, arg, "-o"))
			{
				++arg;
				output = argv[arg];
				continue;
			}
		}
	}

	//
	// Quit if no file was provided
	//
	if (filename == NULL) {
		print_help();
		return -1;
	}


	//
	// Get Features to Export from the FBX into the OPM
	//
	OPint* features = (OPint*)OPallocZero(sizeof(OPint)* 9);
	features[Model_Positions] = 1;
	features[Model_Normals] = 1;
	features[Model_UVs] = 1;
	features[Model_Indices] = 1;
	MenuOptions(
		"Select OPM Features to Export\n------------------------",
		features,
		9,
		"POSITION", "NORMAL", "UVS", "COLORS", "INDICES", "TANGENTS", "BONES", "SKIN", "ANIMATIONS"
		);


	//
	// Initialize ModelData
	//
	ModelData = ModelDataCreate();


	//
	// Setup the Autodesk FBX SDK
	//
	FbxManager* lSdkManager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
	if (!lImporter->Initialize(filename, -1, lSdkManager->GetIOSettings())) {
		OPlog("Call to FbxImporter::Initialize() failed.\n");
		OPlog("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		return -1;
	}

	// Get the scene from the FBX file	
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
	lImporter->Import(lScene);
	lImporter->Destroy();

	FbxDocumentInfo* sceneInfo = lScene->GetSceneInfo();    
	if (!sceneInfo) {
		OPlog("Scene info failed.");
		return -1;
	}

	//
	// Retreive the data from the model
	//
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode) {
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
			FillNode(lRootNode->GetChild(i));
	}
	// Get Skinning information for the bind pose
	GetPoseInformation(lScene, GlobalSkeleton);

	//
	// Write the OPM file
	//
	if (output != NULL) WriteFile(output, features, ModelData, GlobalSkeleton);
	else OPlog("No output file given.");
	

	lSdkManager->Destroy();
	return 0;
}