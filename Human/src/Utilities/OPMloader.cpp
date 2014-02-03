#include "./Human/include/Utilities/OPMloader.h"

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


void OPCalculateTangents(OPMData* data) {
	for (ui16 a = 0; a < data->indexCount; a+=3) {		
		ui16 i1 = ((ui16*)data->indices)[a + 0];
        ui16 i2 = ((ui16*)data->indices)[a + 1];
        ui16 i3 = ((ui16*)data->indices)[a + 2];

		OPMvertex& v1 = ((OPMvertex*)data->vertices)[i1];
        OPMvertex& v2 = ((OPMvertex*)data->vertices)[i2];
        OPMvertex& v3 = ((OPMvertex*)data->vertices)[i3];
		
		v1.Tangent.x = 0;
		v1.Tangent.y = 0;
		v1.Tangent.z = 0;
		v2.Tangent.x = 0;
		v2.Tangent.y = 0;
		v2.Tangent.z = 0;
		v3.Tangent.x = 0;
		v3.Tangent.y = 0;
		v3.Tangent.z = 0;
	}

	for (ui16 a = 0; a < data->indexCount; a+=3) {	
		ui16 i1 = ((ui16*)data->indices)[a + 0];
        ui16 i2 = ((ui16*)data->indices)[a + 1];
        ui16 i3 = ((ui16*)data->indices)[a + 2];
        
		OPMvertex& v1 = ((OPMvertex*)data->vertices)[i1];
        OPMvertex& v2 = ((OPMvertex*)data->vertices)[i2];
        OPMvertex& v3 = ((OPMvertex*)data->vertices)[i3];

		OPvec3 v2v1;
		OPvec3sub(&v2v1, &v2.Position, &v1.Position);
		OPvec3 v3v1;
		OPvec3sub(&v3v1, &v3.Position, &v1.Position);

		float c2c1t = v2.TexCoord.x - v1.TexCoord.x;
		float c2c1b = v2.TexCoord.y - v1.TexCoord.y;

		float c3c1t = v3.TexCoord.x - v1.TexCoord.x;
		float c3c1b = v3.TexCoord.y - v1.TexCoord.y;

		OPvec3 vecNormal = v1.Normal;

		OPvec3 vecTangent;
		vecTangent.x = c3c1b * v2v1.x - c2c1b * v3v1.x;
		vecTangent.y = c3c1b * v2v1.y - c2c1b * v3v1.y;
		vecTangent.z = c3c1b * v2v1.z - c2c1b * v3v1.z;

		OPvec3 vecSmoothBitangent;
		OPvec3cross(&vecSmoothBitangent, &vecNormal, &vecTangent);
		OPvec3norm(&vecSmoothBitangent, &vecSmoothBitangent);

		OPvec3 vecSmoothTangent;
		OPvec3cross(&vecSmoothTangent, &vecSmoothBitangent, &vecNormal);
		OPvec3norm(&vecSmoothTangent, &vecSmoothTangent);
		
		OPvec3add(&v1.Tangent, &v1.Tangent, &vecSmoothTangent);
		OPvec3add(&v2.Tangent, &v2.Tangent, &vecSmoothTangent);
		OPvec3add(&v3.Tangent, &v3.Tangent, &vecSmoothTangent);
		OPvec3norm(&v1.Tangent, &v1.Tangent);
		OPvec3norm(&v2.Tangent, &v2.Tangent);
		OPvec3norm(&v3.Tangent, &v3.Tangent);
	}
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
			OPvec3norm(&vertices[i].Normal, &vertices[i].Normal);
		}
		
		// Read Tangent
		if(OPMhasFeature(features, Tangent)) {
			x = OPread_f32(str);
			y = OPread_f32(str);
			z = OPread_f32(str);
			vertices[i].Tangent.x = x;
			vertices[i].Tangent.y = y;
			vertices[i].Tangent.z = z;
			OPvec3norm(&vertices[i].Tangent, &vertices[i].Tangent);
		}

		// Read UV
		if(OPMhasFeature(features, UV)) {
			x = OPread_f32(str);
			y = OPread_f32(str);
			vertices[i].TexCoord.x = x;
			vertices[i].TexCoord.y = y;
		}
	}

	ui32 indicesCount = OPread_ui32(str);
	ui16* indices = (ui16*)OPalloc(sizeof(str) * (indicesCount * 3));
	for(int i = 0; i < indicesCount; i++){
		indices[i * 3 + 0] = OPread_ui16(str);
		indices[i * 3 + 1] = OPread_ui16(str);
		indices[i * 3 + 2] = OPread_ui16(str);
	}


	OPMData data;
	data.indices = indices;
	data.indexCount = indicesCount * 3;
	data.indexSize = sizeof(ui16);
	data.vertices = vertices;
	data.vertexCount = verticeCount;
	data.vertexSize = sizeof(OPMvertex);
	
	// If there were no tangents provided, build them
	if(!OPMhasFeature(features, Tangent)) {
		OPCalculateTangents(&data);
	}

	return data;
}

OPint OPMload(const OPchar* filename, OPmesh** mesh) {
	OPLog("Reading File Data");
	OPstream* str = OPreadFile(filename);
	OPLog("Reading OPMloadData");
	OPMData data = OPMloadData(str);

	OPLog("Creating vertex and buffers");
	// Create Vertex & Index Buffers for Mesh
	OPmesh temp = OPrenderCreateMesh();
	OPrenderBindMesh(&temp);
	OPrenderBuildMesh(
		data.vertexSize, data.indexSize,
		data.vertexCount, data.indexCount,
		data.vertices, data.indices
	);

	OPLog("Disposing");

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

OPint OPMUnload(void* mesh){
	OPmesh* m = (OPmesh*)mesh;

	OPrenderDelBuffer(&m->IndexBuffer);
	OPrenderDelBuffer(&m->VertexBuffer);
	OPfree(m);

	return 1;
}