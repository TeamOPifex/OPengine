#include "./Human/include/Utilities/LoaderOPM.h"
#include "./Math/include/Quaternion.h"

#include "./Data/include/List.h"

void OPCalculateTangents(OPMData* data) {
	for (ui16 a = 0; a < data->indexCount; a+=3) {		
		ui16 i1 = ((ui16*)data->indices)[a + 0];
        ui16 i2 = ((ui16*)data->indices)[a + 1];
        ui16 i3 = ((ui16*)data->indices)[a + 2];

		OPMvertex& v1 = ((OPMvertex*)data->vertices)[i1];
        OPMvertex& v2 = ((OPMvertex*)data->vertices)[i2];
        OPMvertex& v3 = ((OPMvertex*)data->vertices)[i3];
		
		v1.Tangent.x = 0;
		v1.Tangent.y = 0;
		v1.Tangent.z = 0;
		v2.Tangent.x = 0;
		v2.Tangent.y = 0;
		v2.Tangent.z = 0;
		v3.Tangent.x = 0;
		v3.Tangent.y = 0;
		v3.Tangent.z = 0;
	}

	for (ui16 a = 0; a < data->indexCount; a+=3) {	
		ui16 i1 = ((ui16*)data->indices)[a + 0];
        ui16 i2 = ((ui16*)data->indices)[a + 1];
        ui16 i3 = ((ui16*)data->indices)[a + 2];
        
		OPMvertex& v1 = ((OPMvertex*)data->vertices)[i1];
        OPMvertex& v2 = ((OPMvertex*)data->vertices)[i2];
        OPMvertex& v3 = ((OPMvertex*)data->vertices)[i3];

		OPvec3 v2v1;
		OPvec3sub(&v2v1, &v2.Position, &v1.Position);
		OPvec3 v3v1;
		OPvec3sub(&v3v1, &v3.Position, &v1.Position);

		float c2c1t = v2.TexCoord.x - v1.TexCoord.x;
		float c2c1b = v2.TexCoord.y - v1.TexCoord.y;

		float c3c1t = v3.TexCoord.x - v1.TexCoord.x;
		float c3c1b = v3.TexCoord.y - v1.TexCoord.y;

		OPvec3 vecNormal = v1.Normal;

		OPvec3 vecTangent;
		vecTangent.x = c3c1b * v2v1.x - c2c1b * v3v1.x;
		vecTangent.y = c3c1b * v2v1.y - c2c1b * v3v1.y;
		vecTangent.z = c3c1b * v2v1.z - c2c1b * v3v1.z;

		OPvec3 vecSmoothBitangent;
		OPvec3cross(&vecSmoothBitangent, &vecNormal, &vecTangent);
		OPvec3norm(&vecSmoothBitangent, &vecSmoothBitangent);

		OPvec3 vecSmoothTangent;
		OPvec3cross(&vecSmoothTangent, &vecSmoothBitangent, &vecNormal);
		OPvec3norm(&vecSmoothTangent, &vecSmoothTangent);
		
		OPvec3add(&v1.Tangent, &v1.Tangent, &vecSmoothTangent);
		OPvec3add(&v2.Tangent, &v2.Tangent, &vecSmoothTangent);
		OPvec3add(&v3.Tangent, &v3.Tangent, &vecSmoothTangent);
		OPvec3norm(&v1.Tangent, &v1.Tangent);
		OPvec3norm(&v2.Tangent, &v2.Tangent);
		OPvec3norm(&v3.Tangent, &v3.Tangent);
	}
}

void OPMgenerateTangent(OPvec3* tangent, OPMvertex* v1, OPMvertex* v2){
	f32 dx = v1->Position.x - v2->Position.x;
	f32 dy = v1->Position.y - v2->Position.y;
	f32 dz = v1->Position.z - v2->Position.z;

	OPvec3 diff = {dx, dy, dz};
	OPvec3cross(tangent, &v1->Normal, &diff);
	OPvec3norm(tangent, tangent);
}


enum OPMFaceFeatures {
	OPM_Face_IsNotTriangle = 0x01,
	OPM_Face_HasMaterial = 0x02,
	OPM_Face_HasFaceUVs = 0x04,
	OPM_Face_HasVertexUvs = 0x08,
	OPM_Face_HasFaceNormals = 0x10,
	OPM_Face_HasVertexNormals = 0x20,
	OPM_Face_HasFaceColors = 0x40,
	OPM_Face_HasVertexColors = 0x80
};
//
//OPMData OPMloadDataV2(OPstream* str) {
//	OPMData data;
//
//	OPvec3 *vertices, *normals, *colors; // *tangents
//	OPvec2* uvs;
//
//	ui32 verticeCount = OPreadui32(str);
//	vertices = (OPvec3*)OPalloc(sizeof(OPvec3)* verticeCount);
//	for (ui32 i = 0; i < verticeCount; i++) {
//		vertices[i].x = OPreadf32(str);
//		vertices[i].y = OPreadf32(str);
//		vertices[i].z = OPreadf32(str);
//	}
//
//	ui32 normalCount = OPreadui32(str);
//	normals = (OPvec3*)OPalloc(sizeof(OPvec3)* normalCount);
//	for (ui32 i = 0; i < normalCount; i++) {
//		normals[i].x = OPreadf32(str);
//		normals[i].y = OPreadf32(str);
//		normals[i].z = OPreadf32(str);
//	}
//	
//	ui32 uvCount = OPreadui32(str);
//	uvs = (OPvec2*)OPalloc(sizeof(OPvec2)* uvCount);
//	for (ui32 i = 0; i < uvCount; i++) {
//		uvs[i].x = OPreadf32(str);
//		uvs[i].y = OPreadf32(str);
//	}
//
//	ui32 colorCount = OPreadui32(str);
//	colors = (OPvec3*)OPalloc(sizeof(OPvec3)* colorCount);
//	for (ui32 i = 0; i < colorCount; i++) {
//		colors[i].x = OPreadf32(str);
//		colors[i].y = OPreadf32(str);
//		colors[i].z = OPreadf32(str);
//	}
//
//	ui32 faceCount = OPreadui32(str);
//	ui16 features;
//	OPint isNotTriangle, hasVertexUvs, hasVertexNormals, hasVertexColors;
//
//	ui32 elementSize = 
//		1 + // Position Index
//		1 + // Normal Index
//		1 + // UV index
//		1;  // Color Index
//	OPlist* vertexes = OPlistCreate(verticeCount, sizeof(ui32)* elementSize);
//	OPlist* indices = OPlistCreate(verticeCount * 3, sizeof(ui32));
//	ui32 vertOne, vertTwo, vertThree, vertFour;
//	ui32 uvOne, uvTwo, uvThree, uvFour;
//	ui32 normOne, normTwo, normThree, normFour;
//	ui32 colorOne, colorTwo, colorThree, colorFour;
//	ui32 vertexSize;
//
//	for (ui32 i = 0; i < faceCount; i++) {
//
//		features = OPreadui16(str);
//		ui32 elements = 0;
//
//		isNotTriangle = OPMhasFeature(features, OPM_Face_IsNotTriangle);
//		hasVertexUvs = OPMhasFeature(features, OPM_Face_HasVertexUvs);
//		hasVertexNormals = OPMhasFeature(features, OPM_Face_HasVertexNormals);
//		hasVertexColors = OPMhasFeature(features, OPM_Face_HasVertexColors);
//
//		elements += (1 + hasVertexUvs + hasVertexNormals + hasVertexColors);
//
//		vertexSize = sizeof(ui32)* elements;
//		ui32* vertex1 = (ui32*)OPalloc(vertexSize);
//		ui32* vertex2 = (ui32*)OPalloc(vertexSize);
//		ui32* vertex3 = (ui32*)OPalloc(vertexSize);
//		ui32* vertex4 = (ui32*)OPalloc(vertexSize);
//		OPbzero(vertex1, vertexSize);
//		OPbzero(vertex2, vertexSize);
//		OPbzero(vertex3, vertexSize);
//		OPbzero(vertex4, vertexSize);
//
//		ui32 pos = 0;
//
//		vertex1[pos] = OPreadui32(str);
//		vertex2[pos] = OPreadui32(str);
//		vertex3[pos] = OPreadui32(str);
//
//		if (isNotTriangle) {
//			vertex4[pos] = OPreadui32(str);
//		}
//
//		pos++;
//
//		if (hasVertexUvs) {
//			// Assume only 1 uv layer for now
//			vertex1[pos] = OPreadui32(str);
//			vertex2[pos] = OPreadui32(str);
//			vertex3[pos] = OPreadui32(str);
//
//			if (isNotTriangle) {
//				vertex4[pos] = OPreadui32(str);
//			}
//
//			pos++;
//		}
//
//		if (hasVertexNormals) {
//			vertex1[pos] = OPreadui32(str);
//			vertex2[pos] = OPreadui32(str);
//			vertex3[pos] = OPreadui32(str);
//
//			if (isNotTriangle) {
//				vertex4[pos] = OPreadui32(str);
//			}
//
//			pos++;
//		}
//
//		if (hasVertexColors) {
//			vertex1[pos] = OPreadui32(str);
//			vertex2[pos] = OPreadui32(str);
//			vertex3[pos] = OPreadui32(str);
//
//			if (isNotTriangle) {
//				vertex4[pos] = OPreadui32(str);
//			}
//			pos++;
//		}
//
//		i32 contains1 = -1, contains2 = -1, contains3 = -1, contains4 = -1;
//		for (ui32 j = 0; j < OPlistSize(vertexes); j++) {
//			ui8* val = OPlistGet(vertexes, j);
//			ui32* vertex = (ui32*)val;
//			if (contains1 > -1 && OPmemcmp(vertex1, vertex, vertexSize)) contains1 = j;
//			if (contains2 > -1 && OPmemcmp(vertex2, vertex, vertexSize)) contains2 = j;
//			if (contains3 > -1 && OPmemcmp(vertex3, vertex, vertexSize)) contains3 = j;
//			if (isNotTriangle && contains4 > -1 && OPmemcmp(vertex4, vertex, vertexSize)) {
//				contains4 = j;
//			}
//		}
//
//		if (contains1 == -1) {
//			OPlistPush(vertexes, (ui8*)vertex1);
//			contains1 = OPlistSize(vertexes) - 1;
//		}
//		if (contains2 == -1) {
//			OPlistPush(vertexes, (ui8*)vertex2);
//			contains2 = OPlistSize(vertexes) - 1;
//		}
//		if (contains3 == -1) {
//			OPlistPush(vertexes, (ui8*)vertex3);
//			contains3 = OPlistSize(vertexes) - 1;
//		}
//		if (isNotTriangle && contains4 == -1) {
//			OPlistPush(vertexes, (ui8*)vertex4);
//			contains4 = OPlistSize(vertexes) - 1;
//		}
//
//		ui32 index1 = contains1;
//		ui32 index2 = contains2;
//		ui32 index3 = contains3;
//		ui32 index4 = contains4;
//
//		OPlistPush(indices, (ui8*)&index1);
//		OPlistPush(indices, (ui8*)&index2);
//		OPlistPush(indices, (ui8*)&index3);
//
//		if (isNotTriangle) {
//			OPlistPush(indices, (ui8*)&index2);
//			OPlistPush(indices, (ui8*)&index3);
//			OPlistPush(indices, (ui8*)&index4);
//		}
//		
//		OPfree(vertex1);
//		OPfree(vertex2);
//		OPfree(vertex3);
//		OPfree(vertex4);
//	}
//
//	
//	
//	data.indices = indices->_indices;
//	data.indexCount = OPlistSize(indices);
//	data.indexSize = sizeof(ui16);
//	data.vertexCount = verticeCount;
//	data.vertices = vertexes->_indices;
//	data.vertexSize = vertexSize;
//
//	//data.bounds = OPboundingBox3DCreate(min, max);
//	//data.hierarchy = hierarchy;
//	//data.pose = pose;
//	//data.hierarchyCount = hierarchyCount;
//	data.hierarchy = NULL;
//
//	return data;
//}

OPMData OPMloadData(OPstream* str) {
	ui16 version = OPreadui16(str);
	//if (version == 2) return OPMloadDataV2(str);

	ui32 features = OPreadui32(str);
	ui32 verticeCount = OPreadui32(str);

	OPvec3 min = OPvec3Zero;
	OPvec3 max = OPvec3Zero;


	OPvec3* positions, *normals, *tangents, *colors;
	OPvec2* uvs;

	OPvec4* boneIndices;
	OPvec4* boneWeights;

	if (OPMhasFeature(features, Position))
		positions = (OPvec3*)OPalloc(sizeof(OPvec3)* verticeCount);
	if (OPMhasFeature(features, Normal))
		normals = (OPvec3*)OPalloc(sizeof(OPvec3)* verticeCount);
	if (OPMhasFeature(features, Tangent))
		tangents = (OPvec3*)OPalloc(sizeof(OPvec3)* verticeCount);
	if (OPMhasFeature(features, UV))
		uvs = (OPvec2*)OPalloc(sizeof(OPvec2)* verticeCount);
	if (OPMhasFeature(features, Color))
		colors = (OPvec3*)OPalloc(sizeof(OPvec3)* verticeCount);
	// Read Skinning
	if (OPMhasFeature(features, Skinning)) {
		boneIndices = (OPvec4*)OPalloc(sizeof(OPvec4)* verticeCount);
		boneWeights = (OPvec4*)OPalloc(sizeof(OPvec4)* verticeCount);
	}

	f32 x, y, z;
	for(ui32 i = 0; i < verticeCount; i++) {

		// Read Position
		if(OPMhasFeature(features, Position)) {
			x = OPreadf32(str);
			y = OPreadf32(str);
			z = OPreadf32(str);
			positions[i].x = x;
			positions[i].y = y;
			positions[i].z = z;
			if (x < min.x) min.x = x;
			if (y < min.y) min.y = y;
			if (z < min.z) min.z = z;
			if (x > max.x) max.x = x;
			if (y > max.y) max.y = y;
			if (z > max.z) max.z = z;
		}

		// Read Normal
		if(OPMhasFeature(features, Normal)) {
			x = OPreadf32(str);
			y = OPreadf32(str);
			z = OPreadf32(str);
			normals[i].x = x;
			normals[i].y = y;
			normals[i].z = z;
			OPvec3norm(&normals[i], &normals[i]);
		}
		
		// Read Tangent
		if(OPMhasFeature(features, Tangent)) {
			x = OPreadf32(str);
			y = OPreadf32(str);
			z = OPreadf32(str);
			tangents[i].x = x;
			tangents[i].y = y;
			tangents[i].z = z;
			OPvec3norm(&tangents[i], &tangents[i]);
		}

		// Read UV
		if (OPMhasFeature(features, UV)) {
			x = OPreadf32(str);
			y = OPreadf32(str);
			uvs[i].x = x;
			uvs[i].y = y;
		}

		// Read Color
		if (OPMhasFeature(features, Color)) {
			x = OPreadf32(str);
			y = OPreadf32(str);
			z = OPreadf32(str);
			colors[i].x = x;
			colors[i].y = y;
			colors[i].z = z;
		}

		// Read Skinning
		if (OPMhasFeature(features, Skinning)) {
			boneIndices[i].x = OPreadui16(str);
			boneIndices[i].y = OPreadui16(str);
			boneIndices[i].z = OPreadui16(str);
			boneIndices[i].w = OPreadui16(str);

			boneWeights[i].x = OPreadf32(str);
			boneWeights[i].y = OPreadf32(str);
			boneWeights[i].z = OPreadf32(str);
			boneWeights[i].w = OPreadf32(str);
		}
	}

	ui32 indicesCount = OPreadui32(str);
	ui16* indices = (ui16*)OPalloc(sizeof(ui16)* (indicesCount * 3));
	for(int i = 0; i < indicesCount; i++) {
		indices[i * 3 + 0] = OPreadui16(str);
		indices[i * 3 + 1] = OPreadui16(str);
		indices[i * 3 + 2] = OPreadui16(str);
	}

	i16* hierarchy = NULL;
	OPmat4* pose = NULL;
	ui16 hierarchyCount = 0;

	// Read Bones
	if(OPMhasFeature(features, Bones)) {
		ui32 boneCount = OPreadui16(str);
		hierarchyCount = boneCount;
		hierarchy = (i16*)OPalloc(sizeof(i16)* boneCount);
		pose = (OPmat4*)OPalloc(sizeof(OPmat4)* boneCount);
		
		for(i32 i = 0; i < boneCount; i++) {
			i32 boneIndex = OPreadi16(str);
			hierarchy[i] = boneIndex;
			i8* name = OPreadstring(str);
			f32 px = OPreadf32(str);
			f32 py = OPreadf32(str);
			f32 pz = OPreadf32(str);

			f32 rx = OPreadf32(str);
			f32 ry = OPreadf32(str);
			f32 rz = OPreadf32(str);
			f32 rw = OPreadf32(str);

			f32 sx = OPreadf32(str);
			f32 sy = OPreadf32(str);
			f32 sz = OPreadf32(str);

			OPmat4 tmp;

			OPquat rot = OPquatCreate(rx, ry, rz, rw);
			OPmat4buildQuat(&tmp, &rot);
			//OPmat4scl(&pose[i], sx, sy, sz);
			//OPmat4quat(&tmp, &rot);
			OPmat4translate(&tmp, px, py, pz);
			OPmat4Inverse(&pose[i], &tmp);
			OPlog("Joint: %d %s : %f, %f, %f", boneIndex, name, px, py, pz);
			OPfree(name);
		}
	}

	OPlog("Index Count: %d", indicesCount);
	OPlog("Vertex Count: %d", verticeCount);


	// Read Animation
	//if(OPMhasFeature(features, Animations)) {
	//	i8* name = OPreadstring(str);
	//	ui32 keyframes = OPreadui32(str);
	//	OPlog("Animation: %s", name);
	//	OPlog("Frame Count %d", keyframes);
	//	for(i32 i = 0; i < keyframes; i++) {
	//		i32 index = OPreadi32(str);
	//		ui32 keys = OPreadui32(str);
	//		OPlog("Keyframe %d", i);
	//		OPlog("Bone %d", index);
	//		for(i32 j = 0; j < keys; j++) {
	//			ui32 keyFeatures = OPreadui32(str);
	//			OPlog("Key: %d", (j + 1));
	//			if(keyFeatures && Key_Time) {
	//				f32 time = OPreadf32(str);
	//				OPlog("Keyframe Time: %f", time);
	//			}

	//			if(keyFeatures && Key_Position) {
	//				f32 x = OPreadf32(str);
	//				f32 y = OPreadf32(str);
	//				f32 z = OPreadf32(str);
	//				//OPlog("Position: %f, %f, %f", x, y, z);
	//			}

	//			if(keyFeatures && Key_Rotation) {
	//				f32 x = OPreadf32(str);
	//				f32 y = OPreadf32(str);
	//				f32 z = OPreadf32(str);
	//				f32 w = OPreadf32(str);
	//				OPlog("Rotation: %f, %f, %f, %f", x, y, z, w);
	//			}
	//		}
	//	}
	//	OPfree(name);
	//}




	OPMData data;
	data.indices = indices;
	data.indexCount = indicesCount * 3;
	data.indexSize = sizeof(ui16);
	data.vertexCount = verticeCount;

	OPvertices* vertices = OPverticesCreate(verticeCount, features);

	if (OPMhasFeature(features, Position))
		OPverticesWriteVec3(vertices, positions, Position);
	if (OPMhasFeature(features, Normal))
		OPverticesWriteVec3(vertices, normals, Normal);
	if (OPMhasFeature(features, Tangent))
		OPverticesWriteVec3(vertices, tangents, Tangent);
	if (OPMhasFeature(features, UV))
		OPverticesWriteVec2(vertices, uvs, UV);
	if (OPMhasFeature(features, Color))
		OPverticesWriteVec3(vertices, colors, Color);
	if (OPMhasFeature(features, Bones))
		OPverticesWriteVec4(vertices, boneIndices, Bones);
	if (OPMhasFeature(features, Skinning))
		OPverticesWriteVec4(vertices, boneWeights, Skinning);


	//if (OPMhasFeature(features, Skinning)) {
	//	
	//	OPvec4* boneVertexIndices = (OPvec4*)OPalloc(sizeof(OPvec4)* verticeCount);
	//	OPvec4* boneVertexWeights = (OPvec4*)OPalloc(sizeof(OPvec4)* verticeCount);

	//	for (i32 i = 0; i < verticeCount; i++) {
	//		ui32 vertIndex = vertBoneIndices[i];
	//		ui32 indexOne = boneIndices[vertIndex * 4 + 0];
	//		f32 weightOne = boneWeights[vertIndex * 4 + 0];
	//		ui32 indexTwo = boneIndices[vertIndex * 4 + 1];
	//		f32 weightTwo = boneWeights[vertIndex * 4 + 1];
	//		ui32 indexThree = boneIndices[vertIndex * 4 + 2];
	//		f32 weightThree = boneWeights[vertIndex * 4 + 2];
	//		ui32 indexFour = boneIndices[vertIndex * 4 + 3];
	//		f32 weightFour = boneWeights[vertIndex * 4 + 3];

	//		boneVertexIndices[i] = OPvec4Create(indexOne, indexTwo, indexThree, indexFour);
	//		boneVertexWeights[i] = OPvec4Create(weightOne, weightTwo, weightThree, weightFour);
	//	}

	//	if (OPMhasFeature(features, Bones)) {
	//		OPverticesWriteVec4(vertices, boneVertexIndices, Bones);
	//		OPverticesWriteVec4(vertices, boneVertexWeights, Skinning);
	//	}
	//}

	data.vertices = vertices->data;
	data.vertexSize = vertices->size * sizeof(f32);

	data.bounds = OPboundingBox3DCreate(min, max);
	data.hierarchy = hierarchy;
	data.pose = pose;
	data.hierarchyCount = hierarchyCount;

	return data;
}

OPint OPMload(const OPchar* filename, OPmesh** mesh) {
	OPlog("Reading File Data");
	OPstream* str = OPreadFile(filename);
	OPlog("Reading OPMloadData");
	OPMData data = OPMloadData(str);

	OPlog("Creating vertex and buffers");
	// Create Vertex & Index Buffers for Mesh
	OPmesh temp = OPrenderCreateMesh();
	OPrenderBindMesh(&temp);
	OPrenderBuildMesh(
		data.vertexSize, data.indexSize,
		data.vertexCount, data.indexCount,
		data.vertices, data.indices
	);
	temp.boundingBox = data.bounds;
	if (data.hierarchy != NULL)
		temp.Skeleton = OPskeletonCreate(data.hierarchy, data.pose, data.hierarchyCount);

	OPlog("Disposing");

	// Dispose of allocated buffers
	OPfree(data.vertices);
	OPfree(data.indices);
	OPstreamDestroy(str);

	*mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
	OPmemcpy(*mesh, &temp, sizeof(OPmesh));

	return 1;
}

HashMap* CreateTriangleTable(OPMData* data){
	HashMap* triTable = OPhashMapCreate(data->indexCount / 3);
	OPchar index[10];
	OPint compCount = 4;

	// for each triangle in the mesh
	for(int i = (data->indexCount / 3); i--;){
		int* tri;


		sprintf(index, "%d", i * 3);
		OPhashMapGet(triTable, index, (void**)&tri);

		// if this vertex's tri has been stored, skip it
		if(tri) continue;

		tri = (int*)OPalloc(sizeof(int) * compCount);
		tri[3] = 0;

		// copy all the triangle's indices into an array
		for(int j = 3; j--;){
			tri[j] = i * 3 + j;

			// store the triangle at this vertex's index
			sprintf(index, "%d", i * 3 + j);
			OPhashMapPut(triTable, index, tri);
		}
	}

	return triTable;
}

OPlinkedList* CreateVertexList(OPMData* data){
	OPlinkedList* vertList = OPllCreate();
	for(int i = data->vertexCount; i--;)
		OPllInsertLast(vertList, (void*)i);

	return vertList;
}

void UpdateBasis(OPvec3* axis, OPvec3* basis, OPvec3* position){
	OPfloat pa = OPvec3valDot(axis, position);
	OPfloat ba = OPvec3valDot(axis, basis);

	if(OPabs(pa) > OPabs(ba)){
		*basis = *position;
	}
}

OPvec3 GetCenterOfMass(OPMData* data, OPlinkedList* vertList){
	OPMvertex* vertices = (OPMvertex*)data->vertices;
	OPvec3 com = {0};
	OPllNode* node = vertList->First;
	int verts = 0;

	while(node){
		com += vertices[(OPint)node->Data].Position;
		node = node->Next;
		++verts;
	}
	com /= verts;

	return com;
}

OPvec3 GetNormal(OPvec3 bX, OPvec3 bY, OPvec3 bZ){
	OPvec3 out = {0};

	OPfloat mX = OPvec3valLen(&bX);
	OPfloat mY = OPvec3valLen(&bY);
	OPfloat mZ = OPvec3valLen(&bZ);

	if(mX > mZ && mY > mZ){
		out = OPvec3valCross(&bX, &bY);
	}
	if(mX > mY && mZ > mY){
		out = OPvec3valCross(&bX, &bZ);
	}
	if(mZ > mX && mY > mX){
		out = OPvec3valCross(&bY, &bZ);
	}

	return out;
}

void ReorderVerts(OPMData* data, OPlinkedList** spaceA, OPlinkedList** spaceB){
	OPMvertex* vertices = (OPMvertex*)data->vertices;
	OPint i = 0, j = 0;
	OPllNode* node = (*spaceA)->First;
	OPlinkedList* tempA = OPllCreate();
	OPlinkedList* tempB = OPllCreate();

	// reorder spaceA
	while(node){
		OPMvertex temp = vertices[i];
		vertices[i] = vertices[j = (OPint)node->Data];
		vertices[j] = temp; // reinsert the displaced vert
		OPllInsertLast(tempA, (void*)i);
		++i;
		node = node->Next;
	}

	// reorder spaceB
	node = (*spaceB)->First;
	while(node){
		OPMvertex temp = vertices[i];
		vertices[i] = vertices[j = (OPint)node->Data];
		vertices[j] = temp; // reinsert the displaced vert
		OPllInsertLast(tempB, (void*)i);
		++i;
		node = node->Next;
	}

	// destroy the old input lists
	OPllDestroy(*spaceA);
	OPllDestroy(*spaceB);

	// set to the reorderd
	*spaceA = tempA;
	*spaceB = tempB;
}

OPlinkedList* CreateTriList(OPMData* data, HashMap* triTable, OPlinkedList* vertList, OPint atLeaf){
	OPlinkedList* triList = OPllCreate();
	OPllNode* node = vertList->First;
	OPchar index[10];

	while(node){
		OPint* tri = NULL;

		sprintf(index, "%d", (OPint)node->Data);
		OPhashMapGet(triTable, index, (void**)&tri);

		// only add indices if this tri hasn't been visited
		if(!tri[3]){
			for(OPint i = 3; i--;){
				OPllInsertLast(triList, (void*)tri[i]);
			}
			if(atLeaf) tri[3] = 1; // mark tri as visited
		}
		node = node->Next;
	}

	return triList;
}

OPMPartNode CreateOPMPartNode(OPlinkedList* triList){
	OPllNode* node = triList->First;
	OPint min = (OPint)node->Data, max = (OPint)node->Data;

	while(node){
		OPint i = (OPint)node->Data;
		min = min > i ? i : min;
		max = max < i ? i : max;
		node = node->Next;
	}

	OPMPartNode meshNode = {
		min,
		max,
		0,
		NULL
	};

	return meshNode;
}

OPMPartNode OPMPartition(OPMData* data, HashMap* triTable, OPlinkedList* vertList, OPint depth){
	OPMvertex* vertices = (OPMvertex*)data->vertices;
	OPlinkedList* spaceA = OPllCreate();
	OPlinkedList* spaceB = OPllCreate();
	OPllNode* node = vertList->First;

	if(depth){
		OPvec3 com = GetCenterOfMass(data, vertList);

		OPvec3 X = {1, 0, 0};
		OPvec3 Y = {0, 1, 0};
		OPvec3 Z = {0, 0, 1};

		// basis vectors
		OPvec3 bX = {0}, bY = {0}, bZ = {0};

		OPvec3 normal = {0};

		// determine the basis vectors of this vertex ist
		while(node){
			OPint i = (OPint)node->Data;
			UpdateBasis(&X, &bX, &vertices[i].Position);
			UpdateBasis(&Y, &bY, &vertices[i].Position);
			UpdateBasis(&Z, &bZ, &vertices[i].Position);
			node = node->Next;
		}

		normal = GetNormal(bX, bY, bZ);

		// split vertices
		node = vertList->First;
		while(node){
			OPint i = (OPint)node->Data;
			OPvec3 diff = vertices[i].Position - com;

			if(OPvec3valDot(&diff, &normal) > 0){
				OPllInsertLast(spaceA, (void*)i);
			}
			else{
				OPllInsertLast(spaceB, (void*)i);
			}
			node = node->Next;
		}

		ReorderVerts(data, &spaceA, &spaceB);

		// ALL DONE
		OPlinkedList* trisA = CreateTriList(data, triTable, spaceA, !depth);
		OPlinkedList* trisB = CreateTriList(data, triTable, spaceB, !depth);

		// TODO call recusively
		OPMPartNode* children = (OPMPartNode*)OPalloc(sizeof(OPMPartNode) * 2);
		OPMPartNode nodeA = children[0] = OPMPartition(data, triTable, trisA, depth - 1);
		OPMPartNode nodeB = children[1] = OPMPartition(data, triTable, trisB, depth - 1);

		// Determine to and from using the two child nodes
		OPint From = nodeA.From < nodeB.From ? nodeA.From : nodeB.From;
		OPint To = nodeA.To > nodeB.To ? nodeA.To : nodeB.To;

		OPMPartNode partNode = {
			From,
			To,
			2,
			children
		};

		// Free up stuff
		OPllDestroy(spaceA); OPllDestroy(spaceB);
		OPllDestroy(trisA); OPllDestroy(trisB);

		return partNode;
	}
	else{
		OPlinkedList* tris = CreateTriList(data, triTable, vertList, 1);
		OPMPartNode partNode = CreateOPMPartNode(tris);
		return partNode;
	}
}

OPint OPMPartitionedLoad(const OPchar* filename, OPmesh** mesh){
	OPlog("Reading File Data");
	OPstream* str = OPreadFile(filename);
	OPlog("Reading OPMloadData");
	OPMData data = OPMloadData(str);

	HashMap*      triTable = CreateTriangleTable(&data);
	OPlinkedList* vertList = CreateVertexList(&data);

	OPMPartition(&data, triTable, vertList, 1);

	OPmesh temp = OPrenderCreateMesh();
	OPrenderBindMesh(&temp);
	OPrenderBuildMesh(
		data.vertexSize, data.indexSize,
		data.vertexCount, data.indexCount,
		data.vertices, data.indices
	);

	OPlog("Disposing");
	// Dispose of allocated buffers
	OPfree(data.vertices);
	OPfree(data.indices);
	OPstreamDestroy(str);

	*mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
	OPmemcpy(*mesh, &temp, sizeof(OPmesh));

	return 1;
}

OPint OPMloadPacked(const OPchar* filename, OPmeshPacked** mesh) {
	OPstream* str = OPreadFile(filename);
	OPMData data = OPMloadData(str);

	OPmeshPacked temp = OPrenderCreateMeshPacked(
		data.vertexSize, data.indexSize,
		data.vertexCount, data.indexCount,
		data.vertices, data.indices
	);
	
	// Dispose of allocated buffers
	OPfree(data.vertices);
	OPfree(data.indices);
	OPstreamDestroy(str);

	*mesh = (OPmeshPacked*)OPalloc(sizeof(OPmeshPacked));
	OPmemcpy(*mesh, &temp, sizeof(OPmeshPacked));

	return 1;
}

OPint OPMUnload(void* mesh){
	OPmesh* m = (OPmesh*)mesh;

	OPrenderDelBuffer(&m->IndexBuffer);
	OPrenderDelBuffer(&m->VertexBuffer);
	OPfree(m);

	return 1;
}