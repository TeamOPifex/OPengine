#include "./Pipeline/include/Loaders/OPloaderOPM.h"
#include "./Human/include/Rendering/Enums/OPattributes.h"
#include "./Math/include/OPquat.h"
#include "./Data/include/OPlist.h"

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

		OPvec3 v2v1 = v2.Position - v1.Position;
		OPvec3 v3v1 = v3.Position - v1.Position;

		float c2c1t = v2.TexCoord.x - v1.TexCoord.x;
		float c2c1b = v2.TexCoord.y - v1.TexCoord.y;

		float c3c1t = v3.TexCoord.x - v1.TexCoord.x;
		float c3c1b = v3.TexCoord.y - v1.TexCoord.y;

		OPvec3 vecNormal = v1.Normal;

		OPvec3 vecTangent;
		vecTangent.x = c3c1b * v2v1.x - c2c1b * v3v1.x;
		vecTangent.y = c3c1b * v2v1.y - c2c1b * v3v1.y;
		vecTangent.z = c3c1b * v2v1.z - c2c1b * v3v1.z;

		OPvec3 vecSmoothBitangent = OPvec3Cross(vecNormal, vecTangent);
		vecSmoothBitangent = OPvec3Norm(vecSmoothBitangent);

		OPvec3 vecSmoothTangent = {};
		vecSmoothTangent = OPvec3Cross(vecSmoothBitangent, vecNormal);
		vecSmoothTangent = OPvec3Norm(vecSmoothTangent);

		v1.Tangent += vecSmoothTangent;
		v2.Tangent += vecSmoothTangent;
		v3.Tangent += vecSmoothTangent;
		v1.Tangent = OPvec3Norm(v1.Tangent);
		v2.Tangent = OPvec3Norm(v2.Tangent);
		v3.Tangent = OPvec3Norm(v3.Tangent);

		v1.BiTangent += vecSmoothBitangent;
		v2.BiTangent += vecSmoothBitangent;
		v3.BiTangent += vecSmoothBitangent;
		v1.BiTangent = OPvec3Norm(v1.BiTangent);
		v2.BiTangent = OPvec3Norm(v2.BiTangent);
		v3.BiTangent = OPvec3Norm(v3.BiTangent);
	}
}

void OPMgenerateTangent(OPvec3* tangent, OPMvertex* v1, OPMvertex* v2){
	f32 dx = v1->Position.x - v2->Position.x;
	f32 dy = v1->Position.y - v2->Position.y;
	f32 dz = v1->Position.z - v2->Position.z;

	OPvec3 diff = {dx, dy, dz};
	*tangent = OPvec3Cross(v1->Normal, diff);
	*tangent = OPvec3Norm(*tangent);
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

OPMData OPMloadDataV2(OPstream* str) {
	OPMData result;

	// Already loaded version at this point
	ui32 meshCount = OPreadui32(str);

	for (ui32 i = 0; i < meshCount; i++) {
		ui16 vertexMode = OPreadui16(str);

		if (vertexMode == 2) {
			OPlog("Version 2 not supported yet");
			continue;
		}

		ui32 features = OPreadui32(str);
		ui32 verticesCount = OPreadui32(str);
		ui32 indicesCount = OPreadui32(str);

		result.vertexCount = verticesCount;
		result.indexCount = indicesCount;

		OPvertexLayoutBuilder layout;
		layout.Init();

		if (OPMhasFeature(features, Position))
			layout.Add(OPattributes::POSITION);
		if (OPMhasFeature(features, Normal))
			layout.Add(OPattributes::NORMAL);
		if (OPMhasFeature(features, Tangent))
			layout.Add(OPattributes::TANGENT);
		if (OPMhasFeature(features, UV))
			layout.Add(OPattributes::UV);
		if (OPMhasFeature(features, Color))
			layout.Add(OPattributes::COLOR);
		if (OPMhasFeature(features, Skinning))
			layout.Add(OPattributes::BONES);

		result.vertexLayout = layout.Build();
		result.indexSize = OPindexSize::SHORT;// sizeof(ui16);
		result.indices = OPalloc((ui32)result.indexSize * indicesCount);
		result.vertices = OPalloc(result.vertexLayout.stride * verticesCount);

		f32* vertData = (f32*)result.vertices;
		for (ui32 j = 0; j < verticesCount * (result.vertexLayout.stride / sizeof(f32)); j++) {
			vertData[j] = OPreadf32(str);
		}

		ui16* indData = (ui16*)result.indices;
		for (ui32 j = 0; j < indicesCount; j++) {
			indData[j] = OPreadui16(str);
		}
	}

	return result;
}

OPMData OPMloadData(OPstream* str) {
	ui16 version = OPreadui16(str);
	OPlog("Reading OPM Version: %d", version);
	if (version == 2) return OPMloadDataV2(str);

	ui32 features = OPreadui32(str);
	ui32 verticeCount = OPreadui32(str);

	OPvec3 min = OPVEC3_ZERO;
	OPvec3 max = OPVEC3_ZERO;


	OPvec3* positions, *normals, *tangents, *bitangents, *colors;
	OPvec2* uvs;

	OPvec4* boneIndices;
	OPvec4* boneWeights;
	

	OPvertexLayoutBuilder layout;
	layout.Init();

	if (OPMhasFeature(features, Position)) {
		OPlogDebug("Feature: Position");
		positions = (OPvec3*)OPalloc(sizeof(OPvec3)* verticeCount);
		layout.Add(OPattributes::POSITION);
	}
	if (OPMhasFeature(features, Normal)) {
		OPlogDebug("Feature: Normal");
		normals = (OPvec3*)OPalloc(sizeof(OPvec3)* verticeCount);
		layout.Add(OPattributes::NORMAL);
	}
	if (OPMhasFeature(features, Tangent)) {
		OPlogDebug("Feature: Tangent");
		tangents = (OPvec3*)OPalloc(sizeof(OPvec3)* verticeCount);
		layout.Add(OPattributes::TANGENT);
	}
	if (OPMhasFeature(features, BiTangent)) {
		OPlogDebug("Feature: BiTangent");
		bitangents = (OPvec3*)OPalloc(sizeof(OPvec3)* verticeCount);
		layout.Add(OPattributes::BITANGENT);
	}
	if (OPMhasFeature(features, UV)) {
		OPlogDebug("Feature: UV");
		uvs = (OPvec2*)OPalloc(sizeof(OPvec2)* verticeCount);
		layout.Add(OPattributes::UV);
	}
	if (OPMhasFeature(features, Color)) {
		OPlogDebug("Feature: Color");
		colors = (OPvec3*)OPalloc(sizeof(OPvec3)* verticeCount);
		layout.Add(OPattributes::COLOR);
	}
	// Read Skinning
	if (OPMhasFeature(features, Skinning)) {
		OPlogDebug("Feature: Skinning");
		boneIndices = (OPvec4*)OPalloc(sizeof(OPvec4)* verticeCount);
		boneWeights = (OPvec4*)OPalloc(sizeof(OPvec4)* verticeCount);
		layout.Add(OPattributes::BONES);
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

			//OPlog("Pos: %f %f %f", x, y, z);
		}

		// Read Normal
		if(OPMhasFeature(features, Normal)) {
			x = OPreadf32(str);
			y = OPreadf32(str);
			z = OPreadf32(str);
			normals[i].x = x;
			normals[i].y = y;
			normals[i].z = z;
			normals[i] = OPvec3Norm(normals[i]);
			//OPlog("Normal: %f %f %f", x, y, z);
		}

		// Read Tangent
		if (OPMhasFeature(features, Tangent)) {
			x = OPreadf32(str);
			y = OPreadf32(str);
			z = OPreadf32(str);
			tangents[i].x = x;
			tangents[i].y = y;
			tangents[i].z = z;
			tangents[i] = OPvec3Norm(tangents[i]);
			//OPlog("Tangent: %f %f %f", x, y, z);
		}

		// Read BiTangent
		if (OPMhasFeature(features, BiTangent)) {
			x = OPreadf32(str);
			y = OPreadf32(str);
			z = OPreadf32(str);
			bitangents[i].x = x;
			bitangents[i].y = y;
			bitangents[i].z = z;
			bitangents[i] = OPvec3Norm(bitangents[i]);
			//OPlog("Tangent: %f %f %f", x, y, z);
		}

		// Read UV
		if (OPMhasFeature(features, UV)) {
			x = OPreadf32(str);
			y = OPreadf32(str);
			uvs[i].x = x;
			uvs[i].y = y;
			//OPlog("UV: %f %f", x, y);
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
			boneIndices[i].x = (f32)OPreadui16(str);
			boneIndices[i].y = (f32)OPreadui16(str);
			boneIndices[i].z = (f32)OPreadui16(str);
			boneIndices[i].w = (f32)OPreadui16(str);

			boneWeights[i].x = OPreadf32(str);
			boneWeights[i].y = OPreadf32(str);
			boneWeights[i].z = OPreadf32(str);
			boneWeights[i].w = OPreadf32(str);

			//OPlog("%f %f %f %f %f %f %f %f",
			//	boneIndices[i].x,
			//	boneIndices[i].y,
			//	boneIndices[i].z,
			//	boneIndices[i].w,
			//	boneWeights[i].x,
			//	boneWeights[i].y,
			//	boneWeights[i].z,
			//	boneWeights[i].w);
		}
	}

	ui32 indicesCount = OPreadui32(str);
	ui16* indices = (ui16*)OPalloc(sizeof(ui16)* (indicesCount * 3));
	for(ui32 i = 0; i < indicesCount; i++) {
		indices[i * 3 + 0] = OPreadui16(str);
		indices[i * 3 + 1] = OPreadui16(str);
		indices[i * 3 + 2] = OPreadui16(str);
	}

	//i16* hierarchy = NULL;
	//OPmat4* pose = NULL;
	//ui16 hierarchyCount = 0;

	//// Read Bones
	//if(OPMhasFeature(features, Bones)) {
	//	OPlog("Has Bones");
	//	i32 boneCount = OPreadi16(str);
	//	hierarchyCount = boneCount;
	//	hierarchy = (i16*)OPalloc(sizeof(i16)* boneCount);
	//	pose = (OPmat4*)OPallocZero(sizeof(OPmat4)* boneCount);
	//
	//	for(i32 i = 0; i < boneCount; i++) {

	//		i32 boneIndex = OPreadi16(str);
	//		hierarchy[i] = boneIndex;

	//		OPchar* name = OPreadstring(str);

	//		OPmat4* p = &(pose[i]);
	//		//OPlog("Mat4 Bone: %x", p);

	//		for (i32 c = 0; c < 4; c++) {
	//			p->cols[c].x = OPreadf32(str);
	//			p->cols[c].y = OPreadf32(str);
	//			p->cols[c].z = OPreadf32(str);
	//			p->cols[c].w = OPreadf32(str);
	//		}


	//		// f32 px = OPreadf32(str);
	//		// f32 py = OPreadf32(str);
	//		// f32 pz = OPreadf32(str);

	//		// f32 rx = OPreadf32(str);
	//		// f32 ry = OPreadf32(str);
	//		// f32 rz = OPreadf32(str);
	//		// f32 rw = OPreadf32(str);

	//		// f32 sx = OPreadf32(str);
	//		// f32 sy = OPreadf32(str);
	//		// f32 sz = OPreadf32(str);

	//		OPmat4 matRotate;
	//		OPmat4 matTranslate;

	//		//OPquat rot = OPquatCreate(rx, ry, rz, rw);

	//		//OPmat4BuildQuat(&matRotate, &rot);
	//		//OPmat4BuildTranslate(&matTranslate, px, py, pz);
	//		//OPmat4Mul(&pose[i], &matTranslate, &matRotate);
	//		//OPmat4Mul(&pose[i], &matRotate, &matTranslate);
	//		//OPmat4Inverse(&pose[i], &pose[i]);
	//		//OPmat4Transpose(&pose[i]);
	//		//OPmat4Identity(p);
	//		pose[i] = OPmat4Transpose(pose[i]);


	//		OPlog("Joint: %d %d %s", i, boneIndex, name);
	//		OPmat4Log("Bone", *p);
	//		OPfree(name);
	//	}
	//}

	//i16 trackCount = 0;
	//OPMdataAnim* tracks = NULL;
	//if (OPMhasFeature(features, Animations) && OPMhasFeature(features, Bones)) {
	//	trackCount = OPreadi16(str);
	//	tracks = (OPMdataAnim*)OPalloc(sizeof(OPMdataAnim)* trackCount);
	//	for (OPint i = 0; i < trackCount; i++) {
	//		tracks[i].Name = OPreadstring(str);

	//		tracks[i].FrameCount = OPreadui32(str);

	//		OPint totalFrames = tracks[i].FrameCount * hierarchyCount;
	//		tracks[i].Frames = (OPmat4*)OPalloc(sizeof(OPmat4)* totalFrames);

	//		for (OPint j = 0; j < totalFrames; j++) {

	//			OPmat4* p = &(tracks[i].Frames[j]);
	//			//OPlog("Mat4 Bone: %x", p);

	//			for (i32 c = 0; c < 4; c++) {
	//				p->cols[c].x = OPreadf32(str);
	//				p->cols[c].y = OPreadf32(str);
	//				p->cols[c].z = OPreadf32(str);
	//				p->cols[c].w = OPreadf32(str);
	//			}

	//			tracks[i].Frames[j] = OPmat4Transpose(tracks[i].Frames[j]);
	//		}
	//	}
	//}

	//OPlog("Index Count: %d", indicesCount);
	//OPlog("Vertex Count: %d", verticeCount);


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




	OPMData data = {};
	
	data.vertexLayout = layout.Build();
	data.indices = indices;
	data.indexCount = indicesCount * 3;
	data.indexSize = OPindexSize::SHORT;// sizeof(ui16);
	data.vertexCount = verticeCount;

	OPvertices* vertices = OPverticesCreate(verticeCount, features);

	if (OPMhasFeature(features, Position))
		OPverticesWriteVec3(vertices, positions, Position);
	if (OPMhasFeature(features, Normal))
		OPverticesWriteVec3(vertices, normals, Normal);
	if (OPMhasFeature(features, Tangent))
		OPverticesWriteVec3(vertices, tangents, Tangent);
	if (OPMhasFeature(features, BiTangent))
		OPverticesWriteVec3(vertices, bitangents, BiTangent);
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
	//data.vertexSize = vertices->size * sizeof(f32);

	data.bounds = OPboundingBox3DCreate(min, max);
	//data.hierarchy = hierarchy;
	//data.pose = pose;
	//data.hierarchyCount = hierarchyCount;
	//data.trackCount = trackCount;
	//data.tracks = tracks;



	if (OPMhasFeature(features, Meta)) {
		ui16 metaCount = OPreadui16(str);
		data.metaCount = metaCount;

		if (metaCount > 0) {
			//OPlog("Meta Count: %d", metaCount);
			OPMmeta* meta = (OPMmeta*)OPalloc(sizeof(OPMmeta) * metaCount);
			for (i32 i = 0; i < metaCount; i++) {
				OPchar* metaName = OPreadstring(str);
				OPchar* metaType = OPreadstring(str);
				OPlog("Meta Name: %s (%s)", metaName, metaType);
				f32 x = OPreadf32(str);
				f32 y = OPreadf32(str);
				f32 z = OPreadf32(str);
				f32 rx = OPreadf32(str);
				f32 ry = OPreadf32(str);
				f32 rz = OPreadf32(str);
				f32 sx = OPreadf32(str);
				f32 sy = OPreadf32(str);
				f32 sz = OPreadf32(str);
				meta[i].Name = metaName;
				meta[i].Type = metaType;
				meta[i].Position = OPvec3Create(x, y, z);
				meta[i].Rotation = OPvec3Create(rx, ry, rz);
				meta[i].Scale = OPvec3Create(sx, sy, sz);
			}
			data.meta = meta;
		}
	}

	return data;
}

OPMData OPMloadData(const OPchar * filename)
{
	OPstream* stream = OPreadFile(filename);
	return OPMloadData(stream);
}

OPint OPMload(OPstream* str, OPmesh** mesh) {
	//OPlog("Reading File Data");
	//OPstream* str = OPreadFile(filename);
	if (str == NULL) {
		return 0;
	}
	//OPlog("Reading OPMloadData");
	OPMData data = OPMloadData(str);
	OPlog("Loaded OPM");

	//OPlog("Creating vertex and buffers");
	// Create Vertex & Index Buffers for Mesh
	OPmesh temp = OPmesh(data.vertexLayout);
	OPMESH_ACTIVE = &temp;
	temp.Build(
		data.vertexLayout, data.indexSize,
		data.vertexCount, data.indexCount,
		data.vertices, data.indices
	);
	temp.vertexLayout = data.vertexLayout;
	temp.boundingBox = data.bounds;

	//if (data.hierarchy != NULL) {
	//	temp.Skeleton = OPskeletonCreate(data.hierarchy, data.pose, data.hierarchyCount);

	//	if (data.tracks != NULL)
	//		OPskeletonAnimationInit(&temp.SkeletonAnimation, temp.Skeleton, data.tracks[0].Frames, data.tracks[0].FrameCount);
	//}

	temp.MetaCount = data.metaCount;
	temp.Meta = data.meta;
	//temp.VertexSize = data.vertexLayout.stride;
	temp.IndexSize = data.indexSize;
	temp.VertexCount = data.vertexCount;
	temp.IndexCount = data.indexCount;

	//OPlog("Disposing");

	// Dispose of allocated buffers
	//OPfree(data.vertices);
	//OPfree(data.indices);
	//OPstreamDestroy(str);

	*mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
	OPmemcpy(*mesh, &temp, sizeof(OPmesh));

	return 1;
}

OPhashMap* CreateTriangleTable(OPMData* data){
	OPhashMap* triTable = OPhashMapCreate(data->indexCount / 3);
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
	for(OPint i = data->vertexCount; i--;)
		OPllInsertLast(vertList, (void*)i);

	return vertList;
}

void UpdateBasis(OPvec3* axis, OPvec3* basis, OPvec3* position){
	OPfloat pa = OPvec3Dot(*axis, *position);
	OPfloat ba = OPvec3Dot(*axis, *basis);

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
	com /= (OPfloat)verts;

	return com;
}

OPvec3 GetNormal(OPvec3 bX, OPvec3 bY, OPvec3 bZ){
	OPvec3 out = {0};

	OPfloat mX = OPvec3Len(bX);
	OPfloat mY = OPvec3Len(bY);
	OPfloat mZ = OPvec3Len(bZ);

	if(mX > mZ && mY > mZ){
		out = OPvec3Cross(bX, bY);
	}
	if(mX > mY && mZ > mY){
		out = OPvec3Cross(bX, bZ);
	}
	if(mZ > mX && mY > mX){
		out = OPvec3Cross(bY, bZ);
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

OPlinkedList* CreateTriList(OPMData* data, OPhashMap* triTable, OPlinkedList* vertList, OPint atLeaf){
	OPlinkedList* triList = OPllCreate();
	OPllNode* node = vertList->First;
	OPchar index[10];

	while(node){
		OPint* tri = NULL;

#ifdef OPIFEX_OS64
			sprintf(index, "%lld", (OPint)node->Data);
#else
			sprintf(index, "%d", (OPint)node->Data);
#endif
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
	OPuint min = (OPint)node->Data, max = (OPint)node->Data;

	while(node){
		OPuint i = (OPuint)node->Data;
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

OPMPartNode OPMPartition(OPMData* data, OPhashMap* triTable, OPlinkedList* vertList, OPint depth){
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

			if(OPvec3Dot(diff, normal) > 0){
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
		OPuint From = nodeA.From < nodeB.From ? nodeA.From : nodeB.From;
		OPuint To = nodeA.To > nodeB.To ? nodeA.To : nodeB.To;

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

	OPhashMap*      triTable = CreateTriangleTable(&data);
	OPlinkedList* vertList = CreateVertexList(&data);

	OPMPartition(&data, triTable, vertList, 1);

	OPmesh temp = OPmesh(data.vertexLayout);
	temp.Build(
		data.vertexLayout, data.indexSize,
		data.vertexCount, data.indexCount,
		data.vertices, data.indices
	);

	OPlog("Disposing");
	// Dispose of allocated buffers
	OPfree(data.vertices);
	OPfree(data.indices);
	//OPstreamDestroy(str);

	*mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
	OPmemcpy(*mesh, &temp, sizeof(OPmesh));

	return 1;
}

OPint OPMloadPacked(const OPchar* filename, OPmeshPacked** mesh) {
	OPstream* str = OPreadFile(filename);
	OPMData data = OPMloadData(str);
	
	OPmeshPacked temp = OPmeshPacked(
		data.vertexLayout, data.indexSize,
		data.vertexCount, data.indexCount,
		data.vertices, data.indices
	);

	// Dispose of allocated buffers
	OPfree(data.vertices);
	OPfree(data.indices);
	//OPstreamDestroy(str);

	*mesh = (OPmeshPacked*)OPalloc(sizeof(OPmeshPacked));
	OPmemcpy(*mesh, &temp, sizeof(OPmeshPacked));

	return 1;
}

OPint OPMReload(OPstream* str, OPmesh** mesh){
	OPlog("Reload Mesh OPM");
	OPmesh* resultMesh;
	OPmesh* tex = (OPmesh*)(*mesh);
	OPint result = OPMload(str, &resultMesh);
	if (result) {
		tex->indexBuffer.Destroy();
		tex->vertexBuffer.Destroy();
		//OPrenderDelBuffer(&tex->IndexBuffer);
		//OPrenderDelBuffer(&tex->VertexBuffer);
		OPmemcpy(*mesh, resultMesh, sizeof(OPmesh));
		OPfree(resultMesh);
	}
	return result;
}

OPint OPMUnload(void* mesh){
	OPmesh* m = (OPmesh*)mesh;

	m->indexBuffer.Destroy();
	m->vertexBuffer.Destroy();
	//OPrenderDelBuffer(&m->IndexBuffer);
	//OPrenderDelBuffer(&m->VertexBuffer);
	OPfree(m);

	return 1;
}
