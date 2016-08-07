#pragma once
#include "OPfbxSdk.h"

typedef struct {
	OPint Size;
	OPint ControlPointIndex[4];
	OPvec3 Position[4];
	OPvec3 Normal[4];
	OPvec4 Tangent[4];
	OPvec4 BiTangent[4];
	OPvec2 UV[4];
} OPfbxMeshPoly;


typedef struct {
	OPchar* Name;
	OPchar* Type;
	OPvec3 Position;
	OPvec3 Rotation;
	OPvec3 Scale;
} OPfbxMeshDataMeta;

typedef struct {
	FbxNode* Node;
	FbxMesh* Mesh;
	OPfbxMeshDataMeta* Meta;

	OPint MetaCount;

	OPint PolyCount;

	OPint VertexCount;
	OPint IndexCount;

	OPint ColorCount;
	OPint UVCount;
	OPint NormalCount;
	OPint TangentCount;
} OPfbxMeshData;

OPfbxMeshPoly* OPfbxMeshDataGetPolygons(OPfbxMeshData* meshData, OPfloat scale) {

	OPfbxMeshPoly* polys = (OPfbxMeshPoly*)OPalloc(sizeof(OPfbxMeshPoly)* meshData->PolyCount);

	FbxVector4* controlPoints = meshData->Mesh->GetControlPoints();
	
	OPint vertId = 0;
	OPint polySize;
	for (OPint i = 0; i < meshData->PolyCount; i++) {
		polySize = meshData->Mesh->GetPolygonSize(i);
		polys[i].Size = polySize;

		for (OPint j = 0; j < polySize; j++) {

			int controlPointIndex = meshData->Mesh->GetPolygonVertex(i, j);

			polys[i].ControlPointIndex[j] = controlPointIndex;
			polys[i].Position[j] = OPvec3Create(
				(f32)controlPoints[controlPointIndex][0] * scale,
				(f32)controlPoints[controlPointIndex][1] * scale,
				(f32)controlPoints[controlPointIndex][2] * scale
				);

			// Only support for 1 normal currently
			for (OPint k = 0; k < meshData->Mesh->GetElementNormalCount() && k <= 1; ++k) {

				FbxGeometryElementNormal* eNormal = meshData->Mesh->GetElementNormal(k);

				FbxVector4 normal;
				if (eNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {

					switch (eNormal->GetReferenceMode())
					{
						case FbxGeometryElement::eDirect: {
							normal = eNormal->GetDirectArray().GetAt(vertId);
							OPlog("Normal %f, %f, %f, %f", normal[0], normal[1], normal[2], normal[3]);
							break;
						}
						case FbxGeometryElement::eIndexToDirect: {
							int id = eNormal->GetIndexArray().GetAt(0);
							normal = eNormal->GetDirectArray().GetAt(id);
							OPlog("Normal2 %f, %f, %f, %f", normal[0], normal[1], normal[2], normal[3]);
							break;
						}
					}
					polys[i].Normal[j] = OPvec3Create(normal[0], normal[1], normal[2]);
					OPvec3Log("Normal3", polys[i].Normal[j]);
				} else if(eNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint) {

					
					switch(eNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						polys[i].Normal[j].x = static_cast<float>(eNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
						polys[i].Normal[j].y = static_cast<float>(eNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
						polys[i].Normal[j].z = static_cast<float>(eNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
					
						OPvec3Log("Normal4", polys[i].Normal[j]);
					}
					break;

					case FbxGeometryElement::eIndexToDirect:
					{
						int index = eNormal->GetIndexArray().GetAt(controlPointIndex);
						polys[i].Normal[j].x = static_cast<float>(eNormal->GetDirectArray().GetAt(index).mData[0]);
						polys[i].Normal[j].y = static_cast<float>(eNormal->GetDirectArray().GetAt(index).mData[1]);
						polys[i].Normal[j].z = static_cast<float>(eNormal->GetDirectArray().GetAt(index).mData[2]);
					
						OPvec3Log("Normal5", polys[i].Normal[j]);
					}
					break;

					default:
					OPlog("Invalid Reference");
					}
					break;
				} else {
					OPlog("No Normal");
				}
			}


			// NOTE - Only supporting 1 UV per vertex
			FbxVector2 uv;
			for (OPint k = 0; k < meshData->Mesh->GetElementUVCount(); ++k)
			{
				FbxGeometryElementUV* eUV = meshData->Mesh->GetElementUV(k);

				switch (eUV->GetMappingMode()) {

					case FbxGeometryElement::eByControlPoint: {
						switch (eUV->GetReferenceMode())
						{
							case FbxGeometryElement::eDirect: {
								//OPlg("+");
								uv = eUV->GetDirectArray().GetAt(controlPointIndex);
								break;
							}
							case FbxGeometryElement::eIndexToDirect: {
								//OPlg("-");
								int id = eUV->GetIndexArray().GetAt(controlPointIndex);
								uv = eUV->GetDirectArray().GetAt(id);
								break;
							}
							default: {
								OPlogDebug("WARNING: Unsupported UV Control Point Type");
								break;
							}
						}
					}

					case FbxGeometryElement::eByPolygonVertex: {
						switch (eUV->GetReferenceMode())
						{
							case FbxGeometryElement::eDirect:
							case FbxGeometryElement::eIndexToDirect:
							{
								OPint textureUVIndex = meshData->Mesh->GetTextureUVIndex(i, j);
								//OPlg("*");
								OPlogDebug("UV Index: %d", textureUVIndex);
								uv = eUV->GetDirectArray().GetAt(textureUVIndex);
								break;
							}
							default:
								OPlogDebug("WARNING: Unsupported UV Polygon Vertex Type");
								break;
						}
					}

					case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
					case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
					case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
						break;

					default: {
								 OPlogDebug("WARNING: Unsupported UV Mapping Mode");
								 break;
					}
				}

				polys[i].UV[j] = OPvec2(uv[0], uv[1]);
			}

			FbxVector4 tangent;
			for (OPint l = 0; l < meshData->Mesh->GetElementTangentCount(); ++l)
			{
				FbxGeometryElementTangent* eTangent = meshData->Mesh->GetElementTangent(l);

				if (eTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (eTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect: {
						tangent = eTangent->GetDirectArray().GetAt(controlPointIndex);
						break;
					}
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = eTangent->GetIndexArray().GetAt(controlPointIndex);
						tangent = eTangent->GetDirectArray().GetAt(id);
						break;
					}
					default:
						break; // other reference modes not shown here!
					}
				}
				polys[i].Tangent[j] = OPvec4Create(tangent[0], tangent[1], tangent[2], tangent[3]);
			}

			FbxVector4 bitangent;
			for (OPint l = 0; l < meshData->Mesh->GetElementBinormalCount(); ++l)
			{
				FbxGeometryElementBinormal* eBinormal = meshData->Mesh->GetElementBinormal(l);

				if (eBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (eBinormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect: {
						bitangent = eBinormal->GetDirectArray().GetAt(controlPointIndex);
						break;
					}
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = eBinormal->GetIndexArray().GetAt(controlPointIndex);
						bitangent = eBinormal->GetDirectArray().GetAt(id);
						break;
					}
					default:
						break; // other reference modes not shown here!
					}
				}
				polys[i].BiTangent[j] = OPvec4Create(bitangent[0], bitangent[1], bitangent[2], bitangent[3]);
			}


			// TODO(garrett): COLOR
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
			vertId++; 
		}
	}


	OPlogDebug("========= FBX Polys =========\n");
	for (OPint i = 0; i < meshData->PolyCount; i++) {
		OPlogDebug("Size: %d", polys[i].Size);
		for (OPint j = 0; j < polys[i].Size; j++) {
			OPlogDebug("Ind: %d", polys[i].ControlPointIndex[j]);
			//OPvec3Log("Pos: ", polys[i].Position[j]);
			//OPvec3Log("Norm: ", polys[i].Normal[j]);
			//OPvec2Log("UV: ", polys[i].UV[j]);
		}
	}

	return polys;
}

OPint _meshVertexCount(FbxMesh* fbxMesh) {
	OPint vertexCount = 0;

	OPint polyCount = fbxMesh->GetPolygonCount();

	for (OPint i = 0; i < polyCount; i++) {
		vertexCount += fbxMesh->GetPolygonSize(i);
	}

	return vertexCount;
}

OPint _meshIndexCount(FbxMesh* fbxMesh) {
	OPint indexCount = 0;

	OPint polyCount = fbxMesh->GetPolygonCount();

	for (OPint i = 0; i < polyCount; i++) {
		OPint size = fbxMesh->GetPolygonSize(i);
		if(size == 3) {
			indexCount += 3;
		} else if(size == 4) {
			indexCount += 6;
		} else {
			OPlogDebug("\tWARNING: Unsupported polygon size!");
		}
	}

	return indexCount;
}

ui32* OPfbxMeshDataGetIndices(OPfbxMeshData* meshData) {
	ui32* indices = (ui32*)OPalloc(sizeof(ui32) * meshData->IndexCount);

	OPint pos = 0;
	OPint indexOffset = 0;

	OPint polySize;
	for(OPint i = 0; i < meshData->PolyCount; i++) {
		polySize = meshData->Mesh->GetPolygonSize(i);

		if(polySize == 3 ){
			indices[pos+0] = indexOffset + 0;
			indices[pos+1] = indexOffset + 1;
			indices[pos+2] = indexOffset + 2;
			OPlogDebug("Ind: %d %d %d",indices[pos+0], indices[pos+1],indices[pos+2]);
			pos += 3;
			indexOffset += 3;
		} else if(polySize == 4) {
			indices[pos+0] = indexOffset + 0;
			indices[pos+1] = indexOffset + 1;
			indices[pos+2] = indexOffset + 2;

			indices[pos+3] = indexOffset + 0;
			indices[pos+4] = indexOffset + 2;
			indices[pos+5] = indexOffset + 3;

			OPlogDebug("Ind: %d %d %d %d %d %d", indices[pos+0], indices[pos+1],indices[pos+2], indices[pos+3], indices[pos+4],indices[pos+5]);
			pos += 6;
			indexOffset += 4;
		} else {
			OPlogDebug("WARNING: Unsupported polysize: %d", polySize);
		}
	}

	OPlogDebug("Pos: %d, Ind Count: %d", pos, meshData->IndexCount);

	return indices;
}

OPvec3* OPfbxMeshDataGetPositions(OPfbxMeshData* meshData) {
	OPvec3* positions = (OPvec3*)OPalloc(sizeof(OPvec3) * meshData->VertexCount);

	FbxVector4* controlPoints = meshData->Mesh->GetControlPoints();

	OPint pos = 0;

	OPint polySize;
	for(OPint i = 0; i < meshData->PolyCount; i++) {
		polySize = meshData->Mesh->GetPolygonSize(i);

		for(OPint j = 0; j < polySize; j++) {

			int controlPointIndex = meshData->Mesh->GetPolygonVertex(i, j);

			positions[pos] = OPvec3Create(
				(f32)controlPoints[controlPointIndex][0],
				(f32)controlPoints[controlPointIndex][1],
				(f32)controlPoints[controlPointIndex][2]
				);

			//OPvec3Log("Pos: ", positions[pos]);

			pos++;
		}
	}
	OPlogDebug("Pos: %d, Vert Count: %d", pos, meshData->VertexCount);

	return positions;
}

OPvec3* OPfbxMeshDataGetNormals(OPfbxMeshData* meshData) {
	OPvec3* normals = (OPvec3*)OPalloc(sizeof(OPvec3) * meshData->NormalCount);

	OPint pos = 0;
	OPint vertexInd = 0;

	OPint polySize;
	for(OPint i = 0; i < meshData->PolyCount; i++) {
		polySize = meshData->Mesh->GetPolygonSize(i);

		for(OPint j = 0; j < polySize; j++) {

			for (OPint k = 0; k < meshData->Mesh->GetElementNormalCount(); ++k) {

				FbxGeometryElementNormal* eNormal = meshData->Mesh->GetElementNormal(k);

				FbxVector4 normal;
				if (eNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {

					switch (eNormal->GetReferenceMode())
					{
						case FbxGeometryElement::eDirect: {
							normal = eNormal->GetDirectArray().GetAt(vertexInd);
							break;
						}
						case FbxGeometryElement::eIndexToDirect: {
							int id = eNormal->GetIndexArray().GetAt(vertexInd);
							normal = eNormal->GetDirectArray().GetAt(id);
							break;
						}
					}

					normals[pos] = OPvec3Create( normal[0], normal[1], normal[2] );

					//OPvec3Log("Normal: ", normals[pos]);

					pos++;
				}
			}

			vertexInd++;
		}

	}
	OPlogDebug("Pos: %d, Norm Count: %d", pos, meshData->NormalCount);

	return normals;
}

OPvec2* OPfbxMeshDataGetUVs(OPfbxMeshData* meshData) {
	if(meshData->UVCount == 0) return NULL;
	OPlogDebug("UV Count: %d", meshData->UVCount);

	OPvec2* uvs = (OPvec2*)OPalloc(sizeof(OPvec2) * meshData->UVCount);

	OPint pos = 0;

	OPint polySize;
	FbxVector2 uv;
	for(OPint i = 0; i < meshData->PolyCount; i++) {
		polySize = meshData->Mesh->GetPolygonSize(i);

		for(OPint j = 0; j < polySize; j++) {
			int controlPointIndex = meshData->Mesh->GetPolygonVertex(i, j);

			// NOTE - Only supporting 1 UV per vertex
			for (OPint k = 0; k < meshData->Mesh->GetElementUVCount(); ++k)
			{
				FbxGeometryElementUV* eUV = meshData->Mesh->GetElementUV(k);

				switch (eUV->GetMappingMode()) {

					case FbxGeometryElement::eByControlPoint: {
						switch (eUV->GetReferenceMode())
						{
							case FbxGeometryElement::eDirect: {
								OPlg("+");
								uv = eUV->GetDirectArray().GetAt(controlPointIndex);
								break;
							}
							case FbxGeometryElement::eIndexToDirect: {
								OPlg("-");
								int id = eUV->GetIndexArray().GetAt(controlPointIndex);
								uv = eUV->GetDirectArray().GetAt(id);
								break;
							}
							default: {
								OPlogDebug("WARNING: Unsupported UV Control Point Type");
								break;
							}
						}
					}

					case FbxGeometryElement::eByPolygonVertex: {					
						switch (eUV->GetReferenceMode())
						{
							case FbxGeometryElement::eDirect:
							case FbxGeometryElement::eIndexToDirect:
							{
								OPint textureUVIndex = meshData->Mesh->GetTextureUVIndex(i, j);	
								//OPlg("*");
								OPlogDebug("UV Index: %d",textureUVIndex );
								uv = eUV->GetDirectArray().GetAt(textureUVIndex);
								break;
							}
							default:
								OPlogDebug("WARNING: Unsupported UV Polygon Vertex Type");
								break;
						}
					}

					case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
					case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
					case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
						break;

					default: {
						OPlogDebug("WARNING: Unsupported UV Mapping Mode");
						break;
					}
				}

				uvs[pos] = OPvec2( uv[0], uv[1] );
				//OPvec2Log("UV: ", uvs[pos]);
				pos++;
			}


		}
	}

	OPlogDebug("Pos: %d, UV Count: %d", pos, meshData->UVCount);

	if(pos != meshData->UVCount) {
		OPlogDebug("WARNING: Not all UVs were mapped.");
	}

	return uvs;
}

OPint OPfbxMeshDataGetPropBool(FbxObject* pObject, OPchar* name, OPint* val) {
    FbxProperty lProperty = pObject->GetFirstProperty();
    while (lProperty.IsValid())
    {
        if (lProperty.GetFlag(FbxPropertyFlags::eUserDefined))
        {
            FbxString lString = lProperty.GetLabel();
            if(OPstringEquals(lString.Buffer(), name)) {
				FbxDataType lPropertyDataType=lProperty.GetPropertyDataType();

				if (lPropertyDataType.GetType() == eFbxBool)
            	{
            		(*val) = lProperty.Get<FbxBool>();
                	return 1;
                }
            	return 0;
            }
        }
        lProperty = pObject->GetNextProperty(lProperty);
    }
    return 0;
}

OPint OPfbxMeshDataGetPropString(FbxObject* pObject, OPchar* name, OPchar** val) {
    FbxProperty lProperty = pObject->GetFirstProperty();
    while (lProperty.IsValid())
    {
        if (lProperty.GetFlag(FbxPropertyFlags::eUserDefined))
        {
            FbxString lString = lProperty.GetLabel();
            if(OPstringEquals(lString.Buffer(), name)) {
				FbxDataType lPropertyDataType=lProperty.GetPropertyDataType();

				if (lPropertyDataType.GetType() == eFbxString)
            	{
            		FbxString v = lProperty.Get<FbxString>();
            		(*val) = OPstringCopy(v.Buffer());
                	return 1;
                }
            	return 0;
            }
        }
        lProperty = pObject->GetNextProperty(lProperty);
    }
    return 0;
}

// void DisplayUserProperties(FbxObject* pObject)
// {
//     int lCount = 0;
//     FbxString lTitleStr = "    Property Count: ";

//     FbxProperty lProperty = pObject->GetFirstProperty();
//     while (lProperty.IsValid())
//     {
//         if (lProperty.GetFlag(FbxPropertyFlags::eUserDefined))
//             lCount++;

//         lProperty = pObject->GetNextProperty(lProperty);
//     }

//     if (lCount == 0)
//         return; // there are no user properties to display

//     OPlog("%s %d", lTitleStr.Buffer(), lCount);

//     lProperty = pObject->GetFirstProperty();
//     int i = 0;
//     while (lProperty.IsValid())
//     {
//         if (lProperty.GetFlag(FbxPropertyFlags::eUserDefined))
//         {
//             OPlog("        Property %d", i);
//             FbxString lString = lProperty.GetLabel();
//             OPlog("            Display Name: %s", lString.Buffer());
//             lString = lProperty.GetName();
//             OPlog("            Internal Name: %s", lString.Buffer());
//             OPlog("            Type: %s", lProperty.GetPropertyDataType().GetName());
//             if (lProperty.HasMinLimit()) OPlog("            Min Limit: %llu", lProperty.GetMinLimit());
//             if (lProperty.HasMaxLimit()) OPlog("            Max Limit: %llu", lProperty.GetMaxLimit());
//             OPlog  ("            Is Animatable: %d", lProperty.GetFlag(FbxPropertyFlags::eAnimatable));
            
//                         FbxDataType lPropertyDataType=lProperty.GetPropertyDataType();

//                         // BOOL
//                         if (lPropertyDataType.GetType() == eFbxBool)
//             {
//                 OPlog("            Default Value: %d", lProperty.Get<FbxBool>());
//                         }
//                         // REAL
//                         else if (lPropertyDataType.GetType() == eFbxDouble || lPropertyDataType.GetType() == eFbxFloat)
//                         {
//                 OPlog("            Default Value: %f", lProperty.Get<FbxDouble>());
//                         }
//                         // COLOR
//                         else if (lPropertyDataType.Is(FbxColor3DT) || lPropertyDataType.Is(FbxColor4DT))
//             {
//                                 FbxColor lDefault;
//                 char      lBuf[64];

//                 lDefault = lProperty.Get<FbxColor>();
//                 FBXSDK_sprintf(lBuf, 64, "R=%f, G=%f, B=%f, A=%f", lDefault.mRed, lDefault.mGreen, lDefault.mBlue, lDefault.mAlpha);
//                 OPlog("            Default Value: %s", lBuf);
//             }
//                         // INTEGER
//                         else if (lPropertyDataType.GetType() == eFbxInt)
//                         {
//                 OPlog("            Default Value: %d", lProperty.Get<FbxInt>());
//                         }
//                         // VECTOR
//                         else if(lPropertyDataType.GetType() == eFbxDouble3 || lPropertyDataType.GetType() == eFbxDouble4)
//                         {
//                                 FbxDouble3 lDefault;
//                 char   lBuf[64];

//                 lDefault = lProperty.Get<FbxDouble3>();
//                 FBXSDK_sprintf(lBuf, 64, "X=%f, Y=%f, Z=%f", lDefault[0], lDefault[1], lDefault[2]);
//                 OPlog("            Default Value: %s", lBuf);
//             }
//                         // LIST
//                         else if (lPropertyDataType.GetType() == eFbxEnum)
//                         {
//                 OPlog("            Default Value: %d", lProperty.Get<FbxEnum>());
//                         }
//                         // UNIDENTIFIED
//             else
//                         {
//                 OPlog("            Default Value: UNIDENTIFIED");
//             }
//             i++;
//         }

//         lProperty = pObject->GetNextProperty(lProperty);
//     }
// }

OPint OPfbxMeshDataIsMeta(FbxObject* pObject) {
	OPint isMeta = 0;
	if(OPfbxMeshDataGetPropBool(pObject, "OPIFEX_META", &isMeta)) {
		OPlog("Found property OPIFEX_META");
	}
	return isMeta;
}

OPint OPfbxMeshDataMetaHasProp(FbxObject* pObject, OPchar* name) {    
	FbxProperty lProperty = pObject->GetFirstProperty();
    while (lProperty.IsValid())
    {
        if (lProperty.GetFlag(FbxPropertyFlags::eUserDefined))
        {
            FbxString lString = lProperty.GetLabel();
            if(OPstringEquals(lString.Buffer(), name)) {
            	return 1;
            }
        }
        lProperty = pObject->GetNextProperty(lProperty);
    }
	return 0;
}

OPchar* OPfbxMeshDataMetaName(FbxObject* pObject) {
	OPchar* result = NULL;
	OPfbxMeshDataGetPropString(pObject, "OPIFEX_NAME", &result);
	return result;
}

OPchar* OPfbxMeshDataMetaType(FbxObject* pObject) {
	OPchar* result = NULL;
	OPfbxMeshDataGetPropString(pObject, "OPIFEX_TYPE", &result);
	return result;
}

OPint OPfbxMeshDataGet(OPfbxMeshData* meshData, OPfbxScene* scene) {

	// Loop through each child in the root node of the scene until 
	// we find the first available mesh (Only supporting 1 mesh)
	// TODO: Support multiple meshes
	FbxNodeAttribute::EType attributeType;
	FbxNode* node;
	OPint isMeta = 0;
	OPuint totalMeta = 0;

	// find all properties on all of the nodes
	OPlog("Meta Data");
	for (int i = 0; i < scene->RootNode->GetChildCount(); i++) {
		node = scene->RootNode->GetChild(i);
		if(OPfbxMeshDataIsMeta(node)) {
			totalMeta++;
		}
	}

	OPlog("Total Meta: %lld", totalMeta);

	if(totalMeta > 0) {
		meshData->Meta = (OPfbxMeshDataMeta*)OPalloc(sizeof(OPfbxMeshDataMeta) * totalMeta);
		meshData->MetaCount = totalMeta;
		OPint metaInd = 0;
		for (int i = 0; i < scene->RootNode->GetChildCount(); i++) {
			node = scene->RootNode->GetChild(i);
			if(!OPfbxMeshDataIsMeta(node)) continue;

			meshData->Meta[metaInd].Name = OPfbxMeshDataMetaName(node);
			meshData->Meta[metaInd].Type = OPfbxMeshDataMetaType(node);
			OPlog("META: %s -> %s", meshData->Meta[metaInd].Name, meshData->Meta[metaInd].Type);

			FbxDouble3 pos = node->LclTranslation.Get();
			OPlog("Pos: %f %f %f", pos[0], pos[1], pos[2]);
			FbxDouble3 rot = node->LclRotation.Get();
			OPlog("Rot: %f %f %f", rot[0], rot[1], rot[2]);
			FbxDouble3 scl = node->LclScaling.Get();
			OPlog("Scl: %f %f %f", scl[0], scl[1], scl[2]);

			meshData->Meta[metaInd].Position[0] = pos[0];
			meshData->Meta[metaInd].Position[1] = pos[1];
			meshData->Meta[metaInd].Position[2] = pos[2];

			meshData->Meta[metaInd].Rotation[0] = rot[0];
			meshData->Meta[metaInd].Rotation[1] = rot[1];
			meshData->Meta[metaInd].Rotation[2] = rot[2];

			meshData->Meta[metaInd].Scale[0] = scl[0];
			meshData->Meta[metaInd].Scale[1] = scl[1];
			meshData->Meta[metaInd].Scale[2] = scl[2];

			metaInd++;
		}
	}


	for (int i = 0; i < scene->RootNode->GetChildCount(); i++) {
		node = scene->RootNode->GetChild(i);
		if(OPfbxMeshDataIsMeta(node)) {
			// Meta nodes don't get loaded
			continue;
		}

		attributeType = node->GetNodeAttribute()->GetAttributeType();
		if(attributeType == FbxNodeAttribute::eMesh) {

			OPlogDebug("Found Mesh");
			meshData->Mesh = node->GetMesh();
			meshData->Node = node;

			meshData->PolyCount = meshData->Mesh->GetPolygonCount();
			OPlogDebug("\tTotal Polys: %ld", (meshData->PolyCount));

			OPint vertexCount = _meshVertexCount(meshData->Mesh);
			OPint indexCount = _meshIndexCount(meshData->Mesh);
			OPlogDebug("\tVertex Count: %ld\n\tIndex Count: %ld", vertexCount, indexCount);

			OPint colorCount = vertexCount * meshData->Mesh->GetElementVertexColorCount();
			OPint uvCount = vertexCount * meshData->Mesh->GetElementUVCount();
			OPint normalCount = vertexCount * meshData->Mesh->GetElementNormalCount();
			OPint tangentCount = vertexCount * meshData->Mesh->GetElementTangentCount();
			OPlogDebug("\tColor Count: %ld\n\tUV Count: %ld\n\tNormal Count: %ld\n\tTangent Count: %ld", 
				colorCount, uvCount, normalCount, tangentCount);

			meshData->VertexCount = vertexCount;
			meshData->IndexCount = indexCount;
			meshData->ColorCount = colorCount;
			meshData->UVCount = uvCount;
			meshData->NormalCount = normalCount;
			meshData->TangentCount = tangentCount;
			return 0;
		}
	}
	// Failed to find a mesh
	OPlogDebug("Failed to find a mesh in the provided scene.");
	return 1;
}