#pragma once

#include <stdio.h>

#include ".\Human\Math\Vector2.h"
#include ".\Human\Math\Vector3.h"

#include ".\Core\include\DynamicMemory.h"
#include "./Human/Resources/Model/Mesh.h"


class MeshVertex
{
public:
	Vector3 vertice;
	Vector2 tex;
	Vector3 normal;
	Vector3 tangent;
};

//class Mesh
//{
//public:
//	~Mesh(){
//		OPfree(points);
//		OPfree(indices);
//	}
//	MeshVertex* points;
//	unsigned int* indices;
//	int primitiveCount;
//	int indicesCount;
//	int vertexCount;
//};

Mesh* LoadOBJ(FILE* file, int start, int length);
Mesh* LoadOBJ(FILE* file, int start, int length, ui8 keepPositions);
int lineType(char* word);