#include "./Human/include/Utilities/OPMLoader.h"
#include "./Core/include/DynamicMemory.h"
#include "./Core/include/Log.h"
#include "./Data/include/OPfile.h"
#include "./Human/include/Rendering/OPMvertex.h"

namespace Features{
enum OPMFeatures {
	Position	=	0x01,
	Normal		=	0x02,
	UV			=	0x04,
	Tangent		=	0x08,
	Index		=	0x10,
	Bones		=	0x20,
	Skinning	=	0x40,
	Animations	=	0x80
};
}

bool _has(ui32 features, ui32 feature) {
	return features & feature;
}

void _generateTangent(OPvec3* tangent, OPMvertex* v1, OPMvertex* v2){
	f32 dx = v1->vertice.x - v2->vertice.x;
	f32 dy = v1->vertice.y - v2->vertice.y;
	f32 dz = v1->vertice.z - v2->vertice.z;

	OPvec3 diff;
	diff.x = dx;
	diff.y = dy;
	diff.z = dz;
	OPvec3 tan;
	OPvec3cross(tan, v1->normal, diff);
	OPvec3norm(tan, tan);

	tangent->x = tan.x;
	tangent->y = tan.y;
	tangent->z = tan.z;
}

typedef struct {
	void* vertices;
	ui32 vertexCount;
	ui32 vertexSize;
	void* indices;
	ui32 indexCount;
	ui32 indexSize;

} LoadedData;

LoadedData LoadOPMData(FILE* file) {
	
	ui16 version = OPread_ui16(file);
	ui32 features = OPread_ui32(file);
	ui32 verticeCount = OPread_ui32(file);

	OPMvertex* vertices = (OPMvertex*)OPalloc(sizeof(OPMvertex) * verticeCount);

	f32 x, y, z;
	for(ui32 i = 0; i < verticeCount; i++) {

		// Read Position
		if(_has(features, Features::Position)) {
			x = OPread_f32(file);
			y = OPread_f32(file);
			z = OPread_f32(file);
			vertices[i].vertice.x = x;
			vertices[i].vertice.y = y;
			vertices[i].vertice.z = z;
		}

		// Read Normal
		if(_has(features, Features::Normal)) {
			x = OPread_f32(file);
			y = OPread_f32(file);
			z = OPread_f32(file);
			vertices[i].normal.x = x;
			vertices[i].normal.y = y;
			vertices[i].normal.z = z;
		}

		// Read UV
		if(_has(features, Features::UV)) {
			x = OPread_f32(file);
			y = OPread_f32(file);
			vertices[i].tex.x = x;
			vertices[i].tex.y = y;
		}

		// Read Tangent
		if(_has(features, Features::Tangent)) {
			x = OPread_f32(file);
			y = OPread_f32(file);
			z = OPread_f32(file);
			vertices[i].tangent.x = x;
			vertices[i].tangent.y = y;
			vertices[i].tangent.z = z;
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
		OPMvertex* vert_one;
		OPMvertex* vert_two;
		OPvec3* tangent;
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

	LoadedData data;
	data.indices = indices;
	data.indexCount = indicesCount * 3;
	data.indexSize = sizeof(ui16);
	data.vertices = vertices;
	data.vertexCount = verticeCount;
	data.vertexSize = sizeof(OPMvertex);
	return data;
}

Mesh* LoadOPM(const OPchar* filename) {
	// TODO
	// I thoroughly dissaprove of using OPreadFileInformation,
	// retrieving a file pointer then reading from that... We need
	// more consistancy. We should change this so a OPstream, thus using only
	// what we've built previously. That way it's limited to a single point
	// of failure and is more maintainable.
	FILE* file = OPreadFileInformation(filename).file;  
	LoadedData data = LoadOPMData(file);

	// Create Vertex & Index Buffers for Mesh
	BufferPtr vertexBuffer = new Buffer(VertexBuffer, data.vertexSize * data.vertexCount, data.vertices);
	BufferPtr indexBuffer = new Buffer(IndexBuffer, data.indexSize * data.indexCount * 3, data.indices);

	Mesh* out = new Mesh(vertexBuffer, indexBuffer,  data.indexCount * 3, data.vertexSize);

	// Dispose of allocated buffers
	OPfree(data.vertices);
	OPfree(data.indices);

	fclose(file);

	return out;
}


PackedMesh* LoadOPM(const OPchar* filename, MeshPacker* meshPacker) {
	FILE* file = OPreadFileInformation(filename).file;  
	LoadedData data = LoadOPMData(file);

	ui32 vertexPosition = meshPacker->AddVertexBuffer(data.vertexSize, data.vertices, data.vertexCount);
	ui32 indexPosition = meshPacker->AddIndexBuffer(data.indexSize, data.indices, data.indexCount);
	
	fclose(file);
	
	return new PackedMesh(meshPacker, vertexPosition, indexPosition, data.indexCount * 3, data.vertexSize);
}

OPint OPMLoad(const OPchar* filename, Mesh** image){
	return 1;
}

OPint OPMUnload(void* image){
	return 1;
}