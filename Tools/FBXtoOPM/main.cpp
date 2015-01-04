#include "MenuHelper.h"
#include "OPfbxMesh.h"

int main(int argc, char **argv) {
	int arg;
	i8* filename = NULL;
	i8* output = NULL;
	OPint specified = 0;
	OPint animOnly = 0;
	OPint featureIn[9] = { 0, 0, 0, 0, 0,0 ,0 ,0 ,0 };
	
	//
	// Fill in provided arguments
	//
	if (argc > 1) {
		for (arg = 1; arg < argc; ++arg)
		{
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

			if (IsParam(argv, arg, "--out") || IsParam(argv, arg, "-o"))
			{
				++arg;
				output = argv[arg];
				continue;
			}

			if (IsParam(argv, arg, "--positions") || IsParam(argv, arg, "-p"))
				featureIn[Model_Positions] = specified = 1; continue;

			if (IsParam(argv, arg, "--normals") || IsParam(argv, arg, "-n"))
				featureIn[Model_Normals] = specified = 1; continue;

			if (IsParam(argv, arg, "--uvs") || IsParam(argv, arg, "-u"))
				featureIn[Model_UVs] = specified = 1; continue;

			if (IsParam(argv, arg, "--indices") || IsParam(argv, arg, "-i"))
				featureIn[Model_Indices] = specified = 1; continue;

			if (IsParam(argv, arg, "--bones") || IsParam(argv, arg, "-b"))
				featureIn[Model_Bones] = specified = 1; continue;

			if (IsParam(argv, arg, "--animations") || IsParam(argv, arg, "-a"))
				featureIn[Model_Animations] = specified = 1; continue;
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


	//
	// Get Features to Export from the FBX into the OPM
	//
	OPint* features = (OPint*)OPallocZero(sizeof(OPint)* 9);
	features[Model_Positions] = specified ? featureIn[Model_Positions] : 1;
	features[Model_Normals] = specified ? featureIn[Model_Normals] : 1;
	features[Model_UVs] = specified ? featureIn[Model_UVs] : 1;
	features[Model_Indices] = specified ? featureIn[Model_Indices] : 1;
	features[Model_Bones] = specified ? featureIn[Model_Bones] : 1;
	features[Model_Skinning] = specified ? featureIn[Model_Skinning] : 1;
	features[Model_Animations] = specified ? featureIn[Model_Animations] : 1;

	if (!specified) {
		MenuOptions(
			"Select OPM Features to Export\n------------------------",
			features,
			9,
			"POSITION", "NORMAL", "UVS", "COLORS", "INDICES", "TANGENTS", "BONES", "SKIN", "ANIMATIONS"
			);
	}
	
	// Build out the mesh and write it to file
	OPfbxMesh mesh;
	if(OPfbxMeshCreate(&mesh, filename) == 0) {
		OPfbxMeshWriteToFile(&mesh, output, features, animOnly);
	}

	return 0;
}