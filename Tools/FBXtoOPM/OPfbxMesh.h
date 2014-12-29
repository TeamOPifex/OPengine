#pragma once

#include "./Engine.h"
#include "OPfbxSdk.h"
#include "OPfbxSkeleton.h"
#include "OPfbxMeshData.h"
#include "OPfbxSkin.h"
#include "Helpers.h"

typedef struct {
	OPfbxSdk SDK;
	OPfbxScene SCENE;

	OPfbxSkeleton Skeleton;
	OPfbxMeshData MeshData;

	ui32* Indices;
	OPvec3* Positions;
	OPvec2* UVs;
	OPvec3* Normals;

	OPfbxSkin Skin;

} OPfbxMesh;

OPint OPfbxMeshCreate(OPfbxMesh* mesh, const OPchar* filename) {

	mesh->SDK = OPfbxSdkCreate();
	mesh->SCENE;
	if(OPfbxSdkLoadScene(&mesh->SCENE, &mesh->SDK, filename) == 0) {

		OPlog("Step: FBX Scene Loaded");

		OPfbxSkeletonGet(&mesh->Skeleton, &mesh->SCENE);

		OPlog("Step: Skeleton");

		OPfbxMeshDataGet(&mesh->MeshData, &mesh->SCENE);

		OPlog("Step: MeshData");

		mesh->Indices = OPfbxMeshDataGetIndices(&mesh->MeshData);
		OPlog("Step: indices");
		mesh->Positions = OPfbxMeshDataGetPositions(&mesh->MeshData);
		OPlog("Step: positions");
		mesh->UVs = OPfbxMeshDataGetUVs(&mesh->MeshData);
		OPlog("Step: uvs");
		mesh->Normals = OPfbxMeshDataGetNormals(&mesh->MeshData);
		OPlog("Step: normals");

		OPfbxSkinGet(&mesh->Skin, &mesh->MeshData, &mesh->Skeleton);
		OPlog("Step: skin");

		OPlog("End FBX SDK");
		//OPfbxSdkDestroy(&SDK);

		return 0; // Success
	}

	OPfbxSdkDestroy(&mesh->SDK);

	OPlog("Failed to create the mesh from the given input file.");
	return 1;
}

OPint OPfbxMeshWriteToFile(OPfbxMesh* mesh, const OPchar* filename, OPint* features) {
	ofstream myFile(filename, ios::binary);
	OPlog("Begin Writing File...");

	// OPM File Format Version
	writeU16(&myFile, 1);

	OPlog("Feature[Model_Positions]: %d", features[Model_Positions]);
	OPlog("Feature[Model_Normals]: %d", features[Model_Normals]);
	OPlog("Feature[Model_UVs]: %d", features[Model_UVs]);
	OPlog("Feature[Model_Colors]: %d", features[Model_Colors]);
	OPlog("Feature[Model_Indices]: %d", features[Model_Indices]);
	OPlog("Feature[Model_Bones]: %d", features[Model_Bones]);
	OPlog("Feature[Model_Skinning]: %d", features[Model_Skinning]);
	OPlog("Feature[Model_Animations]: %d", features[Model_Animations]);

	ui32 feature = 0;
	if (features[Model_Positions]) feature += 0x01;
	if (features[Model_Normals]) feature += 0x02;
	if (features[Model_UVs]) feature += 0x04;
	if (features[Model_Colors]) feature += 0x100;
	if (features[Model_Indices]) feature += 0x10;
	if (features[Model_Bones]) feature += 0x20;
	if (features[Model_Skinning]) feature += 0x40;
	if (features[Model_Animations]) feature += 0x80;


	OPlog("Feature: %d", feature);
	// OPM File Features
	writeU32(&myFile, feature);

	OPlog("Vertex Count: %d", mesh->MeshData.VertexCount);
	// Vertex Count
	writeU32(&myFile, mesh->MeshData.VertexCount);
	for (OPint i = 0; i < mesh->MeshData.VertexCount; i++) {

		if (features[Model_Positions]) {
			OPlg(".");
			OPvec3 pos = mesh->Positions[i];
			writeF32(&myFile, pos.x);
			writeF32(&myFile, pos.y);
			writeF32(&myFile, pos.z);
		}

		if (features[Model_Normals]) {
			OPlg("-");
			OPvec3 norm = mesh->Normals[i];
			writeF32(&myFile, norm.x);
			writeF32(&myFile, norm.y);
			writeF32(&myFile, norm.z);
		}

		if (features[Model_UVs]) {
			OPlg("=");
			OPvec2 uv = mesh->UVs[i];
			writeF32(&myFile, uv.x);
			writeF32(&myFile, uv.y);
		}

		if (features[Model_Skinning]) {
			OPlg("+");
			OPint ind1 = mesh->Skin.BoneIndices[i * 4 + 0];
			OPint ind2 = mesh->Skin.BoneIndices[i * 4 + 1];
			OPint ind3 = mesh->Skin.BoneIndices[i * 4 + 2];
			OPint ind4 = mesh->Skin.BoneIndices[i * 4 + 3];

			f32 w1 = mesh->Skin.BoneWeights[i * 4 + 0];
			f32 w2 = mesh->Skin.BoneWeights[i * 4 + 1];
			f32 w3 = mesh->Skin.BoneWeights[i * 4 + 2];
			f32 w4 = mesh->Skin.BoneWeights[i * 4 + 3];

			writeI16(&myFile, ind1);
			writeI16(&myFile, ind2);
			writeI16(&myFile, ind3);
			writeI16(&myFile, ind4);

			writeF32(&myFile, w1);
			writeF32(&myFile, w2);
			writeF32(&myFile, w3);
			writeF32(&myFile, w4);


			OPlog("%d: %d %d %d %d %f %f %f %f", i,
				ind1,
				ind2,
				ind3,
				ind4,
				w1,
				w2,
				w3,
				w4);
		}

	}

	if (features[Model_Indices]) {
		writeU32(&myFile, mesh->MeshData.IndexCount / 3);

		for (i32 i = 0; i < mesh->MeshData.IndexCount; i++) {
			writeU16(&myFile, mesh->Indices[i]);
		}
	}

	if (features[Model_Bones]) {
		writeI16(&myFile, mesh->Skeleton.BoneCount);
		for(OPint i = 0; i < mesh->Skeleton.BoneCount; i++) {
			writeI16(&myFile, mesh->Skeleton.Bones[i].Parent); // TODO: Bone Parent

			ui32 len = strlen(mesh->Skeleton.Bones[i].Name);
			writeU32(&myFile, len);
			write(&myFile, mesh->Skeleton.Bones[i].Name, len);
			for (i32 c = 0; c < 4; c++) {
				writeF32(&myFile, mesh->Skeleton.Bones[i].BindPose[c].x);
				writeF32(&myFile, mesh->Skeleton.Bones[i].BindPose[c].y);
				writeF32(&myFile, mesh->Skeleton.Bones[i].BindPose[c].z);
				writeF32(&myFile, mesh->Skeleton.Bones[i].BindPose[c].w);
			}
		}
		//WriteSkeleton(skel, &myFile);
	}

	return 0;
}


