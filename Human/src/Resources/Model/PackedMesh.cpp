#include "./Human/include/Resources/Model/PackedMesh.h"

PackedMesh::PackedMesh(
	MeshPacker* meshPacker, 
	ui32 vertexPosition, 
	ui32 indexPosition, 
	ui32 count, 
	ui32 stride) : BaseMesh(count, stride){

	MeshPackerPtr = meshPacker;
	VertexPosition = vertexPosition;
	IndexPosition = indexPosition;
}