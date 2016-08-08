#include "MenuHelper.h"
#include "OPfbxMesh.h"
#include "./OPengine.h"

int main(int argc, char **argv) {
	OP_LOG_LEVEL = 2000;
	int arg;
	i8* filename = NULL;
	i8* output = NULL;
	OPint specified = 0;
	OPint featureIn[9] = { 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 };

	OPint animationCount = 0;
	OPchar* animations[1024];
	OPint animationFrames[2048];

	OPfloat scale = 1.0;

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

			if (IsParam(argv, arg, "--indices") || IsParam(argv, arg, "-i")) {
				featureIn[Model_Indices] = specified = 1; continue;
			}

			if (IsParam(argv, arg, "--tangents") || IsParam(argv, arg, "-t")) {
				featureIn[Model_Tangents] = specified = 1; continue;
			}

			if (IsParam(argv, arg, "--bitangents") || IsParam(argv, arg, "-bi")) {
				featureIn[Model_BiTangents] = specified = 1; continue;
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
		print_help();
		return -1;
	}

	OPlog("Begin processing");

	//
	// Get Features to Export from the FBX into the OPM
	//
	OPint features[MAX_FEATURES];
	OPbzero(&features, sizeof(OPint) * MAX_FEATURES);
	features[Model_Positions] = specified ? featureIn[Model_Positions] : 1;
	features[Model_Normals] = specified ? featureIn[Model_Normals] : 1;
	features[Model_UVs] = specified ? featureIn[Model_UVs] : 1;
	features[Model_Indices] = specified ? featureIn[Model_Indices] : 1;
	features[Model_Tangents] = specified ? featureIn[Model_Tangents] : 1;
	features[Model_BiTangents] = specified ? featureIn[Model_BiTangents] : 1;
	features[Model_Bones] = specified ? featureIn[Model_Bones] : 1;
	features[Model_Skinning] = specified ? featureIn[Model_Skinning] : 1;
	features[Model_Skeletons] = specified ? featureIn[Model_Skeletons] : 1;
	features[Model_Animations] = specified ? featureIn[Model_Animations] : 1;
	features[Model_Meta] = specified ? featureIn[Model_Meta] : 1;

	if (!specified) {
		MenuOptions(
			"Select OPM Features to Export\n------------------------",
			features,
			(MAX_FEATURES),
			"POSITION", "NORMAL", "UVS", "COLORS", "INDICES", "TANGENTS", "BONES", "SKIN", "ANIMATIONS", "SKELETONS", "META"
			);
	}

	// Build out the mesh and write it to file
	OPfbxMesh mesh;
	OPstringToLower(output);
	OPint contains = OPstringContains(output, ".opm");
	if(contains > 0) {
		output[contains] = NULL;
	}

	if(OPfbxMeshCreate(&mesh, scale, filename) == 0) {
		OPfbxMeshWriteToFile(&mesh, output, features, animationCount, animations, animationFrames);
	}

	return 0;
}
