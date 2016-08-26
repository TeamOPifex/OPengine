#include "./Human/include/Rendering/Primitives/OPsphere.h"

struct OPsphereFace {
	ui64 ind1, ind2, ind3;
	OPsphereFace(ui64 ind1, ui64 ind2, ui64 ind3) {
		this->ind1 = ind1;
		this->ind2 = ind2;
		this->ind3 = ind3;
	}
};

static ui32 sphereIndex;
static f32* sphereVertices;
static ui32 sphereVertexSize;
static i64* middlePointKeys;
static i64* middlePointValues;
static i64 middlePointIndex;

OPvec3 OPsphere::Color = OPVEC3_ZERO;

void OPsphereAddVertex(double x, double y, double z) {
	double length = OPsqrt(x * x + y * y + z * z);
	ui32 index = sphereVertexSize * sphereIndex;
	sphereVertices[index + 0] = x / length;
	sphereVertices[index + 1] = y / length;
	sphereVertices[index + 2] = z / length;
	sphereIndex++;
}

ui32 OPsphereGetMiddlePoint(ui32 p1, ui32 p2)
{
	// first check if we have it already
	bool firstIsSmaller = p1 < p2;
	i64 smallerIndex = firstIsSmaller ? p1 : p2;
	i64 greaterIndex = firstIsSmaller ? p2 : p1;
	i64 key = (smallerIndex << 32) + greaterIndex;

	for (ui64 i = 0; i < middlePointIndex; i++) {
		if (middlePointKeys[i] == key) {
			return middlePointValues[i];
		}
	}

	// not in cache, calculate it
	OPvec3 point1;
	OPvec3 point2;

	point1.x = sphereVertices[p1 * sphereVertexSize + 0];
	point1.y = sphereVertices[p1 * sphereVertexSize + 1];
	point1.z = sphereVertices[p1 * sphereVertexSize + 2];
	point2.x = sphereVertices[p2 * sphereVertexSize + 0];
	point2.y = sphereVertices[p2 * sphereVertexSize + 1];
	point2.z = sphereVertices[p2 * sphereVertexSize + 2];

	// add vertex makes sure point is on unit sphere
	OPsphereAddVertex(
		(point1.x + point2.x) / 2.0,
		(point1.y + point2.y) / 2.0,
		(point1.z + point2.z) / 2.0);


	// store it, return index
	middlePointKeys[middlePointIndex] = key;
	middlePointValues[middlePointIndex] = sphereIndex - 1;
	middlePointIndex++;

	return sphereIndex - 1;
}

OPmodel* OPsphere::Create(ui32 recursionLevel, ui32 features) {
	sphereIndex = 0;

	ui32 totalFaces = 20;
	for (ui32 i = 0; i < recursionLevel; i++)
	{
		ui32 faces = 0;
		for (ui32 j = 0; j < totalFaces; j++) {
			faces += 4;
		}
		totalFaces = faces;
	}

	sphereVertexSize = 3;
	if ((features & (ui32)OPattributes::COLOR) > 0) {
		sphereVertexSize += 3;
	}
	if ((features & (ui32)OPattributes::NORMAL) > 0) {
		sphereVertexSize += 3;
	}
	if ((features & (ui32)OPattributes::UV) > 0) {
		sphereVertexSize += 2;
	}
	sphereVertices = OPALLOC(f32, totalFaces * sphereVertexSize);
	middlePointKeys = OPALLOC(i64, totalFaces * 3);
	middlePointValues = OPALLOC(i64, totalFaces * 3);
	middlePointIndex = 0;

	double t = (1.0 + OPsqrt(5.0)) / 2.0; 
	
	OPsphereAddVertex(-1, t, 0);
	OPsphereAddVertex(1, t, 0);
	OPsphereAddVertex(-1, -t, 0);
	OPsphereAddVertex(1, -t, 0);

	OPsphereAddVertex(0, -1, t);
	OPsphereAddVertex(0, 1, t);
	OPsphereAddVertex(0, -1, -t);
	OPsphereAddVertex(0, 1, -t);

	OPsphereAddVertex(t, 0, -1);
	OPsphereAddVertex(t, 0, 1);
	OPsphereAddVertex(-t, 0, -1);
	OPsphereAddVertex(-t, 0, 1);

	// create 20 triangles of the icosahedron
	OPsphereFace* faces = OPALLOC(OPsphereFace, totalFaces);
	OPsphereFace* tempFaces = OPALLOC(OPsphereFace, totalFaces);

	ui64 faceIndex = 0;
	ui64 tempFaceIndex = 0;

	// 5 faces around point 0
	faces[faceIndex++] = OPsphereFace(0, 11, 5);
	faces[faceIndex++] = OPsphereFace(0, 5, 1);
	faces[faceIndex++] = OPsphereFace(0, 1, 7);
	faces[faceIndex++] = OPsphereFace(0, 7, 10);
	faces[faceIndex++] = OPsphereFace(0, 10, 11);

	// 5 adjacent faces 
	faces[faceIndex++] = OPsphereFace(1, 5, 9);
	faces[faceIndex++] = OPsphereFace(5, 11, 4);
	faces[faceIndex++] = OPsphereFace(11, 10, 2);
	faces[faceIndex++] = OPsphereFace(10, 7, 6);
	faces[faceIndex++] = OPsphereFace(7, 1, 8);

	// 5 faces around point 3
	faces[faceIndex++] = OPsphereFace(3, 9, 4);
	faces[faceIndex++] = OPsphereFace(3, 4, 2);
	faces[faceIndex++] = OPsphereFace(3, 2, 6);
	faces[faceIndex++] = OPsphereFace(3, 6, 8);
	faces[faceIndex++] = OPsphereFace(3, 8, 9);

	// 5 adjacent faces 
	faces[faceIndex++] = OPsphereFace(4, 9, 5);
	faces[faceIndex++] = OPsphereFace(2, 4, 11);
	faces[faceIndex++] = OPsphereFace(6, 2, 10);
	faces[faceIndex++] = OPsphereFace(8, 6, 7);
	faces[faceIndex++] = OPsphereFace(9, 8, 1);

	// refine triangles
	for (int i = 0; i < recursionLevel; i++)
	{
		tempFaceIndex = 0;
		for(ui32 j = 0; j < faceIndex; j++)
		{
			// replace triangle by 4 triangles
			ui32 a = OPsphereGetMiddlePoint(faces[j].ind1, faces[j].ind2);
			ui32 b = OPsphereGetMiddlePoint(faces[j].ind2, faces[j].ind3);
			ui32 c = OPsphereGetMiddlePoint(faces[j].ind3, faces[j].ind1);

			tempFaces[tempFaceIndex++] = OPsphereFace(faces[j].ind1, a, c);
			tempFaces[tempFaceIndex++] = OPsphereFace(faces[j].ind2, b, a);
			tempFaces[tempFaceIndex++] = OPsphereFace(faces[j].ind3, c, b);
			tempFaces[tempFaceIndex++] = OPsphereFace(a, b, c);
		}

		OPmemcpy(faces, tempFaces, sizeof(OPsphereFace) * tempFaceIndex);
		faceIndex = tempFaceIndex;
	}

	ui32* indices = OPALLOC(ui32, faceIndex * 3);

	for (ui32 i = 0; i < faceIndex; i++) {
		indices[i * 3 + 0] = faces[i].ind1;
		indices[i * 3 + 1] = faces[i].ind2;
		indices[i * 3 + 2] = faces[i].ind3;
	}


	ui32 offset = 3;
	if ((features & (ui32)OPattributes::COLOR) > 0) {
		for (ui32 i = 0; i < sphereIndex; i++) {
			sphereVertices[i * sphereVertexSize + offset + 0] = Color.x;
			sphereVertices[i * sphereVertexSize + offset + 1] = Color.y;
			sphereVertices[i * sphereVertexSize + offset + 2] = Color.z;
		}
		offset += 3;
	}

	if ((features & (ui32)OPattributes::NORMAL) > 0) {
		for (ui32 i = 0; i < sphereIndex; i++) {
			OPvec3 norm = OPvec3Norm(
				OPvec3(
					sphereVertices[i * sphereVertexSize + 0],
					sphereVertices[i * sphereVertexSize + 1],
					sphereVertices[i * sphereVertexSize + 2]
				)
			);
			sphereVertices[i * sphereVertexSize + offset + 0] = norm.x;
			sphereVertices[i * sphereVertexSize + offset + 1] = norm.y;
			sphereVertices[i * sphereVertexSize + offset + 2] = norm.z;
		}
		offset += 3;
	}

	if ((features & (ui32)OPattributes::UV) > 0) {
		for (ui32 i = 0; i < sphereIndex; i++) {
			OPvec3 norm = OPvec3Norm(
				OPvec3(
					sphereVertices[i * sphereVertexSize + 0],
					sphereVertices[i * sphereVertexSize + 1],
					sphereVertices[i * sphereVertexSize + 2]
				)
			);
			
			sphereVertices[i * sphereVertexSize + offset + 0] = OPasin(norm.x) / OPpi + 0.5f;
			sphereVertices[i * sphereVertexSize + offset + 1] = OPasin(norm.y) / OPpi + 0.5f;
		}
		offset += 2;
	}

	OPvertexLayoutBuilder vertexLayoutBuilder = OPvertexLayoutBuilder(features);
	OPmodel* model = OPNEW(OPmodel(1, vertexLayoutBuilder.Build()));
	model->Build(sphereIndex, faceIndex * 3, OPindexSize::INT, sphereVertices, indices);

	OPfree(indices);
	OPfree(middlePointKeys);
	OPfree(middlePointValues);
	OPfree(sphereVertices);
	OPfree(faces);
	OPfree(tempFaces);

	return model;
}