#include "./Pipeline/include/Loaders/OPloaderOPM.h"
#include "./Human/include/Rendering/OPmodel.h"

struct OPmeshAnim {
	OPchar* Name;
	OPuint FrameCount;
	OPmat4* Frames; // Based on bone count * FrameCount
};

bool OPMloadV1(OPmodel* model, OPstream* str);
bool OPMloadDataV2(OPmodel* model, OPstream* str);

bool _loadOPM(OPmodel* model, OPstream* str) {
	ui16 version = str->UI16(); // OPM File Format Version
	
	if (version == 1) {
		OPlogErr("No longer supporting OPM File Format Version 1");
		return OPMloadV1(model, str);
	}

	if (version == 2) {
		OPlogErr("No longer supporting OPM File Format Version 2");
		return OPMloadDataV2(model, str);
	}

	OPchar* modelName = str->String();
	OPlogInfo("MODEL: %s", modelName);

	ui32 meshCount = str->UI32(); // Number of meshes in this OPM

	ui32 features = str->UI32(); // Boolean Flag representation of OPM features
	OPvertexLayoutBuilder vertexLayoutBuilder;// = OPvertexLayoutBuilder(features);
	vertexLayoutBuilder.Init();
	
	if (OPMhasFeature(features, Position))
		vertexLayoutBuilder.Add(OPattributes::POSITION);
	if (OPMhasFeature(features, Normal))
		vertexLayoutBuilder.Add(OPattributes::NORMAL);
	if (OPMhasFeature(features, Tangent))
		vertexLayoutBuilder.Add(OPattributes::TANGENT);
	if (OPMhasFeature(features, BiTangent))
		vertexLayoutBuilder.Add(OPattributes::BITANGENT);
	if (OPMhasFeature(features, UV))
		vertexLayoutBuilder.Add(OPattributes::UV);
	if (OPMhasFeature(features, Color))
		vertexLayoutBuilder.Add(OPattributes::COLOR);
	if (OPMhasFeature(features, Skinning))
		vertexLayoutBuilder.Add(OPattributes::BONES);

	OPvertexLayout vertexLayout = vertexLayoutBuilder.Build();

	model->Init(meshCount, vertexLayout);

	ui16 vertexMode = str->UI16();
	if (vertexMode == 2) {
		OPlogErr("OPM Vertex Mode Version 2 not supported yet");
		return false;
	}
	
	ui32 totalVertices = str->UI32();
	ui32 totalIndices = str->UI32();

	OPindexSize indexSize = OPindexSize::SHORT;

	void* vertices = OPalloc(totalVertices * vertexLayout.stride);
	void* indices = OPalloc(totalIndices * (ui16)indexSize);
	OPuint vertexOffset = 0;
	OPuint indexOffset = 0;

	f32* verticesData = (f32*)vertices;

	for (ui32 i = 0; i < meshCount; i++) {
		OPmesh* mesh = &model->meshes[i];

		OPchar* name = str->String();
		OPlogInfo("SUBMESH: %s", name);

		ui32 verticesCount = str->UI32();
		ui32 indicesCount = str->UI32();

		ui32 floatsInStride = vertexLayout.stride / sizeof(f32);
		ui32 vertexDataOffset = vertexOffset * floatsInStride;
		f32* vertData = &verticesData[vertexDataOffset];
		for (ui32 j = 0; j < verticesCount * floatsInStride; j++) {
			vertData[j] = str->F32();
		}

		ui16* indData = &((ui16*)indices)[indexOffset];
		for (ui32 j = 0; j < indicesCount; j++) {
			indData[j] = str->UI16();
		}

		mesh->offset = indexOffset;
		mesh->count = indicesCount;
		mesh->name = name;

		mesh->vertexArray = &model->vertexArray;
		mesh->vertexBuffer = &model->vertexBuffer;
		mesh->indexBuffer = &model->indexBuffer;
		mesh->meshDesc = OPNEW(OPmeshDesc());
		mesh->meshDesc->vertices = vertData;
		mesh->meshDesc->indices = indData;
		mesh->meshDesc->vertexCount = verticesCount;
		mesh->meshDesc->indexCount = indicesCount;
		mesh->meshDesc->indexSize = indexSize;



		vertexOffset += verticesCount;
		indexOffset += indicesCount;

		mesh->boundingBox.min.x = str->F32();
		mesh->boundingBox.min.y = str->F32();
		mesh->boundingBox.min.z = str->F32();
		mesh->boundingBox.max.x = str->F32();
		mesh->boundingBox.max.y = str->F32();
		mesh->boundingBox.max.z = str->F32();

		ui32 metaCount = str->UI32();
		if (metaCount > 0) {
			mesh->meshMeta = OPNEW(OPmeshMeta());
			mesh->meshMeta->count = metaCount;
			mesh->meshMeta->names = (OPchar**)OPALLOC(OPchar*, metaCount);
			mesh->meshMeta->data = (OPstream*)OPALLOC(OPstream, metaCount);

			for (ui32 j = 0; j < metaCount; j++) {
				OPchar* name = str->String();
				ui32 dataSize = str->UI32();
				if (dataSize == 0) continue;
				void* data = str->Read(dataSize);

				mesh->meshMeta->names[j] = name;
				mesh->meshMeta->data[j].Init(dataSize);
				mesh->meshMeta->data[j].Write(data, dataSize);
			}
		}
	}

	model->name = modelName;
	model->Build(totalVertices, totalIndices, indexSize, vertices, indices);

	return true;
}

OPint OPMloader(OPstream* str, void** model) {
	OPmodel** meshPtr = (OPmodel**)model;

	*meshPtr = OPNEW(OPmodel());

	if (_loadOPM(*meshPtr, str)) {
		return 1;
	}

	return 0;
}

OPint OPMreload(OPstream* str, void** model) {
	return 0;
}

OPint OPMunload(void* model) {
	return 0;
}



// Outdated
bool OPMloadDataV2(OPmodel* model, OPstream* str) {

	// Already loaded version at this point
	ui32 meshCount = str->UI32();

	OPuint indexOffset = 0;
	OPuint vertexOffset = 0;
	ui32 totalVerticesCount = str->UI32();
	ui32 totalIndicesCount = str->UI32();

	ui32 vertexCountTotal = 0;
	ui32 indexCountTotal = 0;

	OPvertexLayout vertexLayout;

	OPindexSize indexSize = OPindexSize::SHORT;// sizeof(ui16);
	void* vertices = OPalloc(totalVerticesCount);
	void* indices = OPalloc((ui16)indexSize * totalIndicesCount);

	for (ui32 i = 0; i < meshCount; i++) {
		ui16 vertexMode = str->UI16();

		if (vertexMode == 2) {
			OPlog("Version 2 not supported yet");
			continue;
		}

		ui32 features = str->UI32();
		ui32 verticesCount = str->UI32();
		ui32 indicesCount = str->UI32();

		vertexCountTotal += verticesCount;
		indexCountTotal += indicesCount;

		OPvertexLayoutBuilder layout;
		layout.Init();

		if (OPMhasFeature(features, Position))
			layout.Add(OPattributes::POSITION);
		if (OPMhasFeature(features, Normal))
			layout.Add(OPattributes::NORMAL);
		if (OPMhasFeature(features, Tangent))
			layout.Add(OPattributes::TANGENT);
		if (OPMhasFeature(features, BiTangent))
			layout.Add(OPattributes::BITANGENT);
		if (OPMhasFeature(features, UV))
			layout.Add(OPattributes::UV);
		if (OPMhasFeature(features, Color))
			layout.Add(OPattributes::COLOR);
		if (OPMhasFeature(features, Skinning))
			layout.Add(OPattributes::BONES);

		// TODO: (garrett) this doesn't support multi-mesh very well
		vertexLayout = layout.Build();
		//result.indices = OPalloc((ui32)result.indexSize * indicesCount);
		//result.vertices = OPalloc(result.vertexLayout.stride * verticesCount);

		f32* vertData = &((f32*)vertices)[vertexOffset * (vertexLayout.stride / sizeof(f32))];
		for (ui32 j = 0; j < verticesCount * (vertexLayout.stride / sizeof(f32)); j++) {
			vertData[j] = str->F32();
		}

		ui16* indData = &((ui16*)indices)[indexOffset];
		for (ui32 j = 0; j < indicesCount; j++) {
			indData[j] = str->UI16();
		}

		vertexOffset += verticesCount;
		indexOffset += indicesCount;

		ui32 metaCount = str->UI32();
		for (ui32 j = 0; j < metaCount; j++) {
			OPchar* name = str->String();
			ui32 dataSize = str->UI32();
			void* data = str->Read(dataSize);
		}
	}

	model->Init(meshCount, vertexLayout);
	model->Build(vertexCountTotal, indexCountTotal, indexSize, vertices, indices);

	OPmesh* mesh = &model->meshes[0];
	mesh->offset = 0;
	mesh->count = indexCountTotal;

	mesh->vertexArray = &model->vertexArray;
	mesh->vertexBuffer = &model->vertexBuffer;
	mesh->indexBuffer = &model->indexBuffer;
	mesh->meshDesc = OPNEW(OPmeshDesc());
	mesh->meshDesc->vertices = vertices;
	mesh->meshDesc->indices = indices;
	mesh->meshDesc->vertexCount = vertexCountTotal;
	mesh->meshDesc->indexCount = indexCountTotal;
	mesh->meshDesc->indexSize = indexSize;

	return true;
}

// Even more out dated
bool OPMloadV1(OPmodel* model, OPstream* str) {
	
	ui32 features = str->UI32();
	ui32 verticeCount = str->UI32();

	OPvec3 min = OPVEC3_ZERO;
	OPvec3 max = OPVEC3_ZERO;


	OPvec3* positions = NULL, *normals = NULL, *tangents = NULL, *bitangents = NULL, *colors = NULL;
	OPvec2* uvs = NULL;

	OPvec4* boneIndices = NULL;
	OPvec4* boneWeights = NULL;


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
			x = str->F32();
			y = str->F32();
			z = str->F32();
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
			x = str->F32();
			y = str->F32();
			z = str->F32();
			normals[i].x = x;
			normals[i].y = y;
			normals[i].z = z;
			normals[i] = OPvec3Norm(normals[i]);
			//OPlog("Normal: %f %f %f", x, y, z);
		}

		// Read Tangent
		if (OPMhasFeature(features, Tangent)) {
			x = str->F32();
			y = str->F32();
			z = str->F32();
			tangents[i].x = x;
			tangents[i].y = y;
			tangents[i].z = z;
			tangents[i] = OPvec3Norm(tangents[i]);
			//OPlog("Tangent: %f %f %f", x, y, z);
		}

		// Read BiTangent
		if (OPMhasFeature(features, BiTangent)) {
			x = str->F32();
			y = str->F32();
			z = str->F32();
			bitangents[i].x = x;
			bitangents[i].y = y;
			bitangents[i].z = z;
			bitangents[i] = OPvec3Norm(bitangents[i]);
			//OPlog("Tangent: %f %f %f", x, y, z);
		}

		// Read UV
		if (OPMhasFeature(features, UV)) {
			x = str->F32();
			y = str->F32();
			uvs[i].x = x;
			uvs[i].y = y;
			//OPlog("UV: %f %f", x, y);
		}

		// Read Color
		if (OPMhasFeature(features, Color)) {
			x = str->F32();
			y = str->F32();
			z = str->F32();
			colors[i].x = x;
			colors[i].y = y;
			colors[i].z = z;
		}

		// Read Skinning
		if (OPMhasFeature(features, Skinning)) {
			boneIndices[i].x = (f32)str->UI16();
			boneIndices[i].y = (f32)str->UI16();
			boneIndices[i].z = (f32)str->UI16();
			boneIndices[i].w = (f32)str->UI16();

			boneWeights[i].x = str->F32();
			boneWeights[i].y = str->F32();
			boneWeights[i].z = str->F32();
			boneWeights[i].w = str->F32();

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

	ui32 indicesCount = str->UI32();
	ui16* indices = (ui16*)OPalloc(sizeof(ui16)* (indicesCount * 3));
	for(ui32 i = 0; i < indicesCount; i++) {
		indices[i * 3 + 0] = str->UI16();
		indices[i * 3 + 1] = str->UI16();
		indices[i * 3 + 2] = str->UI16();
	}

	//i16* hierarchy = NULL;
	//OPmat4* pose = NULL;
	//ui16 hierarchyCount = 0;

	//// Read Bones
	//if(OPMhasFeature(features, Bones)) {
	//	OPlog("Has Bones");
	//	i32 boneCount = str->I16();
	//	hierarchyCount = boneCount;
	//	hierarchy = (i16*)OPalloc(sizeof(i16)* boneCount);
	//	pose = (OPmat4*)OPallocZero(sizeof(OPmat4)* boneCount);
	//
	//	for(i32 i = 0; i < boneCount; i++) {

	//		i32 boneIndex = str->I16();
	//		hierarchy[i] = boneIndex;

	//		OPchar* name = str->String();

	//		OPmat4* p = &(pose[i]);
	//		//OPlog("Mat4 Bone: %x", p);

	//		for (i32 c = 0; c < 4; c++) {
	//			p->cols[c].x = str->F32();
	//			p->cols[c].y = str->F32();
	//			p->cols[c].z = str->F32();
	//			p->cols[c].w = str->F32();
	//		}


	//		// f32 px = str->F32();
	//		// f32 py = str->F32();
	//		// f32 pz = str->F32();

	//		// f32 rx = str->F32();
	//		// f32 ry = str->F32();
	//		// f32 rz = str->F32();
	//		// f32 rw = str->F32();

	//		// f32 sx = str->F32();
	//		// f32 sy = str->F32();
	//		// f32 sz = str->F32();

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
	//	trackCount = str->I16();
	//	tracks = (OPMdataAnim*)OPalloc(sizeof(OPMdataAnim)* trackCount);
	//	for (OPint i = 0; i < trackCount; i++) {
	//		tracks[i].Name = str->String();

	//		tracks[i].FrameCount = str->UI32();

	//		OPint totalFrames = tracks[i].FrameCount * hierarchyCount;
	//		tracks[i].Frames = (OPmat4*)OPalloc(sizeof(OPmat4)* totalFrames);

	//		for (OPint j = 0; j < totalFrames; j++) {

	//			OPmat4* p = &(tracks[i].Frames[j]);
	//			//OPlog("Mat4 Bone: %x", p);

	//			for (i32 c = 0; c < 4; c++) {
	//				p->cols[c].x = str->F32();
	//				p->cols[c].y = str->F32();
	//				p->cols[c].z = str->F32();
	//				p->cols[c].w = str->F32();
	//			}

	//			tracks[i].Frames[j] = OPmat4Transpose(tracks[i].Frames[j]);
	//		}
	//	}
	//}

	//OPlog("Index Count: %d", indicesCount);
	//OPlog("Vertex Count: %d", verticeCount);


	// Read Animation
	//if(OPMhasFeature(features, Animations)) {
	//	i8* name = str->String();
	//	ui32 keyframes = str->UI32();
	//	OPlog("Animation: %s", name);
	//	OPlog("Frame Count %d", keyframes);
	//	for(i32 i = 0; i < keyframes; i++) {
	//		i32 index = str->I32();
	//		ui32 keys = str->UI32();
	//		OPlog("Keyframe %d", i);
	//		OPlog("Bone %d", index);
	//		for(i32 j = 0; j < keys; j++) {
	//			ui32 keyFeatures = str->UI32();
	//			OPlog("Key: %d", (j + 1));
	//			if(keyFeatures && Key_Time) {
	//				f32 time = str->F32();
	//				OPlog("Keyframe Time: %f", time);
	//			}

	//			if(keyFeatures && Key_Position) {
	//				f32 x = str->F32();
	//				f32 y = str->F32();
	//				f32 z = str->F32();
	//				//OPlog("Position: %f, %f, %f", x, y, z);
	//			}

	//			if(keyFeatures && Key_Rotation) {
	//				f32 x = str->F32();
	//				f32 y = str->F32();
	//				f32 z = str->F32();
	//				f32 w = str->F32();
	//				OPlog("Rotation: %f, %f, %f, %f", x, y, z, w);
	//			}
	//		}
	//	}
	//	OPfree(name);
	//}






	OPvertexLayout vertexLayout = layout.Build();
	ui16* indicesData = indices;
	ui32 indexCount = indicesCount * 3;
	OPindexSize indexSize = OPindexSize::SHORT;// sizeof(ui16);
	ui32 vertexCount = verticeCount;

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


	if (positions) OPfree(positions);
	if (normals) OPfree(normals);
	if (tangents) OPfree(tangents);
	if (bitangents) OPfree(bitangents);
	if (colors) OPfree(colors);
	if (uvs) OPfree(uvs);
	if (boneIndices) OPfree(boneIndices);
	if (boneWeights) OPfree(boneWeights);

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

	void* vertsData = vertices->data;
	OPfree(vertices);

	//data.vertexSize = vertices->size * sizeof(f32);

	OPboundingBox3D bounds = OPboundingBox3D(min, max);

	//data.hierarchy = hierarchy;
	//data.pose = pose;
	//data.hierarchyCount = hierarchyCount;
	//data.trackCount = trackCount;
	//data.tracks = tracks;



	//if (OPMhasFeature(features, Meta)) {
	//	ui16 metaCount = str->UI16();
	//	data.metaCount = metaCount;

	//	if (metaCount > 0) {
	//		//OPlog("Meta Count: %d", metaCount);
	//		OPMmeta* meta = (OPMmeta*)OPalloc(sizeof(OPMmeta) * metaCount);
	//		for (i32 i = 0; i < metaCount; i++) {
	//			OPchar* metaName = str->String();
	//			OPchar* metaType = str->String();
	//			OPlog("Meta Name: %s (%s)", metaName, metaType);
	//			f32 x = str->F32();
	//			f32 y = str->F32();
	//			f32 z = str->F32();
	//			f32 rx = str->F32();
	//			f32 ry = str->F32();
	//			f32 rz = str->F32();
	//			f32 sx = str->F32();
	//			f32 sy = str->F32();
	//			f32 sz = str->F32();
	//			meta[i].Name = metaName;
	//			meta[i].Type = metaType;
	//			meta[i].Position = OPvec3Create(x, y, z);
	//			meta[i].Rotation = OPvec3Create(rx, ry, rz);
	//			meta[i].Scale = OPvec3Create(sx, sy, sz);
	//		}
	//		data.meta = meta;
	//	}
	//}

	model->Init(1, vertexLayout);
	model->Build(vertexCount, indexCount, indexSize, vertsData, indicesData);

	OPmesh* mesh = &model->meshes[0];
	mesh->offset = 0;
	mesh->count = indexCount;

	mesh->vertexArray = &model->vertexArray;
	mesh->vertexBuffer = &model->vertexBuffer;
	mesh->indexBuffer = &model->indexBuffer;
	mesh->meshDesc = OPNEW(OPmeshDesc());
	mesh->meshDesc->vertices = vertsData;
	mesh->meshDesc->indices = indices;
	mesh->meshDesc->vertexCount = vertexCount;
	mesh->meshDesc->indexCount = indexCount;
	mesh->meshDesc->indexSize = indexSize;

	return true;
}