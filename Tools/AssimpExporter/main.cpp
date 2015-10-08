#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Helpers.h"

int main() {
	const OPchar* pFile = "C:\\Users\\Garrett\\.opengine\\build\\opengine_0_4_7_8_build\\Tools\\AssimpExporter\\Debug\\model.obj";
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	// If the import failed, report it
	if (!scene)
	{
		return false;
	}

	// Now we can access the file's contents
	const OPchar* output = "output.opm";
	ofstream myFile(output, ios::binary);

	// OPM File Format Version
	writeU16(&myFile, 2);




	return 0;
}