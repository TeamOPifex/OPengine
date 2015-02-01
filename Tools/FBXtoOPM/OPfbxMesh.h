#pragma once

#include "./OPengine.h"
#include "OPfbxSdk.h"
#include "OPfbxSkeleton.h"
#include "OPfbxMeshData.h"
#include "OPfbxSkin.h"
#include "OPfbxAnimation.h"
#include "Helpers.h"

typedef struct {
	OPfbxSdk SDK;
	OPfbxScene SCENE;

	OPfbxSkeleton Skeleton;
	OPfbxMeshData MeshData;

	OPfbxMeshPoly* Polys;
	OPfbxSkinBlendWeight* BlendWeights;
	OPfbxAnimation* Animation;

} OPfbxMesh;

OPint OPfbxMeshCreate(OPfbxMesh* mesh, const OPchar* filename) {

	mesh->SDK = OPfbxSdkCreate();
	mesh->SCENE;
	if(OPfbxSdkLoadScene(&mesh->SCENE, &mesh->SDK, filename) == 0) {

		OPlogDebug("Step: FBX Scene Loaded");

		OPfbxSkeletonGet(&mesh->Skeleton, &mesh->SCENE);

		OPlogDebug("Step: Skeleton");

		OPfbxMeshDataGet(&mesh->MeshData, &mesh->SCENE);

		OPlogDebug("Step: MeshData");

		mesh->Polys = OPfbxMeshDataGetPolygons(&mesh->MeshData);

		mesh->BlendWeights = _skinBlendWeights(&mesh->MeshData, &mesh->Skeleton, &mesh->SCENE);
		OPlogDebug("Step: skin");

		mesh->Animation = OPfbxAnimationGet(&mesh->MeshData, &mesh->Skeleton, &mesh->SCENE);

		OPlogDebug("End FBX SDK");
		//OPfbxSdkDestroy(&SDK);

		return 0; // Success
	}

	OPfbxSdkDestroy(&mesh->SDK);

	OPlogDebug("Failed to create the mesh from the given input file.");
	return 1;
}

OPint OPfbxMeshWriteToFile(OPfbxMesh* mesh, const OPchar* filename, OPint* features, OPint animationCount, OPchar** animationNames, OPint* animationFrames) {
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
	if (features[Model_Bones]) feature += 0x20;
	if (features[Model_Skinning]) feature += 0x40;
	if (features[Model_Animations]) feature += 0x80;
	if (features[Model_Meta]) feature += 0x200;


	OPlogDebug("Feature: %d", feature);
	// OPM File Features
	writeU32(&myFile, feature);

	OPint verticeCount = 0;
	OPint indCount = 0;
	for (OPint i = 0; i < mesh->MeshData.PolyCount; i++) {
		verticeCount += mesh->Polys[i].Size;
		indCount += mesh->Polys[i].Size == 3 ? 3 : 6;
	}

	OPlogDebug("Vertex Count: %d", verticeCount);
	// Vertex Count
	writeU32(&myFile, verticeCount);

	for (OPint i = 0; i < mesh->MeshData.PolyCount; i++) {
		for (OPint j = 0; j < mesh->Polys[i].Size; j++) {

			if (features[Model_Positions]) {
				//OPlg(".");
				OPvec3 pos = mesh->Polys[i].Position[j];
				writeF32(&myFile, pos.x);
				writeF32(&myFile, pos.y);
				writeF32(&myFile, pos.z);
			}

			if (features[Model_Normals]) {
				//OPlg("-");
				OPvec3 norm = mesh->Polys[i].Normal[j];
				writeF32(&myFile, norm.x);
				writeF32(&myFile, norm.y);
				writeF32(&myFile, norm.z);
				OPvec3Log("Norm", norm);
			}

			if (features[Model_Tangents]) {
				//OPlg("%");
				OPvec4 tan = mesh->Polys[i].Tangent[j];
				writeF32(&myFile, tan.x);
				writeF32(&myFile, tan.y);
				writeF32(&myFile, tan.z);
			}

			if (features[Model_UVs]) {
				//OPlg("=");
				OPvec2 uv = mesh->Polys[i].UV[j];
				writeF32(&myFile, uv.x);
				writeF32(&myFile, uv.y);
			}

			if (features[Model_Skinning]) {
				//OPlg("+");
				OPint cp = mesh->Polys[i].ControlPointIndex[j];
				OPfbxSkinBlendWeight bw = mesh->BlendWeights[cp];
				OPint boneIndex;

				if (bw.c > 0) {
					boneIndex = OPfbxSkeletonGetPos(&mesh->Skeleton, mesh->BlendWeights[cp].b1);
					writeI16(&myFile, boneIndex);
				}
				if (bw.c > 1) {
					boneIndex = OPfbxSkeletonGetPos(&mesh->Skeleton, mesh->BlendWeights[cp].b2);
					writeI16(&myFile, boneIndex);
				}
				if (bw.c > 2) {
					boneIndex = OPfbxSkeletonGetPos(&mesh->Skeleton, mesh->BlendWeights[cp].b3);
					writeI16(&myFile, boneIndex);
				}
				if (bw.c > 3) {
					boneIndex = OPfbxSkeletonGetPos(&mesh->Skeleton, mesh->BlendWeights[cp].b4);
					writeI16(&myFile, boneIndex);
				}

				for (OPint k = 0; k < 4 - bw.c; k++) {
					writeI16(&myFile, -1);
				}


				if (bw.c > 0) {
					writeF32(&myFile, mesh->BlendWeights[cp].w1);
				}
				if (bw.c > 1) {
					writeF32(&myFile, mesh->BlendWeights[cp].w2);
				}
				if (bw.c > 2) {
					writeF32(&myFile, mesh->BlendWeights[cp].w3);
				}
				if (bw.c > 3) {
					writeF32(&myFile, mesh->BlendWeights[cp].w4);
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

		for (OPint i = 0; i < mesh->MeshData.PolyCount; i++) {
			if (mesh->Polys[i].Size == 3) {
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
		writeU16(&myFile, mesh->MeshData.MetaCount);
		for(OPuint i = 0; i < mesh->MeshData.MetaCount; i++) {

			// Name
			ui32 len = strlen(mesh->MeshData.Meta[i].Name);
			writeU32(&myFile, len);
			write(&myFile, mesh->MeshData.Meta[i].Name, len);

			// Type
			len = strlen(mesh->MeshData.Meta[i].Type);
			writeU32(&myFile, len);
			write(&myFile, mesh->MeshData.Meta[i].Type, len);

			writeF32(&myFile, mesh->MeshData.Meta[i].Position[0]);
			writeF32(&myFile, mesh->MeshData.Meta[i].Position[1]);
			writeF32(&myFile, mesh->MeshData.Meta[i].Position[2]);

			writeF32(&myFile, mesh->MeshData.Meta[i].Rotation[0]);
			writeF32(&myFile, mesh->MeshData.Meta[i].Rotation[1]);
			writeF32(&myFile, mesh->MeshData.Meta[i].Rotation[2]);

			writeF32(&myFile, mesh->MeshData.Meta[i].Scale[0]);
			writeF32(&myFile, mesh->MeshData.Meta[i].Scale[1]);
			writeF32(&myFile, mesh->MeshData.Meta[i].Scale[2]);
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


