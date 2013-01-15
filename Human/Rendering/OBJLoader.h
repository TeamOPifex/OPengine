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

class MeshVertex
{
public:
	//union 
	//{
	//	struct{Vector3 vertice; Vector2 tex;};
	//};
	Vector3 vertice;
	Vector2 tex;
	//Vector3 normal;
};

class Mesh
{
public:
	~Mesh(){
		OPfree(points);
		OPfree(indices);
	}
	MeshVertex* points;
	unsigned int* indices;
	int primitiveCount;
	int indicesCount;
	int vertexCount;
};

Mesh* LoadOBJ(FILE* file, int start, int length);
int lineType(char* word);