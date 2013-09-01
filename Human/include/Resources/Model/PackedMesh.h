#pragma once
#include "./Human/include/Resources/Model/MeshPacker.h"
#include "./Human/include/Resources/Model/BaseMesh.h"

class PackedMesh : public BaseMesh {
public:
	PackedMesh(
		MeshPacker* meshPacker, 
		ui32 vertexPosition, 
		ui32 indexPosition, 
		ui32 count, 
		ui32 stride);

	MeshPacker* MeshPackerPtr;
	ui32 VertexPosition;
	ui32 IndexPosition;
};

typedef PackedMesh* PackedMeshPtr; 