#include "./Human/include/Utilities/OPMLoader.h"
#include "./Core/include/DynamicMemory.h"
#include "./Core/include/Log.h"
#include "./Data/include/OPfile.h"
#include "./Human/include/Rendering/OPMvertex.h"


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

bool OPMhasFeature(ui32 features, ui32 feature) {
	return features & feature;
}

void OPMgenerateTangent(Vector3* tangent, MeshVertex* v1, MeshVertex* v2){
	// f32 dx = v1->vertice.x - v2->vertice.x;
	// f32 dy = v1->vertice.y - v2->vertice.y;
	// f32 dz = v1->vertice.z - v2->vertice.z;

	// Vector3 diff(dx, dy, dz);
	// Vector3 tan = Vector3::Cross(v1->normal, diff);
	// tan.Normalize();
	// tangent->x = tan.x;
	// tangent->y = tan.y;
	// tangent->z = tan.z;
}

typedef struct {
	void* vertices;
	ui32 vertexCount;
	ui32 vertexSize;
	void* indices;
	ui32 indexCount;
	ui32 indexSize;

} OPMData;

OPMData OPMloadData(OPstream* str) {
	
	// ui16 version = OPread(str);
	// ui32 features = OPread_ui32(str);
	// ui32 verticeCount = OPread_ui32(str);

	// MeshVertex* vertices = (MeshVertex*)OPalloc(sizeof(MeshVertex) * verticeCount);

	// f32 x, y, z;
	// for(ui32 i = 0; i < verticeCount; i++) {

	// 	// Read Position
	// 	if(_has(features, Features::Position)) {
	// 		x = OPread_f32(str);
	// 		y = OPread_f32(str);
	// 		z = OPread_f32(str);
	// 		vertices[i].vertice.x = x;
	// 		vertices[i].vertice.y = y;
	// 		vertices[i].vertice.z = z;
	// 	}

	// 	// Read Normal
	// 	if(_has(features, Features::Normal)) {
	// 		x = OPread_f32(str);
	// 		y = OPread_f32(str);
	// 		z = OPread_f32(str);
	// 		vertices[i].normal.x = x;
	// 		vertices[i].normal.y = y;
	// 		vertices[i].normal.z = z;
	// 	}

	// 	// Read UV
	// 	if(_has(features, Features::UV)) {
	// 		x = OPread_f32(str);
	// 		y = OPread_f32(str);
	// 		vertices[i].tex.x = x;
	// 		vertices[i].tex.y = y;
	// 	}

	// 	// Read Tangent
	// 	if(_has(features, Features::Tangent)) {
	// 		x = OPread_f32(str);
	// 		y = OPread_f32(str);
	// 		z = OPread_f32(str);
	// 		vertices[i].tangent.x = x;
	// 		vertices[i].tangent.y = y;
	// 		vertices[i].tangent.z = z;
	// 	}
	// }

	// ui32 indicesCount = OPread_ui32(str);
	// ui16* indices = (ui16*)OPalloc(sizeof(str) * (indicesCount * 3));
	// for(int i = 0; i < indicesCount; i++){
	// 	indices[i * 3 + 0] = OPread_ui16(file);
	// 	indices[i * 3 + 1] = OPread_ui16(file);
	// 	indices[i * 3 + 2] = OPread_ui16(file);
	// }

	// // If there were no tangents provided, build them
	// if(!_has(features, Features::Tangent)) {
	// 	MeshVertex* vert_one;
	// 	MeshVertex* vert_two;
	// 	Vector3* tangent;
	// 	for(ui32 i = 0; i < indicesCount; i+=3){
	// 		vert_one = &vertices[indices[i] + 0];
	// 		vert_two = &vertices[indices[i] + 1];
	// 		_generateTangent(&vert_one->tangent, vert_one, vert_two);

	// 		vert_one = &vertices[indices[i] + 1];
	// 		vert_two = &vertices[indices[i] + 2];
	// 		_generateTangent(&vert_one->tangent, vert_one, vert_two);

	// 		vert_one = &vertices[indices[i] + 2];
	// 		vert_two = &vertices[indices[i] + 0];
	// 		_generateTangent(&vert_one->tangent, vert_one, vert_two);
	// 	}
	// }

	// OPMData data;
	// data.indices = indices;
	// data.indexCount = indicesCount * 3;
	// data.indexSize = sizeof(ui16);
	// data.vertices = vertices;
	// data.vertexCount = verticeCount;
	// data.vertexSize = sizeof(MeshVertex);
	return data;
}

OPint OPMload(const OPchar* filename, OPmesh** mesh) {
	OPsteam str = *OPreadFile(filename);
	OPMData data = OPMloadData(str);

	// Create Vertex & Index Buffers for Mesh
	OPmesh mesh = OPrenderBuildMesh(
		data.vertexSize, data.indexSize * 3,
		data.vertexCount, data.indexCount,
		data.vertices, data.indices
	);

	// Dispose of allocated buffers
	OPfree(data.vertices);
	OPfree(data.indices);

	return 1;
}


OPint OPMloadPacked(const OPchar* filename, OPmeshPacked** mesh) {
	OPsteam str = *OPreadFile(filename);
	OPMData data = OPMloadData(str);

	OPmeshPacked mesh = OPrenderCreateMeshPacked(
		data.vertexSize, data.indexSize * 3,
		data.vertexCount, data.indexCount,
		data.vertices, data.indices
	);
	
	return 1;
}

OPint OPMUnload(void* image){
	return 1;
}