#pragma once

#include "./Human/include/Math/Vector2.h"
#include "./Human/include/Math/Vector3.h"

class MeshVertex
{
public:
	Vector3 vertice;
	Vector3 normal;
	Vector2 tex;
	Vector3 tangent;
};