#include "./Human/include/Utilities/OPMLoader.h"
#include "./Core/include/DynamicMemory.h"
#include "./Core/include/Log.h"
#include "./Data/include/OPfile.h"
#include "./Human/include/Rendering/MeshVertex.h"

enum Features {
	Position	=	0x01,
	Normal		=	0x02,
	UV			=	0x04,
	Tangent		=	0x08,
	Index		=	0x10,
	Bones		=	0x20,
	Skinning	=	0x40,
	Animations	=	0x80
};

bool _has(ui32 features, ui32 feature) {
	return features & feature;
}

void _generateTangent(Vector3* tangent, MeshVertex* v1, MeshVertex* v2){
	f32 dx = v1->vertice._x - v2->vertice._x;
	f32 dy = v1->vertice._y - v2->vertice._y;
	f32 dz = v1->vertice._z - v2->vertice._z;

	Vector3 diff(dx, dy, dz);
	Vector3 tan = Vector3::Cross(v1->normal, diff);
	tan.Normalize();
	tangent->_x = tan._x;
	tangent->_y = tan._y;
	tangent->_z = tan._z;
}

Mesh* LoadOPM(FILE* file) {

	char* buffer = (char*)OPalloc(sizeof(char) * 4096);
	
	ui16 version = OPread_ui16(file);
	ui32 features = OPread_ui32(file);
	ui32 verticeCount = OPread_ui32(file);
	
	MeshVertex* vertices = (MeshVertex*)OPalloc(sizeof(MeshVertex) * verticeCount);

	f32 x, y, z;
	for(ui32 i = 0; i < verticeCount; i++) {
		
		// Read Position
		if(_has(features, Features::Position)) {
			x = OPread_f32(file);
			y = OPread_f32(file);
			z = OPread_f32(file);
			vertices[i].vertice._x = x;
			vertices[i].vertice._y = y;
			vertices[i].vertice._z = z;
		}

		// Read Normal
		if(_has(features, Features::Normal)) {
			x = OPread_f32(file);
			y = OPread_f32(file);
			z = OPread_f32(file);
			vertices[i].normal._x = x;
			vertices[i].normal._y = y;
			vertices[i].normal._z = z;
		}

		// Read UV
		if(_has(features, Features::UV)) {
			x = OPread_f32(file);
			y = OPread_f32(file);
			vertices[i].tex._x = x;
			vertices[i].tex._y = y;
		}

		// Read Tangent
		if(_has(features, Features::Tangent)) {
			x = OPread_f32(file);
			y = OPread_f32(file);
			z = OPread_f32(file);
			vertices[i].tangent._x = x;
			vertices[i].tangent._y = y;
			vertices[i].tangent._z = z;
		}
	}
	
	ui32 indicesCount = OPread_ui32(file);
	ui16* indices = (ui16*)OPalloc(sizeof(ui16) * (indicesCount * 3));
	for(int i = 0; i < indicesCount; i++){
		indices[i * 3 + 0] = OPread_ui16(file);
		indices[i * 3 + 1] = OPread_ui16(file);
		indices[i * 3 + 2] = OPread_ui16(file);
	}

	// If there were no tangents provided, build them
	if(!_has(features, Features::Tangent)) {
		MeshVertex* vert_one;
		MeshVertex* vert_two;
		Vector3* tangent;
		for(ui32 i = 0; i < indicesCount; i+=3){
			vert_one = &vertices[indices[i] + 0];
			vert_two = &vertices[indices[i] + 1];
			_generateTangent(&vert_one->tangent, vert_one, vert_two);
		
			vert_one = &vertices[indices[i] + 1];
			vert_two = &vertices[indices[i] + 2];
			_generateTangent(&vert_one->tangent, vert_one, vert_two);
		
			vert_one = &vertices[indices[i] + 2];
			vert_two = &vertices[indices[i] + 0];
			_generateTangent(&vert_one->tangent, vert_one, vert_two);
		}
	}

	// Create Vertex & Index Buffers for Mesh
	BufferPtr vertexBuffer = new Buffer(VertexBuffer, sizeof(MeshVertex) * verticeCount, vertices);
	BufferPtr indexBuffer = new Buffer(IndexBuffer, sizeof(ui16) * indicesCount * 3, indices);

	Mesh* out = new Mesh(vertexBuffer, indexBuffer, indicesCount * 3, sizeof(MeshVertex));

	// Dispose of allocated buffers
	OPfree(buffer);
	OPfree(vertices);
	OPfree(indices);

	return out;
}