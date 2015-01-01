#include "MenuHelper.h"
#include "OPfbxMesh.h"

int main(int argc, char **argv) {
	int arg;
	i8* filename = NULL;
	i8* output = NULL;
	
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
	features[Model_Positions] = 1;
	features[Model_Normals] = 1;
	features[Model_UVs] = 1;
	features[Model_Indices] = 1;
	features[Model_Bones] = 1;
	features[Model_Skinning] = 1;

	OPint interactive = 0;
	if(interactive) {
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
		OPfbxMeshWriteToFile(&mesh, output, features);
	}

	return 0;
}