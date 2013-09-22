#include "./Human/include/Utilities/OPMLoader.h"

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

OPint OPMhasFeature(ui32 features, ui32 feature) {
	return features & feature;
}

void OPMgenerateTangent(OPvec3* tangent, OPMvertex* v1, OPMvertex* v2){
	f32 dx = v1->Position.x - v2->Position.x;
	f32 dy = v1->Position.y - v2->Position.y;
	f32 dz = v1->Position.z - v2->Position.z;

	OPvec3 diff = {dx, dy, dz};
	OPvec3cross(tangent, &v1->Normal, &diff);
	OPvec3norm(tangent, tangent);
}

OPMData OPMloadData(OPstream* str) {
	ui16 version = OPread_ui16(str);
	ui32 features = OPread_ui32(str);
	ui32 verticeCount = OPread_ui32(str);

	OPMvertex* vertices = (OPMvertex*)OPalloc(sizeof(OPMvertex) * verticeCount);

	f32 x, y, z;
	for(ui32 i = 0; i < verticeCount; i++) {

		// Read Position
		if(OPMhasFeature(features, Position)) {
			x = OPread_f32(str);
			y = OPread_f32(str);
			z = OPread_f32(str);
			vertices[i].Position.x = x;
			vertices[i].Position.y = y;
			vertices[i].Position.z = z;
		}

		// Read Normal
		if(OPMhasFeature(features, Normal)) {
			x = OPread_f32(str);
			y = OPread_f32(str);
			z = OPread_f32(str);
			vertices[i].Normal.x = x;
			vertices[i].Normal.y = y;
			vertices[i].Normal.z = z;
		}

		// Read UV
		if(OPMhasFeature(features, UV)) {
			x = OPread_f32(str);
			y = OPread_f32(str);
			vertices[i].TexCoord.x = x;
			vertices[i].TexCoord.y = y;
		}

		// Read Tangent
		if(OPMhasFeature(features, Tangent)) {
			x = OPread_f32(str);
			y = OPread_f32(str);
			z = OPread_f32(str);
			vertices[i].Tangent.x = x;
			vertices[i].Tangent.y = y;
			vertices[i].Tangent.z = z;
		}
	}

	ui32 indicesCount = OPread_ui32(str);
	ui16* indices = (ui16*)OPalloc(sizeof(str) * (indicesCount * 3));
	for(int i = 0; i < indicesCount; i++){
		indices[i * 3 + 0] = OPread_ui16(str);
		indices[i * 3 + 1] = OPread_ui16(str);
		indices[i * 3 + 2] = OPread_ui16(str);
	}

	// If there were no tangents provided, build them
	if(!OPMhasFeature(features, Tangent)) {
		OPMvertex* vert_one;
		OPMvertex* vert_two;
		OPvec3* tangent;
		for(ui32 i = 0; i < indicesCount; i+=3){
			vert_one = &vertices[indices[i] + 0];
			vert_two = &vertices[indices[i] + 1];
			OPMgenerateTangent(&vert_one->Tangent, vert_one, vert_two);

			vert_one = &vertices[indices[i] + 1];
			vert_two = &vertices[indices[i] + 2];
			OPMgenerateTangent(&vert_one->Tangent, vert_one, vert_two);

			vert_one = &vertices[indices[i] + 2];
			vert_two = &vertices[indices[i] + 0];
			OPMgenerateTangent(&vert_one->Tangent, vert_one, vert_two);
		}
	}

	OPMData data;
	data.indices = indices;
	data.indexCount = indicesCount * 3;
	data.indexSize = sizeof(ui16);
	data.vertices = vertices;
	data.vertexCount = verticeCount;
	data.vertexSize = sizeof(OPMvertex);
	return data;
}

OPint OPMload(const OPchar* filename, OPmesh** mesh) {
	OPstream* str = OPreadFile(filename);
	OPMData data = OPMloadData(str);

	// Create Vertex & Index Buffers for Mesh
	OPmesh temp = OPrenderCreateMesh();
	OPrenderBindMesh(&temp);
	OPrenderBuildMesh(
		data.vertexSize, data.indexSize,
		data.vertexCount, data.indexCount,
		data.vertices, data.indices
	);

	// Dispose of allocated buffers
	OPfree(data.vertices);
	OPfree(data.indices);
	OPstreamDestroy(str);

	*mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
	OPmemcpy(*mesh, &temp, sizeof(OPmesh));

	return 1;
}


OPint OPMloadPacked(const OPchar* filename, OPmeshPacked** mesh) {
	OPstream* str = OPreadFile(filename);
	OPMData data = OPMloadData(str);

	OPmeshPacked temp = OPrenderCreateMeshPacked(
		data.vertexSize, data.indexSize,
		data.vertexCount, data.indexCount,
		data.vertices, data.indices
	);
	
	// Dispose of allocated buffers
	OPfree(data.vertices);
	OPfree(data.indices);
	OPstreamDestroy(str);

	*mesh = (OPmeshPacked*)OPalloc(sizeof(OPmeshPacked));
	OPmemcpy(*mesh, &temp, sizeof(OPmeshPacked));

	return 1;
}

OPint OPMUnload(void* image){
	return 1;
}