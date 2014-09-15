#include "./Core/include/Types.h"
#include "./Core/include/Log.h"

#include "fbxsdk.h"

void print_help()
{
	OPlog("Usage: FBXtoOPM [--help]");
}

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
void DisplayString(const char* pHeader)
{
	FbxString lString;

	lString = pHeader;
	lString += "\n";
	FBXSDK_printf(lString);
}


void DisplayBool(const char* pHeader, bool pValue)
{
	FbxString lString;

	lString = pHeader;
	lString += pValue ? "true" : "false";
	lString += "\n";
	FBXSDK_printf(lString);
}


void DisplayInt(const char* pHeader, int pValue)
{
	FbxString lString;

	lString = pHeader;
	lString += pValue;
	lString += "\n";
	FBXSDK_printf(lString);
}


void DisplayDouble(const char* pHeader, double pValue)
{
	FbxString lString;
	FbxString lFloatValue = (float)pValue;

	lFloatValue = pValue <= -HUGE_VAL ? "-INFINITY" : lFloatValue.Buffer();
	lFloatValue = pValue >= HUGE_VAL ? "INFINITY" : lFloatValue.Buffer();

	lString = pHeader;
	lString += lFloatValue;
	lString += "\n";
	FBXSDK_printf(lString);
}


void Display2DVector(const char* pHeader, FbxVector2 pValue)
{
	FbxString lString;
	FbxString lFloatValue1 = (float)pValue[0];
	FbxString lFloatValue2 = (float)pValue[1];

	lFloatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : lFloatValue1.Buffer();
	lFloatValue1 = pValue[0] >= HUGE_VAL ? "INFINITY" : lFloatValue1.Buffer();
	lFloatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : lFloatValue2.Buffer();
	lFloatValue2 = pValue[1] >= HUGE_VAL ? "INFINITY" : lFloatValue2.Buffer();

	lString = pHeader;
	lString += lFloatValue1;
	lString += ", ";
	lString += lFloatValue2;
	lString += "\n";
	FBXSDK_printf(lString);
}


void Display3DVector(const char* pHeader, FbxVector4 pValue)
{
	FbxString lString;
	FbxString lFloatValue1 = (float)pValue[0];
	FbxString lFloatValue2 = (float)pValue[1];
	FbxString lFloatValue3 = (float)pValue[2];

	lFloatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : lFloatValue1.Buffer();
	lFloatValue1 = pValue[0] >= HUGE_VAL ? "INFINITY" : lFloatValue1.Buffer();
	lFloatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : lFloatValue2.Buffer();
	lFloatValue2 = pValue[1] >= HUGE_VAL ? "INFINITY" : lFloatValue2.Buffer();
	lFloatValue3 = pValue[2] <= -HUGE_VAL ? "-INFINITY" : lFloatValue3.Buffer();
	lFloatValue3 = pValue[2] >= HUGE_VAL ? "INFINITY" : lFloatValue3.Buffer();

	lString = pHeader;
	lString += lFloatValue1;
	lString += ", ";
	lString += lFloatValue2;
	lString += ", ";
	lString += lFloatValue3;
	lString += "\n";
	FBXSDK_printf(lString);
}

void Display4DVector(const char* pHeader, FbxVector4 pValue)
{
	FbxString lString;
	FbxString lFloatValue1 = (float)pValue[0];
	FbxString lFloatValue2 = (float)pValue[1];
	FbxString lFloatValue3 = (float)pValue[2];
	FbxString lFloatValue4 = (float)pValue[3];

	lFloatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : lFloatValue1.Buffer();
	lFloatValue1 = pValue[0] >= HUGE_VAL ? "INFINITY" : lFloatValue1.Buffer();
	lFloatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : lFloatValue2.Buffer();
	lFloatValue2 = pValue[1] >= HUGE_VAL ? "INFINITY" : lFloatValue2.Buffer();
	lFloatValue3 = pValue[2] <= -HUGE_VAL ? "-INFINITY" : lFloatValue3.Buffer();
	lFloatValue3 = pValue[2] >= HUGE_VAL ? "INFINITY" : lFloatValue3.Buffer();
	lFloatValue4 = pValue[3] <= -HUGE_VAL ? "-INFINITY" : lFloatValue4.Buffer();
	lFloatValue4 = pValue[3] >= HUGE_VAL ? "INFINITY" : lFloatValue4.Buffer();

	lString = pHeader;
	lString += lFloatValue1;
	lString += ", ";
	lString += lFloatValue2;
	lString += ", ";
	lString += lFloatValue3;
	lString += ", ";
	lString += lFloatValue4;
	lString += "\n";
	FBXSDK_printf(lString);
}


//void DisplayColor(const char* pHeader, FbxPropertyT<FbxDouble3> pValue, const char* pSuffix /* = "" */)
//
//{
//	FbxString lString;
//
//	lString = pHeader;
//	//lString += (float) pValue.mRed;
//	//lString += (double)pValue.GetArrayItem(0);
//	lString += " (red), ";
//	//lString += (float) pValue.mGreen;
//	//lString += (double)pValue.GetArrayItem(1);
//	lString += " (green), ";
//	//lString += (float) pValue.mBlue;
//	//lString += (double)pValue.GetArrayItem(2);
//	lString += " (blue)";
//	lString += pSuffix;
//	lString += "\n";
//	FBXSDK_printf(lString);
//}
//

void DisplayColor(const char* pHeader, FbxColor pValue)
{
	FbxString lString;

	lString = pHeader;
	lString += (float)pValue.mRed;

	lString += " (red), ";
	lString += (float)pValue.mGreen;

	lString += " (green), ";
	lString += (float)pValue.mBlue;

	lString += " (blue)";
	lString += "\n";
	FBXSDK_printf(lString);
}
/**
* Return a string-based representation based on the attribute type.
*/
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
	switch (type) {
	case FbxNodeAttribute::eUnknown: return "unidentified";
	case FbxNodeAttribute::eNull: return "null";
	case FbxNodeAttribute::eMarker: return "marker";
	case FbxNodeAttribute::eSkeleton: return "skeleton";
	case FbxNodeAttribute::eMesh: return "mesh";
	case FbxNodeAttribute::eNurbs: return "nurbs";
	case FbxNodeAttribute::ePatch: return "patch";
	case FbxNodeAttribute::eCamera: return "camera";
	case FbxNodeAttribute::eCameraStereo: return "stereo";
	case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
	case FbxNodeAttribute::eLight: return "light";
	case FbxNodeAttribute::eOpticalReference: return "optical reference";
	case FbxNodeAttribute::eOpticalMarker: return "marker";
	case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
	case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
	case FbxNodeAttribute::eBoundary: return "boundary";
	case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
	case FbxNodeAttribute::eShape: return "shape";
	case FbxNodeAttribute::eLODGroup: return "lodgroup";
	case FbxNodeAttribute::eSubDiv: return "subdiv";
	default: return "unknown";
	}
}

/**
* Print an attribute.
*/
void PrintAttribute(FbxNodeAttribute* pAttribute) {
	if (!pAttribute) return;

	FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
	FbxString attrName = pAttribute->GetName();
	// Note: to retrieve the character array of a FbxString, use its Buffer() method.
	printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}


void DisplayMetaDataConnections(FbxObject* pObject)
{
	int nbMetaData = pObject->GetSrcObjectCount<FbxObjectMetaData>();
	if (nbMetaData > 0)
		OPlog("    MetaData connections ");

	for (int i = 0; i < nbMetaData; i++)
	{
		FbxObjectMetaData* metaData = pObject->GetSrcObject<FbxObjectMetaData>(i);
		OPlog("        Name: ", (char*)metaData->GetName());
	}
}

void DisplayControlsPoints(FbxMesh* pMesh)
{
	int i, lControlPointsCount = pMesh->GetControlPointsCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();

	OPlog("    Control Points");

	for (i = 0; i < lControlPointsCount; i++)
	{
		OPlog("        Control Point %d", i);
		OPlog("            Coordinates: %f, %f, %f", lControlPoints[i][0], lControlPoints[i][1], lControlPoints[i][2]);

		for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
		{
			FbxGeometryElementNormal* leNormals = pMesh->GetElementNormal(j);
			if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				char header[100];
				FBXSDK_sprintf(header, 100, "            Normal Vector: %f, %f, %f");
				if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect){
					FbxVector4 vec = leNormals->GetDirectArray().GetAt(i);
					OPlog(header, vec[0], vec[1], vec[2]);
				}
			}
		}
	}

	OPlog("");
}

#include "./Math/include/Vector3.h"
#include "./Math/include/Vector2.h"

OPvec3* vertices, *normals, *tangents;
OPvec2* uvs;
ui16* indices;

i32 VertexCount, ColorCount, UVCount, NormalCount, TangentCount, IndexCount;

void DisplayPolygons(FbxMesh* pMesh)
{
	int i, j, lPolygonCount = pMesh->GetPolygonCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	char header[100];

	VertexCount = 0;
	ColorCount = 0;
	UVCount = 0;
	NormalCount = 0;
	TangentCount = 0;
	IndexCount = 0;

	for (i = 0; i < lPolygonCount; i++)
	{
		int l;
		int lPolygonSize = pMesh->GetPolygonSize(i);
		if (lPolygonSize == 3) {
			IndexCount += 3;
		}
		else if (lPolygonSize == 4) {
			IndexCount += 6;
		}
		VertexCount += lPolygonSize;
		ColorCount += lPolygonSize * pMesh->GetElementVertexColorCount();
		UVCount += lPolygonSize * pMesh->GetElementUVCount();
		NormalCount += lPolygonSize * pMesh->GetElementNormalCount();
		TangentCount += lPolygonSize * pMesh->GetElementTangentCount();
	}

	vertices = (OPvec3*)OPalloc(sizeof(OPvec3)* VertexCount);
	normals = (OPvec3*)OPalloc(sizeof(OPvec3)* NormalCount);
	uvs = (OPvec2*)OPalloc(sizeof(OPvec2)* UVCount);
	indices = (ui16*)OPalloc(sizeof(ui16)* IndexCount);

	int vertexId = 0;
	i32 pos = -1;
	i32 ind = 0;
	for (i = 0; i < lPolygonCount; i++)
	{
		DisplayInt("        Polygon ", i);
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
			indices[ind+0] = pos + 1;
			indices[ind+1] = pos + 2;
			indices[ind+2] = pos + 3;
			ind += 3;
		}
		else if (lPolygonSize == 4) {
			indices[ind+0] = pos + 1;
			indices[ind+1] = pos + 2;
			indices[ind+2] = pos + 3;

			indices[ind+3] = pos + 1;
			indices[ind+4] = pos + 3;
			indices[ind+5] = pos + 4;
			ind += 6;
		}

		for (j = 0; j < lPolygonSize; j++)
		{
			pos++;

			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);

			Display3DVector("            Coordinates: ", lControlPoints[lControlPointIndex]);

			vertices[pos].x = (float)lControlPoints[lControlPointIndex][0];
			vertices[pos].y = (float)lControlPoints[lControlPointIndex][1];
			vertices[pos].z = (float)lControlPoints[lControlPointIndex][2];

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
				FBXSDK_sprintf(header, 100, "            Texture UV: ");

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
						uvs[pos].x = uv[0];
						uvs[pos].y = uv[1];
						Display2DVector(header, leUV->GetDirectArray().GetAt(lControlPointIndex));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
						uv = leUV->GetDirectArray().GetAt(id);
						uvs[pos].x = uv[0];
						uvs[pos].y = uv[1];
						Display2DVector(header, leUV->GetDirectArray().GetAt(id));
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
							uvs[pos].x = uv[0];
							uvs[pos].y = uv[1];
							Display2DVector(header, leUV->GetDirectArray().GetAt(lTextureUVIndex));
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
				FBXSDK_sprintf(header, 100, "            Normal: ");

				FbxVector4 normal;
				if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						normal = leNormal->GetDirectArray().GetAt(vertexId);
						normals[pos].x = normal[0];
						normals[pos].y = normal[1];
						normals[pos].z = normal[2];
						Display3DVector(header, normal);
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leNormal->GetIndexArray().GetAt(vertexId);
						normal = leNormal->GetDirectArray().GetAt(id);
						normals[pos].x = normal[0];
						normals[pos].y = normal[1];
						normals[pos].z = normal[2];
						Display3DVector(header, normal);
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
void PrintNode(FbxNode* pNode) {
	FbxNodeAttribute::EType lAttributeType;
	int i;

	if (pNode->GetNodeAttribute() == NULL)
	{
		FBXSDK_printf("NULL Node Attribute\n\n");
		return;
	}
	else
	{
		lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

		switch (lAttributeType)
		{
		default:
			break;
		case FbxNodeAttribute::eMesh:
			FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();
			DisplayPolygons(lMesh);

			break;
		}
	}
}

int main(int argc, char **argv) {
	int arg;
	i8* filename = NULL;
	i8* output = NULL;

	if (argc == 1) {
		print_help();
	}
	else {
		for (arg = 1; arg < argc; ++arg)
		{
			if (0 == strcmp("--help", argv[arg]) || 0 == strcmp("-h", argv[arg]))
			{
				print_help();
				exit(1);
			}

			if (0 == strcmp("--font", argv[arg]) || 0 == strcmp("-f", argv[arg]))
			{
				++arg;

				if (filename)
				{
					fprintf(stderr, "Multiple --font parameters.\n");
					print_help();
					exit(1);
				}

				if (arg >= argc)
				{
					fprintf(stderr, "No font file given.\n");
					print_help();
					exit(1);
				}

				filename = argv[arg];
				continue;
			}

			if (0 == strcmp("--output", argv[arg]) || 0 == strcmp("-o", argv[arg]))
			{
				++arg;

				if (output)
				{
					fprintf(stderr, "Multiple --font parameters.\n");
					print_help();
					exit(1);
				}

				if (arg >= argc)
				{
					fprintf(stderr, "No font file given.\n");
					print_help();
					exit(1);
				}

				output = argv[arg];
				continue;
			}
		}
	}

	if (filename == NULL) {
		print_help();
		return 0;
	}

	FbxManager* lSdkManager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
	if (!lImporter->Initialize(filename, -1, lSdkManager->GetIOSettings())) {
		OPlog("Call to FbxImporter::Initialize() failed.\n");
		OPlog("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		return -1;
	}

	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
	lImporter->Import(lScene);
	lImporter->Destroy();

	FbxDocumentInfo* sceneInfo = lScene->GetSceneInfo();    if (sceneInfo)
	{
		//FBXSDK_printf("--------------------\nMeta-Data\n--------------------\n");
		//FBXSDK_printf("    Title: %s\n", sceneInfo->mTitle.Buffer());
		//FBXSDK_printf("    Subject: %s\n", sceneInfo->mSubject.Buffer());
		//FBXSDK_printf("    Author: %s\n", sceneInfo->mAuthor.Buffer());
		//FBXSDK_printf("    Keywords: %s\n", sceneInfo->mKeywords.Buffer());
		//FBXSDK_printf("    Revision: %s\n", sceneInfo->mRevision.Buffer());
		//FBXSDK_printf("    Comment: %s\n", sceneInfo->mComment.Buffer());

		FbxNode* lRootNode = lScene->GetRootNode();
		if (lRootNode) {
			for (int i = 0; i < lRootNode->GetChildCount(); i++)
				PrintNode(lRootNode->GetChild(i));
		}

		if (output != NULL) {
			ofstream myFile(output, ios::binary);
			writeU16(&myFile, 1);					// Version

			writeU32(&myFile, 1 + 2 + 4 + 16);		// Features

			writeU32(&myFile, VertexCount);

			for (i32 i = 0; i < VertexCount; i++) {
				writeF32(&myFile, vertices[i].x);
				writeF32(&myFile, vertices[i].y);
				writeF32(&myFile, vertices[i].z);
				OPlog("%f %f %f", vertices[i].x, vertices[i].y, vertices[i].z);

				writeF32(&myFile, normals[i].x);
				writeF32(&myFile, normals[i].y);
				writeF32(&myFile, normals[i].z);

				writeF32(&myFile, uvs[i].x);
				writeF32(&myFile, uvs[i].y);
			}

			writeU32(&myFile, IndexCount / 3);

			for (i32 i = 0; i < IndexCount; i++) {
				writeU16(&myFile, indices[i]);
			}
		}
	}

	lSdkManager->Destroy();

	return 0;
}