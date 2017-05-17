#pragma once

#include "./OPengine.h"
#include "OPfbxSdk.h"
#include "OPfbxSkeleton.h"
#include "OPfbxMeshData.h"
#include "OPfbxSkin.h"
#include "OPfbxAnimation.h"
#include "Helpers.h"

#include "./Human/include/Rendering/OPMvertex.h"

enum ModelFeatures {
	Model_Positions = 0,
	Model_Normals = 1,
	Model_UVs = 2,
	Model_Colors = 3,
	Model_Indices = 4,
	Model_Tangents = 5,
	Model_Bones = 6,
	Model_Skinning = 7,
	Model_Animations = 8,
	Model_Skeletons = 9,
	Model_Meta = 10,
	Model_Bitangents = 11,
	MAX_FEATURES
};

typedef struct {
	OPfbxSdk SDK;
	OPfbxScene SCENE;

	OPfbxSkeleton Skeleton;

	OPuint MeshDataCount;
	OPfbxMeshData* MeshData;

	OPfbxAnimation* Animation;

} OPfbxMesh;

OPint OPfbxMeshCreate(OPfbxMesh* mesh, OPfloat scale, const OPchar* filename) {

	mesh->SDK = OPfbxSdkCreate();
	mesh->SCENE;
	if(OPfbxSdkLoadScene(&mesh->SCENE, &mesh->SDK, filename) == 0) {

		OPlogDebug("Step: FBX Scene Loaded");

		OPfbxSkeletonGet(&mesh->Skeleton, &mesh->SCENE);

		OPlogDebug("Step: Skeleton");

		mesh->MeshDataCount = OPfbxMeshDataGet(&mesh->MeshData, &mesh->SCENE);
		OPlogDebug("Step: MeshData");

		OPfbxMeshDataGetPolygons(&mesh->MeshData, mesh->MeshDataCount, scale);
		_skinBlendWeights(&mesh->MeshData, mesh->MeshDataCount, &mesh->Skeleton, &mesh->SCENE);
		OPlogDebug("Step: skin");

		mesh->Animation = OPfbxAnimationGet(&mesh->MeshData, mesh->MeshDataCount, &mesh->Skeleton, &mesh->SCENE, scale);

		//mesh->MeshData.MetaCount = 0;

		OPlogDebug("End FBX SDK");
		//OPfbxSdkDestroy(&SDK);

		return 0; // Success
	}

	OPfbxSdkDestroy(&mesh->SDK);

	OPlogDebug("Failed to create the mesh from the given input file.");
	return 1;
}

OPint OPfbxMeshWriteToFileV3(OPfbxMesh* mesh, const OPchar* filename, OPint* features, OPint animationCount, OPchar** animationNames, OPint* animationFrames) {
	// Version 3
	OPchar* output = OPstringCreateMerged(filename, ".opm");
	ofstream myFile(output, ios::binary);
	OPlogDebug("Begin Writing File...");

	// OPM File Format Version
	writeU16(&myFile, 3);

	const OPchar* name = "exported mesh";
	writeString(&myFile, filename);


	writeU32(&myFile, mesh->MeshDataCount);



	ui32 featureFlags = 0;
	if (features[Model_Positions]) featureFlags += 0x01;
	if (features[Model_Normals]) featureFlags += 0x02;
	if (features[Model_UVs]) featureFlags += 0x04;
	if (features[Model_Tangents]) featureFlags += 0x08;
	if (features[Model_Bitangents]) featureFlags += 0x400;
	if (features[Model_Colors]) featureFlags += 0x100;
	if (features[Model_Indices]) featureFlags += 0x10;
	if (features[Model_Bones]) featureFlags += 0x20;
	if (features[Model_Skinning]) featureFlags += 0x40;
	if (features[Model_Animations]) featureFlags += 0x80;
	if (features[Model_Meta]) featureFlags += 0x200;

	// Features in the OPM
	writeU32(&myFile, featureFlags);



	// Vertex Mode
	// 1 == Vertex Stride ( Pos/Norm/Uv )[]
	// 2 == Vertex Arrays ( Pos )[] ( Norm )[] ( Uv )[]
	writeU16(&myFile, 1);


	ui32 totalVerticesEntireModel = 0;
	ui32 totalIndicesEntireModel = 0;

	for (ui32 i = 0; i < mesh->MeshDataCount; i++) {
		totalVerticesEntireModel += mesh->MeshData[i].VertexCount;
		totalIndicesEntireModel += mesh->MeshData[i].IndexCount;
	}

	writeU32(&myFile, totalVerticesEntireModel);
	writeU32(&myFile, totalIndicesEntireModel);

	OPindexSize indexSize = OPindexSize::INT;
	writeU8(&myFile, (ui8)indexSize);


	ui32 offset = 0;
	for (ui32 i = 0; i < mesh->MeshDataCount; i++) {
		OPfbxMeshData meshData = mesh->MeshData[i];
		OPboundingBox3D boundingBox;


		writeString(&myFile, name);

		writeU32(&myFile, meshData.VertexCount);
		writeU32(&myFile, meshData.IndexCount);

		for (ui32 j = 0; j < meshData.PolyCount; j++) {
			OPfbxMeshPoly poly = meshData.Polys[j];
			for (ui32 k = 0; k < poly.Size; k++) {
				writeF32(&myFile, poly.Position[k].x);
				writeF32(&myFile, poly.Position[k].y);
				writeF32(&myFile, poly.Position[k].z);


				if (poly.Position[k].x < boundingBox.min.x) boundingBox.min.x = poly.Position[k].x;
				if (poly.Position[k].y < boundingBox.min.y) boundingBox.min.y = poly.Position[k].y;
				if (poly.Position[k].z < boundingBox.min.z) boundingBox.min.z = poly.Position[k].z;
				if (poly.Position[k].x > boundingBox.max.x) boundingBox.max.x = poly.Position[k].x;
				if (poly.Position[k].y > boundingBox.max.y) boundingBox.max.y = poly.Position[k].y;
				if (poly.Position[k].z > boundingBox.max.z) boundingBox.max.z = poly.Position[k].z;

				// Normal
				if (features[Model_Normals]) {
					writeF32(&myFile, poly.Normal[k].x);
					writeF32(&myFile, poly.Normal[k].y);
					writeF32(&myFile, poly.Normal[k].z);
				}

				if (features[Model_Tangents]) {
					writeF32(&myFile, poly.Tangent[k].x);
					writeF32(&myFile, poly.Tangent[k].y);
					writeF32(&myFile, poly.Tangent[k].z);
				}

				if (features[Model_Bitangents]) {
					writeF32(&myFile, poly.BiTangent[k].x);
					writeF32(&myFile, poly.BiTangent[k].y);
					writeF32(&myFile, poly.BiTangent[k].z);
				}

				if (features[Model_UVs]) {
					writeF32(&myFile, poly.UV[k].x);
					writeF32(&myFile, poly.UV[k].y);
				}

				if (features[Model_Bones]) {

					OPint cp = poly.ControlPointIndex[k];
					OPfbxSkinBlendWeight bw = meshData.BlendWeights[cp];
					OPint boneIndex;

					if (bw.c > 0) {
						boneIndex = OPfbxSkeletonGetPos(&mesh->Skeleton, bw.b1);
						//writeI16(&myFile, boneIndex);
						writeF32(&myFile, boneIndex);
					}
					if (bw.c > 1) {
						boneIndex = OPfbxSkeletonGetPos(&mesh->Skeleton, bw.b2);
						//writeI16(&myFile, boneIndex);
						writeF32(&myFile, boneIndex);
					}
					if (bw.c > 2) {
						boneIndex = OPfbxSkeletonGetPos(&mesh->Skeleton, bw.b3);
						//writeI16(&myFile, boneIndex);
						writeF32(&myFile, boneIndex);
					}
					if (bw.c > 3) {
						boneIndex = OPfbxSkeletonGetPos(&mesh->Skeleton, bw.b4);
						//writeI16(&myFile, boneIndex);
						writeF32(&myFile, boneIndex);
					}

					for (OPint k = 0; k < 4 - bw.c; k++) {
						//writeI16(&myFile, -1);
						writeF32(&myFile, -1);
					}


					if (bw.c > 0) {
						writeF32(&myFile, bw.w1);
					}
					if (bw.c > 1) {
						writeF32(&myFile, bw.w2);
					}
					if (bw.c > 2) {
						writeF32(&myFile, bw.w3);
					}
					if (bw.c > 3) {
						writeF32(&myFile, bw.w4);
					}

					for (OPint k = 0; k < 4 - bw.c; k++) {
						writeF32(&myFile, 0.0f);
					}
				}

				// TODO
				//if (features[Model_Colors]) {
				//	writeF32(&myFile, poly.colors[k].r);
				//	writeF32(&myFile, colors[k].g);
				//	writeF32(&myFile, colors[k].b);
				//}
			}
		}


		if (features[Model_Indices]) {
			for (OPint i = 0; i < meshData.PolyCount; i++) {
				if (meshData.Polys[i].Size == 3) {
					writeU32(&myFile, offset + 0);
					writeU32(&myFile, offset + 1);
					writeU32(&myFile, offset + 2);
					offset += 3;
				}
				else {
					writeU32(&myFile, offset + 0);
					writeU32(&myFile, offset + 1);
					writeU32(&myFile, offset + 2);

					writeU32(&myFile, offset + 0);
					writeU32(&myFile, offset + 2);
					writeU32(&myFile, offset + 3);
					offset += 4;
				}
			}
		}


		writeF32(&myFile, boundingBox.min.x);
		writeF32(&myFile, boundingBox.min.y);
		writeF32(&myFile, boundingBox.min.z);
		writeF32(&myFile, boundingBox.max.x);
		writeF32(&myFile, boundingBox.max.y);
		writeF32(&myFile, boundingBox.max.z);


		
		writeString(&myFile, ""); // diffuse
		writeString(&myFile, "");
		writeString(&myFile, ""); // ambient
		writeString(&myFile, "");
		writeString(&myFile, ""); // height
		writeString(&myFile, "");
		writeString(&myFile, ""); // shininess
		writeString(&myFile, "");
		writeString(&myFile, ""); // displacement
		writeString(&myFile, "");
		writeString(&myFile, ""); // reflection
		writeString(&myFile, "");
		writeString(&myFile, ""); // other2
		writeString(&myFile, "");
		//writeString(&myFile, model->meshes[i].materialDesc->diffuse);
		//writeString(&myFile, model->meshes[i].materialDesc->specular);
		//writeString(&myFile, model->meshes[i].materialDesc->ambient);
		//writeString(&myFile, model->meshes[i].materialDesc->emissive);
		//writeString(&myFile, model->meshes[i].materialDesc->height);
		//writeString(&myFile, model->meshes[i].materialDesc->normals);
		//writeString(&myFile, model->meshes[i].materialDesc->shininess);
		//writeString(&myFile, model->meshes[i].materialDesc->opacity);
		//writeString(&myFile, model->meshes[i].materialDesc->displacement);
		//writeString(&myFile, model->meshes[i].materialDesc->lightMap);
		//writeString(&myFile, model->meshes[i].materialDesc->reflection);
		//writeString(&myFile, model->meshes[i].materialDesc->other1);
		//writeString(&myFile, model->meshes[i].materialDesc->other2);
		//writeString(&myFile, model->meshes[i].materialDesc->other3);

		writeU32(&myFile, 0);

	}

	myFile.close(); if (features[Model_Bones]) {
		OPchar* skel = OPstringCreateMerged(output, ".skel");
		ofstream mySkelFile(skel, ios::binary);
		writeI16(&mySkelFile, mesh->Skeleton.BoneCount);
		for (OPint i = 0; i < mesh->Skeleton.BoneCount; i++) {
			writeI16(&mySkelFile, mesh->Skeleton.Bones[i].Parent); // TODO: Bone Parent

			ui32 len = strlen(mesh->Skeleton.Bones[i].Name);
			writeU32(&mySkelFile, len);
			write(&mySkelFile, mesh->Skeleton.Bones[i].Name, len);
			for (i32 c = 0; c < 4; c++) {
				writeF32(&mySkelFile, mesh->Skeleton.Bones[i].BindPose[c].x);
				writeF32(&mySkelFile, mesh->Skeleton.Bones[i].BindPose[c].y);
				writeF32(&mySkelFile, mesh->Skeleton.Bones[i].BindPose[c].z);
				writeF32(&mySkelFile, mesh->Skeleton.Bones[i].BindPose[c].w);
			}
		}
	}


	if (features[Model_Animations]) {
		OPlog("Track Count: %d", mesh->Animation->AnimationTrackCount);
		if (mesh->Animation->AnimationTrackCount == 1 && animationCount > 0) {

			OPlogDebug("Track Name to split: %s", mesh->Animation->Animations[0].Name);

			for (OPint i = 0; i < animationCount; i++) {
				OPlog("Anim %s", animationNames[i]);
				OPchar* prefix = OPstringCreateMerged(".", animationNames[i]);
				OPchar* animName = OPstringCreateMerged(prefix, ".anim");
				OPchar* anim = OPstringCreateMerged(output, animName);
				ofstream myAnimFile(anim, ios::binary);

				writeI16(&myAnimFile, mesh->Skeleton.BoneCount);

				ui32 len = strlen(animationNames[i]);
				writeU32(&myAnimFile, len);
				write(&myAnimFile, animationNames[i], len);

				OPint startFrame = animationFrames[i * 2];
				OPint endFrame = animationFrames[i * 2 + 1];
				OPint totalFrames = endFrame - startFrame;

				writeU32(&myAnimFile, totalFrames);
				for (OPint j = startFrame * mesh->Skeleton.BoneCount; j < endFrame * mesh->Skeleton.BoneCount; j++) {
					//OPlg("%d ", j);
					//OPmat4Log("Frame", mesh->Animation->Animations[0].JointTransform[j]);
					for (i32 c = 0; c < 4; c++) {
						writeF32(&myAnimFile, mesh->Animation->Animations[0].JointTransform[j][c].x);
						writeF32(&myAnimFile, mesh->Animation->Animations[0].JointTransform[j][c].y);
						writeF32(&myAnimFile, mesh->Animation->Animations[0].JointTransform[j][c].z);
						writeF32(&myAnimFile, mesh->Animation->Animations[0].JointTransform[j][c].w);
					}
				}
			}
		}
		else {
			//writeI16(&myAnimFile, mesh->Animation->AnimationTrackCount);
			for (OPint i = 0; i < mesh->Animation->AnimationTrackCount; i++) {
				OPlogDebug("Track Name: %s", mesh->Animation->Animations[i].Name);
				OPchar* prefix = OPstringCreateMerged(".", mesh->Animation->Animations[i].Name);
				OPchar* animName = OPstringCreateMerged(prefix, ".anim");
				OPchar* anim = OPstringCreateMerged(output, animName);
				ofstream myAnimFile(anim, ios::binary);

				writeI16(&myAnimFile, mesh->Skeleton.BoneCount);

				ui32 len = strlen(mesh->Animation->Animations[i].Name);
				writeU32(&myAnimFile, len);
				write(&myAnimFile, mesh->Animation->Animations[i].Name, len);

				writeU32(&myAnimFile, mesh->Animation->Animations[i].TotalFrames);
				for (OPint j = 0; j < mesh->Animation->Animations[i].TotalFrames * mesh->Skeleton.BoneCount; j++) {
					//OPlg("%d ", j);
					//OPmat4Log("Frame", mesh->Animation->Animations[i].JointTransform[j]);
					for (i32 c = 0; c < 4; c++) {
						writeF32(&myAnimFile, mesh->Animation->Animations[i].JointTransform[j][c].x);
						writeF32(&myAnimFile, mesh->Animation->Animations[i].JointTransform[j][c].y);
						writeF32(&myAnimFile, mesh->Animation->Animations[i].JointTransform[j][c].z);
						writeF32(&myAnimFile, mesh->Animation->Animations[i].JointTransform[j][c].w);
					}
				}

			}
		}


	}

	return 0;
}

OPint OPfbxMeshWriteToFile(OPfbxMesh* mesh, const OPchar* filename, OPint* features, OPint animationCount, OPchar** animationNames, OPint* animationFrames) {
	return OPfbxMeshWriteToFileV3(mesh, filename, features, animationCount, animationNames, animationFrames);


	OPchar* output = OPstringCreateMerged(filename, ".opm");
	ofstream myFile(output, ios::binary);
	OPlogDebug("Begin Writing File...");

	// OPM File Format Version
	writeU16(&myFile, 1);

	OPlogDebug("Feature[Model_Positions]: %d", features[Model_Positions]);
	OPlogDebug("Feature[Model_Normals]: %d", features[Model_Normals]);
	OPlogDebug("Feature[Model_UVs]: %d", features[Model_UVs]);
	OPlogDebug("Feature[Model_Colors]: %d", features[Model_Colors]);
	OPlogDebug("Feature[Model_Indices]: %d", features[Model_Indices]);
	OPlogDebug("Feature[Model_Bones]: %d", features[Model_Bones]);
	OPlogDebug("Feature[Model_Skinning]: %d", features[Model_Skinning]);
	OPlogDebug("Feature[Model_Animations]: %d", features[Model_Animations]);
	OPlogDebug("Feature[Model_Meta]: %d", features[Model_Meta]);

	ui32 feature = 0;
	if (features[Model_Positions]) feature += 0x01;
	if (features[Model_Normals]) feature += 0x02;
	if (features[Model_UVs]) feature += 0x04;
	if (features[Model_Colors]) feature += 0x100;
	if (features[Model_Indices]) feature += 0x10;
	if (features[Model_Tangents]) feature += 0x08;
	if (features[Model_Bones]) feature += 0x20;
	if (features[Model_Skinning]) feature += 0x40;
	if (features[Model_Animations]) feature += 0x80;
	if (features[Model_Meta]) feature += 0x200;
	if (features[Model_Bitangents]) feature += 0x400;


	OPlogDebug("Feature: %d", feature);
	// OPM File Features
	writeU32(&myFile, feature);

	OPint verticeCount = 0;
	OPint indCount = 0;
	for (OPint i = 0; i < mesh->MeshData[0].PolyCount; i++) {
		verticeCount += mesh->MeshData[0].Polys[i].Size;
		indCount += mesh->MeshData[0].Polys[i].Size == 3 ? 3 : 6;
	}

	OPlogDebug("Vertex Count: %d", verticeCount);
	// Vertex Count
	writeU32(&myFile, verticeCount);

	for (OPint i = 0; i < mesh->MeshData[0].PolyCount; i++) {
		for (OPint j = 0; j < mesh->MeshData[0].Polys[i].Size; j++) {

			if (features[Model_Positions]) {
				//OPlg(".");
				OPvec3 pos = mesh->MeshData[0].Polys[i].Position[j];
				writeF32(&myFile, pos.x);
				writeF32(&myFile, pos.y);
				writeF32(&myFile, pos.z);
			}

			if (features[Model_Normals]) {
				//OPlg("-");
				OPvec3 norm = mesh->MeshData[0].Polys[i].Normal[j];
				writeF32(&myFile, norm.x);
				writeF32(&myFile, norm.y);
				writeF32(&myFile, norm.z);
			}

			if (features[Model_Tangents]) {
				//OPlg("%");
				OPvec4 tan = mesh->MeshData[0].Polys[i].Tangent[j];
				writeF32(&myFile, tan.x);
				writeF32(&myFile, tan.y);
				writeF32(&myFile, tan.z);
			}

			if (features[Model_Bitangents]) {
				//OPlg("%");
				OPvec4 bitan = mesh->MeshData[0].Polys[i].BiTangent[j];
				writeF32(&myFile, bitan.x);
				writeF32(&myFile, bitan.y);
				writeF32(&myFile, bitan.z);
			}

			if (features[Model_UVs]) {
				//OPlg("=");
				OPvec2 uv = mesh->MeshData[0].Polys[i].UV[j];
				writeF32(&myFile, uv.x);
				writeF32(&myFile, uv.y);
			}

			if (features[Model_Skinning]) {
				//OPlg("+");
				OPint cp = mesh->MeshData[0].Polys[i].ControlPointIndex[j];
				OPfbxSkinBlendWeight bw = mesh->MeshData[0].BlendWeights[cp];
				OPint boneIndex;

				if (bw.c > 0) {
					boneIndex = OPfbxSkeletonGetPos(&mesh->Skeleton, mesh->MeshData[0].BlendWeights[cp].b1);
					writeI16(&myFile, boneIndex);
				}
				if (bw.c > 1) {
					boneIndex = OPfbxSkeletonGetPos(&mesh->Skeleton, mesh->MeshData[0].BlendWeights[cp].b2);
					writeI16(&myFile, boneIndex);
				}
				if (bw.c > 2) {
					boneIndex = OPfbxSkeletonGetPos(&mesh->Skeleton, mesh->MeshData[0].BlendWeights[cp].b3);
					writeI16(&myFile, boneIndex);
				}
				if (bw.c > 3) {
					boneIndex = OPfbxSkeletonGetPos(&mesh->Skeleton, mesh->MeshData[0].BlendWeights[cp].b4);
					writeI16(&myFile, boneIndex);
				}

				for (OPint k = 0; k < 4 - bw.c; k++) {
					writeI16(&myFile, -1);
				}


				if (bw.c > 0) {
					writeF32(&myFile, mesh->MeshData[0].BlendWeights[cp].w1);
				}
				if (bw.c > 1) {
					writeF32(&myFile, mesh->MeshData[0].BlendWeights[cp].w2);
				}
				if (bw.c > 2) {
					writeF32(&myFile, mesh->MeshData[0].BlendWeights[cp].w3);
				}
				if (bw.c > 3) {
					writeF32(&myFile, mesh->MeshData[0].BlendWeights[cp].w4);
				}

				for (OPint k = 0; k < 4 - bw.c; k++) {
					writeF32(&myFile, 0.0f);
				}

			}
		}
	}

	OPint offset = 0;
	if (features[Model_Indices]) {
		writeU32(&myFile, indCount / 3);

		for (OPint i = 0; i < mesh->MeshData[0].PolyCount; i++) {
			if (mesh->MeshData[0].Polys[i].Size == 3) {
				writeU16(&myFile, offset + 0);
				writeU16(&myFile, offset + 1);
				writeU16(&myFile, offset + 2);
				offset += 3;
			}
			else {
				writeU16(&myFile, offset + 0);
				writeU16(&myFile, offset + 1);
				writeU16(&myFile, offset + 2);

				writeU16(&myFile, offset + 0);
				writeU16(&myFile, offset + 2);
				writeU16(&myFile, offset + 3);
				offset += 4;
			}
		}
	}


	if (features[Model_Meta]) {
		writeU16(&myFile, mesh->MeshData[0].MetaCount);
		for(OPuint i = 0; i < mesh->MeshData[0].MetaCount; i++) {

			// Name
			ui32 len = strlen(mesh->MeshData[0].Meta[i].Name);
			writeU32(&myFile, len);
			write(&myFile, mesh->MeshData[0].Meta[i].Name, len);

			// Type
			len = strlen(mesh->MeshData[0].Meta[i].Type);
			writeU32(&myFile, len);
			write(&myFile, mesh->MeshData[0].Meta[i].Type, len);

			writeF32(&myFile, mesh->MeshData[0].Meta[i].Position[0]);
			writeF32(&myFile, mesh->MeshData[0].Meta[i].Position[1]);
			writeF32(&myFile, mesh->MeshData[0].Meta[i].Position[2]);

			writeF32(&myFile, mesh->MeshData[0].Meta[i].Rotation[0]);
			writeF32(&myFile, mesh->MeshData[0].Meta[i].Rotation[1]);
			writeF32(&myFile, mesh->MeshData[0].Meta[i].Rotation[2]);

			writeF32(&myFile, mesh->MeshData[0].Meta[i].Scale[0]);
			writeF32(&myFile, mesh->MeshData[0].Meta[i].Scale[1]);
			writeF32(&myFile, mesh->MeshData[0].Meta[i].Scale[2]);
		}
	}


	if (features[Model_Bones]) {
		OPchar* skel = OPstringCreateMerged(output, ".skel");
		ofstream mySkelFile(skel, ios::binary);
		writeI16(&mySkelFile, mesh->Skeleton.BoneCount);
		for (OPint i = 0; i < mesh->Skeleton.BoneCount; i++) {
			writeI16(&mySkelFile, mesh->Skeleton.Bones[i].Parent); // TODO: Bone Parent

			ui32 len = strlen(mesh->Skeleton.Bones[i].Name);
			writeU32(&mySkelFile, len);
			write(&mySkelFile, mesh->Skeleton.Bones[i].Name, len);
			for (i32 c = 0; c < 4; c++) {
				writeF32(&mySkelFile, mesh->Skeleton.Bones[i].BindPose[c].x);
				writeF32(&mySkelFile, mesh->Skeleton.Bones[i].BindPose[c].y);
				writeF32(&mySkelFile, mesh->Skeleton.Bones[i].BindPose[c].z);
				writeF32(&mySkelFile, mesh->Skeleton.Bones[i].BindPose[c].w);
			}
		}
	}
	

	if (features[Model_Animations]) {
		OPlog("Track Count: %d", mesh->Animation->AnimationTrackCount);
		if (mesh->Animation->AnimationTrackCount == 1 && animationCount > 0) {

			OPlogDebug("Track Name to split: %s", mesh->Animation->Animations[0].Name);

			for (OPint i = 0; i < animationCount; i++) {
				OPlog("Anim %s", animationNames[i]);
				OPchar* prefix = OPstringCreateMerged(".", animationNames[i]);
				OPchar* animName = OPstringCreateMerged(prefix, ".anim");
				OPchar* anim = OPstringCreateMerged(output, animName);
				ofstream myAnimFile(anim, ios::binary);

				writeI16(&myAnimFile, mesh->Skeleton.BoneCount);

				ui32 len = strlen(animationNames[i]);
				writeU32(&myAnimFile, len);
				write(&myAnimFile, animationNames[i], len);

				OPint startFrame = animationFrames[i * 2];
				OPint endFrame = animationFrames[i * 2 + 1];
				OPint totalFrames = endFrame - startFrame;

				writeU32(&myAnimFile, totalFrames);
				for (OPint j = startFrame * mesh->Skeleton.BoneCount; j < endFrame * mesh->Skeleton.BoneCount; j++) {
					//OPlg("%d ", j);
					//OPmat4Log("Frame", mesh->Animation->Animations[0].JointTransform[j]);
					for (i32 c = 0; c < 4; c++) {
						writeF32(&myAnimFile, mesh->Animation->Animations[0].JointTransform[j][c].x);
						writeF32(&myAnimFile, mesh->Animation->Animations[0].JointTransform[j][c].y);
						writeF32(&myAnimFile, mesh->Animation->Animations[0].JointTransform[j][c].z);
						writeF32(&myAnimFile, mesh->Animation->Animations[0].JointTransform[j][c].w);
					}
				}
			}
		} else {
			//writeI16(&myAnimFile, mesh->Animation->AnimationTrackCount);
			for (OPint i = 0; i < mesh->Animation->AnimationTrackCount; i++) {
				OPlogDebug("Track Name: %s", mesh->Animation->Animations[i].Name);
				OPchar* prefix = OPstringCreateMerged(".", mesh->Animation->Animations[i].Name);
				OPchar* animName = OPstringCreateMerged(prefix, ".anim");
				OPchar* anim = OPstringCreateMerged(output, animName);
				ofstream myAnimFile(anim, ios::binary);

				writeI16(&myAnimFile, mesh->Skeleton.BoneCount);

				ui32 len = strlen(mesh->Animation->Animations[i].Name);
				writeU32(&myAnimFile, len);
				write(&myAnimFile, mesh->Animation->Animations[i].Name, len);

				writeU32(&myAnimFile, mesh->Animation->Animations[i].TotalFrames);
				for (OPint j = 0; j < mesh->Animation->Animations[i].TotalFrames * mesh->Skeleton.BoneCount; j++) {
					//OPlg("%d ", j);
					//OPmat4Log("Frame", mesh->Animation->Animations[i].JointTransform[j]);
					for (i32 c = 0; c < 4; c++) {
						writeF32(&myAnimFile, mesh->Animation->Animations[i].JointTransform[j][c].x);
						writeF32(&myAnimFile, mesh->Animation->Animations[i].JointTransform[j][c].y);
						writeF32(&myAnimFile, mesh->Animation->Animations[i].JointTransform[j][c].z);
						writeF32(&myAnimFile, mesh->Animation->Animations[i].JointTransform[j][c].w);
					}
				}

			}
		}


	}

	OPlog("Finished writing files");

	return 0;
}


