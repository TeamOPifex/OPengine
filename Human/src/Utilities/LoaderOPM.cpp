#include "./Human/include/Utilities/LoaderOPM.h"

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

enum OPMKeyFeatures {
	Key_Time	=	0x01,
	Key_Position=	0x02,
	Key_Rotation=	0x04
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
	ui16 version = OPreadui16(str);
	ui32 features = OPreadui32(str);
	ui32 verticeCount = OPreadui32(str);


	OPMvertex* vertices = (OPMvertex*)OPalloc(sizeof(OPMvertex) * verticeCount);

	f32 x, y, z;
	for(ui32 i = 0; i < verticeCount; i++) {

		// Read Position
		if(OPMhasFeature(features, Position)) {
			x = OPreadf32(str);
			y = OPreadf32(str);
			z = OPreadf32(str);
			vertices[i].Position.x = x;
			vertices[i].Position.y = y;
			vertices[i].Position.z = z;
		}

		// Read Normal
		if(OPMhasFeature(features, Normal)) {
			x = OPreadf32(str);
			y = OPreadf32(str);
			z = OPreadf32(str);
			vertices[i].Normal.x = x;
			vertices[i].Normal.y = y;
			vertices[i].Normal.z = z;
			OPvec3norm(&vertices[i].Normal, &vertices[i].Normal);
		}
		
		// Read Tangent
		if(OPMhasFeature(features, Tangent)) {
			x = OPreadf32(str);
			y = OPreadf32(str);
			z = OPreadf32(str);
			vertices[i].Tangent.x = x;
			vertices[i].Tangent.y = y;
			vertices[i].Tangent.z = z;
			OPvec3norm(&vertices[i].Tangent, &vertices[i].Tangent);
		}

		// Read UV
		if(OPMhasFeature(features, UV)) {
			x = OPreadf32(str);
			y = OPreadf32(str);
			vertices[i].TexCoord.x = x;
			vertices[i].TexCoord.y = y;
		}
	}

	ui32 indicesCount = OPreadui32(str);
	ui16* indices = (ui16*)OPalloc(sizeof(str) * (indicesCount * 3));
	for(OPuint i = 0; i < indicesCount; i++){
		indices[i * 3 + 0] = OPreadui16(str);
		indices[i * 3 + 1] = OPreadui16(str);
		indices[i * 3 + 2] = OPreadui16(str);
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

	// Read Bones
	if(OPMhasFeature(features, Bones)){
		ui32 boneCount = OPreadui32(str);
		// OPskeleton* skeleton = (OPskeleton*)OPalloc(sizeof(OPskeleton));
		// skeleton->hierarchy = (ui16*)OPalloc(sizeof(ui16) * boneCount);
		// skeleton->localPoses = (OPmat4*)OPalloc(sizeof(OPmat4) * boneCount);
		// skeleton->globalPoses = (OPmat4*)OPalloc(sizeof(OPmat4) * boneCount);

		for(i32 i = 0; i < boneCount; i++) {
			i32 boneIndex = OPreadi32(str);
			i8* name = OPreadstring(str);
			f32 x = OPreadf32(str);
			f32 y = OPreadf32(str);
			f32 z = OPreadf32(str);
			OPlog("Joint: %d %s", boneIndex, name);
			OPfree(name);
			//OPmat4Translate(&skeleton->localPoses[i], x, y, z);			
		}
	}

	// Read Skinning
	if(OPMhasFeature(features, Skinning)){
		ui32 indexCount = OPreadui32(str);
		ui32* indices = (ui32*)OPalloc(sizeof(ui32)* indexCount);
		for(i32 i = 0; i < indexCount; i++){
			indices[i] = OPreadui32(str);
		}
		ui32 weightCount = OPreadui32(str);
		f32* weights = (f32*)OPalloc(sizeof(f32)* weightCount);
		for(i32 i = 0; i < weightCount; i++) {
			weights[i] = OPreadf32(str);
		}
		for (i32 i = 0; i < indexCount; i++) {
			i32 index = indices[i];
			f32 weight = weights[i];
		}
	}

	// Read Animation
	if(OPMhasFeature(features, Animations)){
		i8* name = OPreadstring(str);
		OPlog("Animation: %s", name);
		ui32 keyframes = OPreadui32(str);
		OPlog("Keyframes %d", keyframes);
		for(i32 i = 0; i < keyframes; i++) {
			OPlog("Keyframe %d", i);
			i32 index = OPreadi32(str);
			OPlog("Bone %d", index);
			ui32 keys = OPreadui32(str);
			for(i32 j = 0; j < keys; j++) {
				ui32 keyFeatures = OPreadui32(str);
				OPlog("Keyframe Features: %d", keyFeatures);
				if(keyFeatures && Key_Time) {
					f32 time = OPreadf32(str);
					OPlog("Keyframe Time: %f", time);
				}

				if(keyFeatures && Key_Position) {
					f32 x = OPreadf32(str);
					f32 y = OPreadf32(str);
					f32 z = OPreadf32(str);
					OPlog("Position: %f, %f, %f", x, y, z);
				}

				if(keyFeatures && Key_Rotation) {
					f32 x = OPreadf32(str);
					f32 y = OPreadf32(str);
					f32 z = OPreadf32(str);
					f32 w = OPreadf32(str);
					OPlog("Rotation: %f, %f, %f, %f", x, y, z, w);
				}
			}
		}
		OPfree(name);
	}



	return data;
}

OPint OPMload(const OPchar* filename, OPmesh** mesh) {
	OPlog("Reading File Data");
	OPstream* str = OPreadFile(filename);
	OPlog("Reading OPMloadData");
	OPMData data = OPMloadData(str);

	OPlog("Creating vertex and buffers");
	// Create Vertex & Index Buffers for Mesh
	OPmesh temp = OPrenderCreateMesh();
	OPrenderBindMesh(&temp);
	OPrenderBuildMesh(
		data.vertexSize, data.indexSize,
		data.vertexCount, data.indexCount,
		data.vertices, data.indices
	);

	OPlog("Disposing");

	// Dispose of allocated buffers
	OPfree(data.vertices);
	OPfree(data.indices);
	OPstreamDestroy(str);

	*mesh = (OPmesh*)OPalloc(sizeof(OPmesh));
	OPmemcpy(*mesh, &temp, sizeof(OPmesh));

	return 1;
}

HashMap* CreateTriangleTable(OPMData* data){
	HashMap* triTable = OPhashMapCreate(data->indexCount / 3);
	OPchar index[10];
	OPint compCount = 4;

	// for each triangle in the mesh
	for(int i = (data->indexCount / 3); i--;){
		int* tri;


		sprintf(index, "%d", i * 3);
		OPhashMapGet(triTable, index, (void**)&tri);

		// if this vertex's tri has been stored, skip it
		if(tri) continue;

		tri = (int*)OPalloc(sizeof(int) * compCount);
		tri[3] = 0;

		// copy all the triangle's indices into an array
		for(int j = 3; j--;){
			tri[j] = i * 3 + j;

			// store the triangle at this vertex's index
			sprintf(index, "%d", i * 3 + j);
			OPhashMapPut(triTable, index, tri);
		}
	}

	return triTable;
}

OPlinkedList* CreateVertexList(OPMData* data){
	OPlinkedList* vertList = OPllCreate();
	for(int i = data->vertexCount; i--;)
		OPllInsertLast(vertList, (void*)i);

	return vertList;
}

void UpdateBasis(OPvec3* axis, OPvec3* basis, OPvec3* position){
	OPfloat pa = OPvec3valDot(axis, position);
	OPfloat ba = OPvec3valDot(axis, basis);

	if(OPabs(pa) > OPabs(ba)){
		*basis = *position;
	}
}

OPvec3 GetCenterOfMass(OPMData* data, OPlinkedList* vertList){
	OPMvertex* vertices = (OPMvertex*)data->vertices;
	OPvec3 com = {0};
	OPllNode* node = vertList->First;
	int verts = 0;

	while(node){
		com += vertices[(OPint)node->Data].Position;
		node = node->Next;
		++verts;
	}
	com /= (OPfloat)verts;

	return com;
}

OPvec3 GetNormal(OPvec3 bX, OPvec3 bY, OPvec3 bZ){
	OPvec3 out = {0};

	OPfloat mX = OPvec3valLen(&bX);
	OPfloat mY = OPvec3valLen(&bY);
	OPfloat mZ = OPvec3valLen(&bZ);

	if(mX > mZ && mY > mZ){
		out = OPvec3valCross(&bX, &bY);
	}
	if(mX > mY && mZ > mY){
		out = OPvec3valCross(&bX, &bZ);
	}
	if(mZ > mX && mY > mX){
		out = OPvec3valCross(&bY, &bZ);
	}

	return out;
}

void ReorderVerts(OPMData* data, OPlinkedList** spaceA, OPlinkedList** spaceB){
	OPMvertex* vertices = (OPMvertex*)data->vertices;
	OPint i = 0, j = 0;
	OPllNode* node = (*spaceA)->First;
	OPlinkedList* tempA = OPllCreate();
	OPlinkedList* tempB = OPllCreate();

	// reorder spaceA
	while(node){
		OPMvertex temp = vertices[i];
		vertices[i] = vertices[j = (OPint)node->Data];
		vertices[j] = temp; // reinsert the displaced vert
		OPllInsertLast(tempA, (void*)i);
		++i;
		node = node->Next;
	}

	// reorder spaceB
	node = (*spaceB)->First;
	while(node){
		OPMvertex temp = vertices[i];
		vertices[i] = vertices[j = (OPint)node->Data];
		vertices[j] = temp; // reinsert the displaced vert
		OPllInsertLast(tempB, (void*)i);
		++i;
		node = node->Next;
	}

	// destroy the old input lists
	OPllDestroy(*spaceA);
	OPllDestroy(*spaceB);

	// set to the reorderd
	*spaceA = tempA;
	*spaceB = tempB;
}

OPlinkedList* CreateTriList(OPMData* data, HashMap* triTable, OPlinkedList* vertList, OPint atLeaf){
	OPlinkedList* triList = OPllCreate();
	OPllNode* node = vertList->First;
	OPchar index[10];

	while(node){
		OPint* tri = NULL;

		sprintf(index, "%d", (OPint)node->Data);
		OPhashMapGet(triTable, index, (void**)&tri);

		// only add indices if this tri hasn't been visited
		if(!tri[3]){
			for(OPint i = 3; i--;){
				OPllInsertLast(triList, (void*)tri[i]);
			}
			if(atLeaf) tri[3] = 1; // mark tri as visited
		}
		node = node->Next;
	}

	return triList;
}

OPMPartNode CreateOPMPartNode(OPlinkedList* triList){
	OPllNode* node = triList->First;
	OPint min = (OPint)node->Data, max = (OPint)node->Data;

	while(node){
		OPint i = (OPint)node->Data;
		min = min > i ? i : min;
		max = max < i ? i : max;
		node = node->Next;
	}

	OPMPartNode meshNode = {
		min,
		max,
		0,
		NULL
	};

	return meshNode;
}

OPMPartNode OPMPartition(OPMData* data, HashMap* triTable, OPlinkedList* vertList, OPint depth){
	OPMvertex* vertices = (OPMvertex*)data->vertices;
	OPlinkedList* spaceA = OPllCreate();
	OPlinkedList* spaceB = OPllCreate();
	OPllNode* node = vertList->First;

	if(depth){
		OPvec3 com = GetCenterOfMass(data, vertList);

		OPvec3 X = {1, 0, 0};
		OPvec3 Y = {0, 1, 0};
		OPvec3 Z = {0, 0, 1};

		// basis vectors
		OPvec3 bX = {0}, bY = {0}, bZ = {0};

		OPvec3 normal = {0};

		// determine the basis vectors of this vertex ist
		while(node){
			OPint i = (OPint)node->Data;
			UpdateBasis(&X, &bX, &vertices[i].Position);
			UpdateBasis(&Y, &bY, &vertices[i].Position);
			UpdateBasis(&Z, &bZ, &vertices[i].Position);
			node = node->Next;
		}

		normal = GetNormal(bX, bY, bZ);

		// split vertices
		node = vertList->First;
		while(node){
			OPint i = (OPint)node->Data;
			OPvec3 diff = vertices[i].Position - com;

			if(OPvec3valDot(&diff, &normal) > 0){
				OPllInsertLast(spaceA, (void*)i);
			}
			else{
				OPllInsertLast(spaceB, (void*)i);
			}
			node = node->Next;
		}

		ReorderVerts(data, &spaceA, &spaceB);

		// ALL DONE
		OPlinkedList* trisA = CreateTriList(data, triTable, spaceA, !depth);
		OPlinkedList* trisB = CreateTriList(data, triTable, spaceB, !depth);

		// TODO call recusively
		OPMPartNode* children = (OPMPartNode*)OPalloc(sizeof(OPMPartNode) * 2);
		OPMPartNode nodeA = children[0] = OPMPartition(data, triTable, trisA, depth - 1);
		OPMPartNode nodeB = children[1] = OPMPartition(data, triTable, trisB, depth - 1);

		// Determine to and from using the two child nodes
		OPint From = nodeA.From < nodeB.From ? nodeA.From : nodeB.From;
		OPint To = nodeA.To > nodeB.To ? nodeA.To : nodeB.To;

		OPMPartNode partNode = {
			From,
			To,
			2,
			children
		};

		// Free up stuff
		OPllDestroy(spaceA); OPllDestroy(spaceB);
		OPllDestroy(trisA); OPllDestroy(trisB);

		return partNode;
	}
	else{
		OPlinkedList* tris = CreateTriList(data, triTable, vertList, 1);
		OPMPartNode partNode = CreateOPMPartNode(tris);
		return partNode;
	}
}

OPint OPMPartitionedLoad(const OPchar* filename, OPmesh** mesh){
	OPlog("Reading File Data");
	OPstream* str = OPreadFile(filename);
	OPlog("Reading OPMloadData");
	OPMData data = OPMloadData(str);

	HashMap*      triTable = CreateTriangleTable(&data);
	OPlinkedList* vertList = CreateVertexList(&data);

	OPMPartition(&data, triTable, vertList, 1);

	OPmesh temp = OPrenderCreateMesh();
	OPrenderBindMesh(&temp);
	OPrenderBuildMesh(
		data.vertexSize, data.indexSize,
		data.vertexCount, data.indexCount,
		data.vertices, data.indices
	);

	OPlog("Disposing");
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