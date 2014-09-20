#include "WriteHelper.h"
#include "FBXHelpers.h"
#include "SkinInformation.h"
#include "SkeletonInformation.h"
#include "PoseInformation.h"
#include "MenuHelper.h"

#include "./Math/include/Vector3.h"
#include "./Math/include/Vector2.h"
#include "./Data/include/List.h"

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

OPModelData ModelData;
ModelSkeletonData* GlobalSkeleton;



void print_help()
{
	OPlog("Usage: FBXtoOPM [--help]");
}
void DisplayPolygons(FbxNode* pNode)
{
	FbxMesh* pMesh = pNode->GetMesh();

	int i, j, lPolygonCount = pMesh->GetPolygonCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	char header[100];

	ModelSkinBlendWeights* skin = GetSkin(pNode, GlobalSkeleton);

	ui16 indexOffset = ModelData.VertexCount;

	for (i = 0; i < lPolygonCount; i++)
	{
		int l;
		int lPolygonSize = pMesh->GetPolygonSize(i);
		if (lPolygonSize == 3) {
			ModelData.IndexCount += 3;
		}
		else if (lPolygonSize == 4) {
			ModelData.IndexCount += 6;
		}

		ModelData.VertexCount += lPolygonSize;
		ModelData.ColorCount += lPolygonSize * pMesh->GetElementVertexColorCount();
		ModelData.UVCount += lPolygonSize * pMesh->GetElementUVCount();
		ModelData.NormalCount += lPolygonSize * pMesh->GetElementNormalCount();
		ModelData.TangentCount += lPolygonSize * pMesh->GetElementTangentCount();
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
			OPlistPush(ModelData.indices, (ui8*)&ind0);
			OPlistPush(ModelData.indices, (ui8*)&ind1);
			OPlistPush(ModelData.indices, (ui8*)&ind2);
			ind += 3;
		}
		else if (lPolygonSize == 4) {
			ind0 = pos + 1 + indexOffset;
			ind1 = pos + 2 + indexOffset;
			ind2 = pos + 3 + indexOffset;
			ind3 = pos + 4 + indexOffset;

			OPlistPush(ModelData.indices, (ui8*)&ind0);
			OPlistPush(ModelData.indices, (ui8*)&ind1);
			OPlistPush(ModelData.indices, (ui8*)&ind2);

			OPlistPush(ModelData.indices, (ui8*)&ind0);
			OPlistPush(ModelData.indices, (ui8*)&ind2);
			OPlistPush(ModelData.indices, (ui8*)&ind3);
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
				OPlistPush(ModelData.boneIndices, (ui8*)&boneIndex);
				OPlistPush(ModelData.boneWeights, (ui8*)&skin[lControlPointIndex].w1);
			}
			if (skin[lControlPointIndex].c > 1) {
				boneIndex = GetBonePosition(GlobalSkeleton, skin[lControlPointIndex].b2);
				OPlistPush(ModelData.boneIndices, (ui8*)&boneIndex);
				OPlistPush(ModelData.boneWeights, (ui8*)&skin[lControlPointIndex].w2);
			}
			if (skin[lControlPointIndex].c > 2) {
				boneIndex = GetBonePosition(GlobalSkeleton, skin[lControlPointIndex].b3);
				OPlistPush(ModelData.boneIndices, (ui8*)&boneIndex);
				OPlistPush(ModelData.boneWeights, (ui8*)&skin[lControlPointIndex].w3);
			}
			if (skin[lControlPointIndex].c > 3) {
				boneIndex = GetBonePosition(GlobalSkeleton, skin[lControlPointIndex].b4);
				OPlistPush(ModelData.boneIndices, (ui8*)&boneIndex);
				OPlistPush(ModelData.boneWeights, (ui8*)&skin[lControlPointIndex].w4);
			}

			for (i32 b = 0; b < skin[lControlPointIndex].c - 4; b++) {
				boneIndex = -1;
				OPlistPush(ModelData.boneIndices, (ui8*)&boneIndex);
				OPlistPush(ModelData.boneWeights, (ui8*)&boneWeight);				
			}
			
			OPlistPush(ModelData.vertices, (ui8*)&vertexPos);

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
						OPlistPush(ModelData.uvs, (ui8*)&vertexUV);
						//Display2DVector(header, leUV->GetDirectArray().GetAt(lControlPointIndex));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
						uv = leUV->GetDirectArray().GetAt(id);
						vertexUV = OPvec2Create(uv[0], uv[1]);
						OPlistPush(ModelData.uvs, (ui8*)&vertexUV);
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
							OPlistPush(ModelData.uvs, (ui8*)&vertexUV);
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
						OPlistPush(ModelData.normals, (ui8*)&vertexNorm);
						//Display3DVector(header, normal);
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leNormal->GetIndexArray().GetAt(vertexId);
						normal = leNormal->GetDirectArray().GetAt(id);
						vertexNorm = OPvec3Create(normal[0], normal[1], normal[2]);
						OPlistPush(ModelData.normals, (ui8*)&vertexNorm);
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

void DisplayAnimationLayer(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher)
{
	int lModelCount;
	FbxString lOutputString;

	lOutputString = "     Node Name: ";
	lOutputString += pNode->GetName();
	lOutputString += "\n\n";
	FBXSDK_printf(lOutputString);

	//DisplayChannels(pNode, pAnimLayer, DisplayCurveKeys, DisplayListCurveKeys, isSwitcher);
	FBXSDK_printf("\n");

	for (lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
	{
		DisplayAnimationLayer(pAnimLayer, pNode->GetChild(lModelCount), isSwitcher);
	}
}

void DisplayAnimationStack(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher)
{
	int l;
	int nbAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
	FbxString lOutputString;

	lOutputString = "Animation stack contains ";
	lOutputString += nbAnimLayers;
	lOutputString += " Animation Layer(s)\n";
	FBXSDK_printf(lOutputString);

	for (l = 0; l < nbAnimLayers; l++)
	{
		FbxAnimLayer* lAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(l);

		lOutputString = "AnimLayer ";
		lOutputString += l;
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);

		DisplayAnimationLayer(lAnimLayer, pNode, isSwitcher);
	}
}

void DisplayAnimation(FbxScene* pScene)
{
	int i;
	for (i = 0; i < pScene->GetSrcObjectCount<FbxAnimStack>(); i++)
	{
		FbxAnimStack* lAnimStack = pScene->GetSrcObject<FbxAnimStack>(i);

		FbxString lOutputString = "Animation Stack Name: ";
		lOutputString += lAnimStack->GetName();
		lOutputString += "\n\n";
		FBXSDK_printf(lOutputString);

		DisplayAnimationStack(lAnimStack, pScene->GetRootNode(), true);
		DisplayAnimationStack(lAnimStack, pScene->GetRootNode(), false);
	}
}

void DisplayPose(FbxScene* pScene)
{
	int      i, j, k, lPoseCount;
	FbxString  lName;

	lPoseCount = pScene->GetPoseCount();

	for (i = 0; i < lPoseCount; i++)
	{
		FbxPose* lPose = pScene->GetPose(i);

		lName = lPose->GetName();
		OPlog("Pose Name: %s", lName.Buffer());

		OPlog("    Is a bind pose: %d", lPose->IsBindPose());

		OPlog("    Number of items in the pose: %d", lPose->GetCount());

		OPlog("");

		for (j = 0; j<lPose->GetCount(); j++)
		{
			lName = lPose->GetNodeName(j).GetCurrentName();
			OPlog("    Item name: %s", lName.Buffer());

			if (!lPose->IsBindPose())
			{
				// Rest pose can have local matrix
				DisplayBool("    Is local space matrix: ", lPose->IsLocalMatrix(j));
			}

			OPlog("    Matrix value: %s", "");

			FbxString lMatrixValue;

			for (k = 0; k<4; k++)
			{
				FbxMatrix  lMatrix = lPose->GetMatrix(j);
				FbxVector4 lRow = lMatrix.GetRow(k);
				char        lRowValue[1024];

				FBXSDK_sprintf(lRowValue, 1024, "%9.4f %9.4f %9.4f %9.4f\n", lRow[0], lRow[1], lRow[2], lRow[3]);
				lMatrixValue += FbxString("        ") + FbxString(lRowValue);
			}

			OPlog("%s", lMatrixValue.Buffer());
		}
	}

	lPoseCount = pScene->GetCharacterPoseCount();

	for (i = 0; i < lPoseCount; i++)
	{
		FbxCharacterPose* lPose = pScene->GetCharacterPose(i);
		FbxCharacter*     lCharacter = lPose->GetCharacter();

		if (!lCharacter) break;

		OPlog("Character Pose Name: %s", lCharacter->GetName());

		FbxCharacterLink lCharacterLink;
		FbxCharacter::ENodeId  lNodeId = FbxCharacter::eHips;

		while (lCharacter->GetCharacterLink(lNodeId, &lCharacterLink))
		{
			FbxAMatrix& lGlobalPosition = lCharacterLink.mNode->EvaluateGlobalTransform(FBXSDK_TIME_ZERO);

			OPlog("    Matrix value: %s", "");

			FbxString lMatrixValue;

			for (k = 0; k<4; k++)
			{
				FbxVector4 lRow = lGlobalPosition.GetRow(k);
				char        lRowValue[1024];

				FBXSDK_sprintf(lRowValue, 1024, "%9.4f %9.4f %9.4f %9.4f\n", lRow[0], lRow[1], lRow[2], lRow[3]);
				lMatrixValue += FbxString("        ") + FbxString(lRowValue);
			}

			OPlog("%s", lMatrixValue.Buffer());

			lNodeId = FbxCharacter::ENodeId(int(lNodeId) + 1);
		}
	}
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
				DisplayPolygons(pNode);
				break;
			
			case FbxNodeAttribute::eSkeleton:
				GlobalSkeleton = GetSkeleton(pNode);
				break;
		}
	}
}

enum ModelFeatures {
	Model_Positions = 0,
	Model_Normals,
	Model_UVs,
	Model_Colors,
	Model_Indices,
	Model_Bones,
	Model_Skinning,
	Model_Animations
};

void WriteFile(i8* output, OPint* features) {

	ofstream myFile(output, ios::binary);

	// OPM File Format Version
	writeU16(&myFile, 1);

	ui32 feature = 0;
	if (features[Model_Positions]) feature += 1;
	if (features[Model_Normals]) feature += 2;
	if (features[Model_UVs]) feature += 4;
	if (features[Model_Colors]) feature += 256;
	if (features[Model_Indices]) feature += 16;
	if (features[Model_Bones]) feature += 32;
	if (features[Model_Skinning]) feature += 64;
	if (features[Model_Animations]) feature += 128;

	// OPM File Features
	writeU32(&myFile, feature);

	// Vertex Count
	writeU32(&myFile, ModelData.VertexCount);

	for (i32 i = 0; i < ModelData.VertexCount; i++) {

		if (features[Model_Positions]) {
			OPvec3* pos = (OPvec3*)OPlistGet(ModelData.vertices, i);
			writeF32(&myFile, pos->x);
			writeF32(&myFile, pos->y);
			writeF32(&myFile, pos->z);
		}

		if (features[Model_Normals]) {
			OPvec3* norm = (OPvec3*)OPlistGet(ModelData.normals, i);
			writeF32(&myFile, norm->x);
			writeF32(&myFile, norm->y);
			writeF32(&myFile, norm->z);
		}

		if (features[Model_UVs]) {
			OPvec2* uv = (OPvec2*)OPlistGet(ModelData.uvs, i);
			writeF32(&myFile, uv->x);
			writeF32(&myFile, uv->y);
		}

		if (features[Model_Skinning]) {
			ui16* ind1 = (ui16*)OPlistGet(ModelData.boneIndices, i * 4 + 0);
			ui16* ind2 = (ui16*)OPlistGet(ModelData.boneIndices, i * 4 + 1);
			ui16* ind3 = (ui16*)OPlistGet(ModelData.boneIndices, i * 4 + 2);
			ui16* ind4 = (ui16*)OPlistGet(ModelData.boneIndices, i * 4 + 3);

			f32* w1 = (f32*)OPlistGet(ModelData.boneWeights, i * 4 + 0);
			f32* w2 = (f32*)OPlistGet(ModelData.boneWeights, i * 4 + 1);
			f32* w3 = (f32*)OPlistGet(ModelData.boneWeights, i * 4 + 2);
			f32* w4 = (f32*)OPlistGet(ModelData.boneWeights, i * 4 + 3);

			writeU16(&myFile, *ind1);
			writeU16(&myFile, *ind2);
			writeU16(&myFile, *ind3);
			writeU16(&myFile, *ind4);

			writeF32(&myFile, *w1);
			writeF32(&myFile, *w2);
			writeF32(&myFile, *w3);
			writeF32(&myFile, *w4);
		}
	}

	if (features[Model_Indices]) {
		writeU32(&myFile, ModelData.IndexCount / 3);

		for (i32 i = 0; i < ModelData.IndexCount; i++) {
			ui16* ind = (ui16*)OPlistGet(ModelData.indices, i);
			writeU16(&myFile, *ind);
		}
	}

	if (features[Model_Bones]) {
		WriteSkeleton(GlobalSkeleton, &myFile);
	}
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

int main(int argc, char **argv) {
	int arg;
	i8* filename = NULL;
	i8* output = NULL;
	

	// Fill arguments
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

	// Quit if no file was provided
	if (filename == NULL) {
		print_help();
		return -1;
	}

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

	// Initialize ModelData
	ModelData.VertexCount = 0;
	ModelData.ColorCount = 0;
	ModelData.IndexCount = 0;
	ModelData.NormalCount = 0;
	ModelData.TangentCount = 0;
	ModelData.UVCount = 0;
	ModelData.IndexPosition = 0;
	ModelData.vertices = OPlistCreate(128, sizeof(OPvec3));
	ModelData.normals = OPlistCreate(128, sizeof(OPvec3));
	ModelData.uvs = OPlistCreate(128, sizeof(OPvec2));
	ModelData.indices = OPlistCreate(128, sizeof(ui16));
	ModelData.boneIndices = OPlistCreate(128, sizeof(i16));
	ModelData.boneWeights = OPlistCreate(128, sizeof(f32));

	// Setup Autodesk FBX SDK
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

	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode) {
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
			FillNode(lRootNode->GetChild(i));
	}

	GetPoseInformation(lScene, GlobalSkeleton);

	// Write the OPM file
	if (output != NULL) WriteFile(output, features);
	else OPlog("No output file given.");
	
	lSdkManager->Destroy();

	return 0;
}