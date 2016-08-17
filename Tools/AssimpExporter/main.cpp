#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Helpers.h"
#include "./Data/include/OPstring.h"

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

int main(int argc, char **argv) {
	int arg;
	i8* filename = NULL;
	i8* output = NULL;	
	OPint specified = 0;
	OPint featureIn[MAX_FEATURES] = { 0, 0, 0, 0, 0, 0 ,0 ,0 ,0, 0 ,0, 0 };
	OPfloat scale = 0.01; // cm to meters

	OPLOGLEVEL = (ui32)OPlogLevel::VERBOSE;

	OPint animationCount = 0;
	OPchar* animations[1024];
	OPint animationFrames[2048];

	if (argc <= 1) {
		print_help();
	}

	if (argc == 2) {
		filename = argv[1];
	}

	//
	// Fill in provided arguments
	//
	if (argc > 1) {
		for (arg = 1; arg < argc; ++arg)
		{
			OPlog("Arg: %s", argv[arg]);
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

			if (IsParam(argv, arg, "--anim") || IsParam(argv, arg, "-anim"))
			{
				animations[animationCount] = argv[++arg];
				animationFrames[animationCount * 2] = atoi(argv[++arg]);
				animationFrames[animationCount * 2 + 1] = atoi(argv[++arg]);
				animationCount++;

				OPlog("Anim %d", animationCount);
				continue;
			}

			if (IsParam(argv, arg, "--out") || IsParam(argv, arg, "-o"))
			{
				++arg;
				output = argv[arg];
				continue;
			}

			if (IsParam(argv, arg, "--positions") || IsParam(argv, arg, "-p")) {
				featureIn[Model_Positions] = specified = 1; continue;
			}

			if (IsParam(argv, arg, "--normals") || IsParam(argv, arg, "-n")) {
				featureIn[Model_Normals] = specified = 1; continue;
			}

			if (IsParam(argv, arg, "--uvs") || IsParam(argv, arg, "-u")) {
				featureIn[Model_UVs] = specified = 1; continue;
			}
			
			if (IsParam(argv, arg, "--tangents") || IsParam(argv, arg, "-t")) {
				featureIn[Model_Tangents] = specified = 1; continue;
			}

			if (IsParam(argv, arg, "--bitangents") || IsParam(argv, arg, "-bi")) {
				featureIn[Model_Bitangents] = specified = 1; continue;
			}

			if (IsParam(argv, arg, "--indices") || IsParam(argv, arg, "-i")) {
				featureIn[Model_Indices] = specified = 1; continue;
			}

			if (IsParam(argv, arg, "--bones") || IsParam(argv, arg, "-b")) {
				featureIn[Model_Bones] = specified = 1; continue;
			}

			if (IsParam(argv, arg, "--skeletons") || IsParam(argv, arg, "-s")) {
				featureIn[Model_Skeletons] = specified = 1; continue;
			}

			if (IsParam(argv, arg, "--skinning") || IsParam(argv, arg, "-skin")) {
				OPlog("Use Skinning");
				featureIn[Model_Skinning] = specified = 1; continue;
			}

			if (IsParam(argv, arg, "--animations") || IsParam(argv, arg, "-a")) {
				OPlog("Use Animations");
				featureIn[Model_Animations] = specified = 1; continue;
			}

			if (IsParam(argv, arg, "--meta") || IsParam(argv, arg, "-m")) {
				OPlog("Use Meta");
				featureIn[Model_Meta] = specified = 1; continue;
			}

			if (IsParam(argv, arg, "--scale") || IsParam(argv, arg, "-scl")) {
				++arg;
				scale = atof(argv[arg]);
				continue;
			}
		}
	}
	specified = 0;

	//
	// Quit if no file was provided
	//
	if (filename == NULL) {
		print_help();
		return -1;
	}

	//
	// Quit if no output file was provided
	//
	if (output == NULL) {
		output = filename;
	}

	const OPchar* pFile = filename;// "test.obj";
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);

	// If the import failed, report it
	if (!scene)
	{
		return false;
	}

	OPstringToLower(output);
	OPint contains = OPstringContains(output, ".opm");
	if (contains > 0) {
		output[contains] = NULL;
	}
	// Now we can access the file's contents
	OPchar* outputFinal = OPstringCreateMerged(output, ".opm");
	ofstream myFile(outputFinal, ios::binary);

	// OPM File Format Version
	writeU16(&myFile, 2);


	ui32 features[MAX_FEATURES];
	OPbzero(features, sizeof(ui32) * MAX_FEATURES);
	features[Model_Positions] = specified ? featureIn[Model_Positions] : 1;
	features[Model_Normals] = specified ? featureIn[Model_Normals] : 1;
	features[Model_UVs] = specified ? featureIn[Model_UVs] : 1;
	features[Model_Tangents] = specified ? featureIn[Model_Tangents] : 1;
	features[Model_Bitangents] = specified ? featureIn[Model_Bitangents] : 1;
	features[Model_Indices] = specified ? featureIn[Model_Indices] : 1;
	features[Model_Bones] = specified ? featureIn[Model_Bones] : 0;
	features[Model_Skinning] = specified ? featureIn[Model_Skinning] : 0;
	features[Model_Skeletons] = specified ? featureIn[Model_Skeletons] : 0;
	features[Model_Animations] = specified ? featureIn[Model_Animations] : 0;
	features[Model_Meta] = specified ? featureIn[Model_Meta] : 0;



	// Vertex Mode
	// 1 == Vertex Stride ( Pos/Norm/Uv )[]
	// 2 == Vertex Arrays ( Pos )[] ( Norm )[] ( Uv )[]
	writeU32(&myFile, scene->mNumMeshes);

	ui32 totalVerticesEntireModel = 0;
	ui32 totalIndicesEntireModel = 0;

	for (ui32 i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		ui32 stride = 0;
		if (features[Model_Positions]) stride += 3 * sizeof(f32);
		if (features[Model_Normals] && mesh->HasNormals()) stride += 3 * sizeof(f32);
		if (features[Model_UVs] && mesh->HasTextureCoords(0)) stride += 3 * sizeof(f32);
		if (features[Model_Tangents] && mesh->HasTangentsAndBitangents()) stride += 3 * sizeof(f32);
		if (features[Model_Bitangents] && mesh->HasTangentsAndBitangents()) stride += 3 * sizeof(f32);
		if (features[Model_Colors]) stride += 3 * sizeof(f32);
		if (features[Model_Bones]) stride += 4 * sizeof(f32);
		if (features[Model_Skinning]) stride += 4 * sizeof(f32);

		for (ui32 j = 0; j < mesh->mNumFaces; j++) {
			aiFace face = mesh->mFaces[j];
			totalVerticesEntireModel += face.mNumIndices * stride;
			if (face.mNumIndices == 3) {
				totalIndicesEntireModel += 3;
			}
			else if (face.mNumIndices == 4) {
				totalIndicesEntireModel += 6;
			}
		}
	}

	writeU32(&myFile, totalVerticesEntireModel);
	writeU32(&myFile, totalIndicesEntireModel);

	ui16 offset = 0;
	for (ui32 i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];

		// Vertex Mode
		writeU16(&myFile, 1);

		ui32 feature = 0;
		if (features[Model_Positions]) feature += 0x01;
		if (features[Model_Normals] && mesh->HasNormals()) feature += 0x02;
		if (features[Model_UVs] && mesh->HasTextureCoords(0)) feature += 0x04;
		if (features[Model_Tangents] && mesh->HasTangentsAndBitangents()) feature += 0x08;
		if (features[Model_Bitangents] && mesh->HasTangentsAndBitangents()) feature += 0x400;
		if (features[Model_Colors]) feature += 0x100;
		if (features[Model_Indices]) feature += 0x10;
		if (features[Model_Bones]) feature += 0x20;
		if (features[Model_Skinning]) feature += 0x40;
		if (features[Model_Animations]) feature += 0x80;
		if (features[Model_Meta]) feature += 0x200;

		writeU32(&myFile, feature);

		ui32 totalVertices = 0;
		ui32 totalIndices = 0;
		for (ui32 j = 0; j < mesh->mNumFaces; j++) {
			aiFace face = mesh->mFaces[j];
			totalVertices += face.mNumIndices;
			if (face.mNumIndices == 3) {
				totalIndices += 3;
			}
			else if (face.mNumIndices == 4) {
				totalIndices += 6;
			}
		}

		writeU32(&myFile, totalVertices);
		writeU32(&myFile, totalIndices);

		//ui16* indData = (ui16*)OPalloc(sizeof(ui16) * totalIndices);
		//ui16 offset = 0;

		for (ui32 j = 0; j < mesh->mNumFaces; j++) {
			aiFace face = mesh->mFaces[j];
			if (face.mNumIndices > 4) {
				OPlogErr("Only Supporting 3 and 4 point faces.");
				continue;
			}
			
			aiVector3D verts[4];
			aiVector3D normals[4];
			aiVector3D uvs[4];
			aiColor4D colors[4];
			aiVector3D bitangents[4];
			aiVector3D tangents[4];

			for (ui32 k = 0; k < face.mNumIndices; k++) {
				verts[k] = mesh->mVertices[face.mIndices[k]];
				if (mesh->HasNormals()) {
					normals[k] = mesh->mNormals[face.mIndices[k]];
				}
				if (mesh->HasTextureCoords(0)) {
					// Only supporting 1 layer of texture coordinates right now
					uvs[k] = mesh->mTextureCoords[0][face.mIndices[k]];
				}
				if (mesh->HasVertexColors(0)) {
					// Only supporting 1 layer of colors right now
					colors[k] = mesh->mColors[0][face.mIndices[k]];
				}
				if (mesh->HasTangentsAndBitangents()) {
					bitangents[k] = mesh->mBitangents[face.mIndices[k]];
					tangents[k] = mesh->mTangents[face.mIndices[k]];
				}
				if (mesh->HasBones()) {
					//mesh->mBones[face.mIndices[k]]->
				}
			}

			// Write each vertex
			for (ui32 k = 0; k < face.mNumIndices; k++) {

				// Position
				writeF32(&myFile, verts[k].x * scale);
				writeF32(&myFile, verts[k].y * scale);
				writeF32(&myFile, verts[k].z * scale);

				// Normal
				if (mesh->HasNormals() && features[Model_Normals]) {
					writeF32(&myFile, normals[k].x);
					writeF32(&myFile, normals[k].y);
					writeF32(&myFile, normals[k].z);
				}

				if (mesh->HasTangentsAndBitangents() && features[Model_Tangents]) {
					writeF32(&myFile, tangents[k].x);
					writeF32(&myFile, tangents[k].y);
					writeF32(&myFile, tangents[k].z);
				}

				if (mesh->HasTangentsAndBitangents() && features[Model_Bitangents]) {
					writeF32(&myFile, bitangents[k].x);
					writeF32(&myFile, bitangents[k].y);
					writeF32(&myFile, bitangents[k].z);
				}

				if (mesh->HasTextureCoords(0) && features[Model_UVs]) {
					writeF32(&myFile, uvs[k].x);
					writeF32(&myFile, uvs[k].y);
				}

				if (mesh->HasBones() && features[Model_Animations]) {
					// TODO
				}

				if (mesh->HasVertexColors(0) && features[Model_Colors]) {
					writeF32(&myFile, colors[k].r);
					writeF32(&myFile, colors[k].g);
					writeF32(&myFile, colors[k].b);
				}
				else if (features[Model_Colors]) {
					writeF32(&myFile, 0);
					writeF32(&myFile, 0);
					writeF32(&myFile, 0);
				}
			}
		}

		for (ui32 j = 0; j < mesh->mNumFaces; j++) {
			aiFace face = mesh->mFaces[j];
			if (face.mNumIndices > 4) {
				OPlogErr("Only Supporting 3 and 4 point faces.");
				continue;
			}

			if (face.mNumIndices == 3) {
				OPlog("Triangle");
				writeU16(&myFile, offset++);
				writeU16(&myFile, offset++);
				writeU16(&myFile, offset++);
			}
			else {
				OPlog("Quad");
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

	if (scene->HasAnimations()) {
		//scene->mAnimations
	}

	if (scene->HasMaterials()) {

	}

	if (scene->HasTextures()) {

	}



	return 0;
}