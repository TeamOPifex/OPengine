#pragma once

#include <stdio.h>

#include ".\Human\Math\Vector2.h"
#include ".\Human\Math\Vector3.h"

#include ".\Core\include\DynamicMemory.h"
// FacePoint contains 3 int's being holding points for
// the index's for each array.
struct FacePoint
{
	int VertexIndex;
	int TextureIndex;
	int NormalIndex;
};
// Face contains either 3 or 4 FacePoints, 
// tri is marked as true if it's a triangle
struct Face
{
	FacePoint one;
	FacePoint two;
	FacePoint three;
	FacePoint four;
	bool tri;
};

struct OBJMeshPoint
{
	Vector3 vertice;
	Vector2 tex;
	Vector3 normal;
};

class OBJMesh
{
public:
	~OBJMesh(){
		OPfree(points);
		OPfree(indices);
	}
	OBJMeshPoint* points;
	int* indices;
	int primitiveCount;
	int vertexCount;
	Vector3 position;
};

OBJMesh* LoadOBJ(FILE* file, int start, int length);
int lineType(char* word);