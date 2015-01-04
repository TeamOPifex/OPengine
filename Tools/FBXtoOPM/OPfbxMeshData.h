#pragma once
#include "OPfbxSdk.h"

typedef struct {
	OPint Size;
	OPint ControlPointIndex[4];
	OPvec3 Position[4];
	OPvec3 Normal[4];
	OPvec2 UV[4];
} OPfbxMeshPoly;

typedef struct {
	FbxNode* Node;
	FbxMesh* Mesh;

	OPint PolyCount;

	OPint VertexCount;
	OPint IndexCount;

	OPint ColorCount;
	OPint UVCount;
	OPint NormalCount;
	OPint TangentCount;
} OPfbxMeshData;

OPfbxMeshPoly* OPfbxMeshDataGetPolygons(OPfbxMeshData* meshData) {

	OPfbxMeshPoly* polys = (OPfbxMeshPoly*)OPalloc(sizeof(OPfbxMeshPoly)* meshData->PolyCount);

	FbxVector4* controlPoints = meshData->Mesh->GetControlPoints();
	
	OPint polySize;
	for (OPint i = 0; i < meshData->PolyCount; i++) {
		polySize = meshData->Mesh->GetPolygonSize(i);
		polys[i].Size = polySize;

		for (OPint j = 0; j < polySize; j++) {

			int controlPointIndex = meshData->Mesh->GetPolygonVertex(i, j);

			polys[i].ControlPointIndex[j] = controlPointIndex;
			polys[i].Position[j] = OPvec3Create(
				(f32)controlPoints[controlPointIndex][0],
				(f32)controlPoints[controlPointIndex][1],
				(f32)controlPoints[controlPointIndex][2]
				);

			// Only support for 1 normal currently
			for (OPint k = 0; k < meshData->Mesh->GetElementNormalCount() && k <= 1; ++k) {

				FbxGeometryElementNormal* eNormal = meshData->Mesh->GetElementNormal(k);

				FbxVector4 normal;
				if (eNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {

					switch (eNormal->GetReferenceMode())
					{
						case FbxGeometryElement::eDirect: {
							normal = eNormal->GetDirectArray().GetAt(0);
							break;
						}
						case FbxGeometryElement::eIndexToDirect: {
							int id = eNormal->GetIndexArray().GetAt(0);
							normal = eNormal->GetDirectArray().GetAt(id);
							break;
						}
					}
					polys[i].Normal[j] = OPvec3Create(normal[0], normal[1], normal[2]);
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
								OPlog("WARNING: Unsupported UV Control Point Type");
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
								OPlg("*");
								OPlog("UV Index: %d", textureUVIndex);
								uv = eUV->GetDirectArray().GetAt(textureUVIndex);
								break;
							}
							default:
								OPlog("WARNING: Unsupported UV Polygon Vertex Type");
								break;
						}
					}

					case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
					case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
					case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
						break;

					default: {
								 OPlog("WARNING: Unsupported UV Mapping Mode");
								 break;
					}
				}

				polys[i].UV[j] = OPvec2Create(uv[0], uv[1]);
			}

		}
	}


	OPlog("========= FBX Polys =========\n");
	for (OPint i = 0; i < meshData->PolyCount; i++) {
		OPlog("Size: %d", polys[i].Size);
		for (OPint j = 0; j < polys[i].Size; j++) {
			OPlog("Ind: %d", polys[i].ControlPointIndex[j]);
			OPvec3Log("Pos: ", polys[i].Position[j]);
			OPvec3Log("Norm: ", polys[i].Normal[j]);
			OPvec2Log("UV: ", polys[i].UV[j]);
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
			OPlog("\tWARNING: Unsupported polygon size!");
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
			OPlog("Ind: %d %d %d",indices[pos+0], indices[pos+1],indices[pos+2]);
			pos += 3;
			indexOffset += 3;
		} else if(polySize == 4) {
			indices[pos+0] = indexOffset + 0;
			indices[pos+1] = indexOffset + 1;
			indices[pos+2] = indexOffset + 2;

			indices[pos+3] = indexOffset + 0;
			indices[pos+4] = indexOffset + 2;
			indices[pos+5] = indexOffset + 3;

			OPlog("Ind: %d %d %d %d %d %d", indices[pos+0], indices[pos+1],indices[pos+2], indices[pos+3], indices[pos+4],indices[pos+5]);
			pos += 6;
			indexOffset += 4;
		} else {
			OPlog("WARNING: Unsupported polysize: %d", polySize);
		}
	}

	OPlog("Pos: %d, Ind Count: %d", pos, meshData->IndexCount);

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

			OPvec3Log("Pos: ", positions[pos]);

			pos++;
		}
	}
	OPlog("Pos: %d, Vert Count: %d", pos, meshData->VertexCount);

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

					OPvec3Log("Normal: ", normals[pos]);

					pos++;
				}
			}

			vertexInd++;
		}

	}
	OPlog("Pos: %d, Norm Count: %d", pos, meshData->NormalCount);

	return normals;
}

OPvec2* OPfbxMeshDataGetUVs(OPfbxMeshData* meshData) {
	if(meshData->UVCount == 0) return NULL;
	OPlog("UV Count: %d", meshData->UVCount);

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
								OPlog("WARNING: Unsupported UV Control Point Type");
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
								OPlg("*");
								OPlog("UV Index: %d",textureUVIndex );
								uv = eUV->GetDirectArray().GetAt(textureUVIndex);
								break;
							}
							default:
								OPlog("WARNING: Unsupported UV Polygon Vertex Type");
								break;
						}
					}

					case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
					case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
					case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
						break;

					default: {
						OPlog("WARNING: Unsupported UV Mapping Mode");
						break;
					}
				}

				uvs[pos] = OPvec2Create( uv[0], uv[1] );
				OPvec2Log("UV: ", uvs[pos]);
				pos++;
			}


		}
	}

	OPlog("Pos: %d, UV Count: %d", pos, meshData->UVCount);

	if(pos != meshData->UVCount) {
		OPlog("WARNING: Not all UVs were mapped.");
	}

	return uvs;
}

OPint OPfbxMeshDataGet(OPfbxMeshData* meshData, OPfbxScene* scene) {

	// Loop through each child in the root node of the scene until 
	// we find the first available mesh (Only supporting 1 mesh)
	// TODO: Support multiple meshes
	FbxNodeAttribute::EType attributeType;
	FbxNode* node;

	for (int i = 0; i < scene->RootNode->GetChildCount(); i++) {
		node = scene->RootNode->GetChild(i);		
		attributeType = node->GetNodeAttribute()->GetAttributeType();
		if(attributeType == FbxNodeAttribute::eMesh) {

			OPlog("Found Mesh");
			meshData->Mesh = node->GetMesh();
			meshData->Node = node;

			meshData->PolyCount = meshData->Mesh->GetPolygonCount();
			OPlog("\tTotal Polys: %d", meshData->PolyCount);

			OPint vertexCount = _meshVertexCount(meshData->Mesh);
			OPint indexCount = _meshIndexCount(meshData->Mesh);
			OPlog("\tVertex Count: %d\n\tIndex Count: %d", vertexCount, indexCount);

			OPint colorCount = vertexCount * meshData->Mesh->GetElementVertexColorCount();
			OPint uvCount = vertexCount * meshData->Mesh->GetElementUVCount();
			OPint normalCount = vertexCount * meshData->Mesh->GetElementNormalCount();
			OPint tangentCount = vertexCount * meshData->Mesh->GetElementTangentCount();
			OPlog("\tColor Count: %d\n\tUV Count: %d\n\tNormal Count: %d\n\tTangent Count: %d", 
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
	OPlog("Failed to find a mesh in the provided scene.");
	return 1;
}