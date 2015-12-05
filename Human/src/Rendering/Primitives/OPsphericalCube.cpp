#include "./Human/include/Rendering/Primitives/OPsphericalCube.h"
#include "./Math/include/OPray3D.h"
#include "./Core/include/Assert.h"
#include "./Core/include/Assert.h"

OPsphericalCube OPsphericalCubeCreate(OPimage * faces) {

	ASSERT(faces != NULL, "Faces are required");
	ui32 LWH = faces[0].Width;

	for(ui8 face = 0; face < 6; face++) {
		ASSERT(faces[face].Width == LWH && faces[face].Height == LWH, "Textures must be the same size");
	}


	// Vec 3 Position
	// Vec 3 Normal
	// Vec 2 UV
	OPint VertsPerSide = LWH * LWH;
	OPint PerSideSize = sizeof(OPsphericalCubeVertex)* VertsPerSide;
	OPint TotalVerts = VertsPerSide * 6;
	OPsphericalCubeVertex* vertsMem = (OPsphericalCubeVertex*)OPalloc(PerSideSize * 6);

	OPsphericalCubeVertex* verts[6];
	verts[OP_SPHERICAL_CUBE_SIDE_TOP] = &vertsMem[OP_SPHERICAL_CUBE_SIDE_TOP * VertsPerSide];
	verts[OP_SPHERICAL_CUBE_SIDE_BOTTOM] = &vertsMem[OP_SPHERICAL_CUBE_SIDE_BOTTOM * VertsPerSide];
	verts[OP_SPHERICAL_CUBE_SIDE_LEFT] = &vertsMem[OP_SPHERICAL_CUBE_SIDE_LEFT * VertsPerSide];
	verts[OP_SPHERICAL_CUBE_SIDE_RIGHT] = &vertsMem[OP_SPHERICAL_CUBE_SIDE_RIGHT * VertsPerSide];
	verts[OP_SPHERICAL_CUBE_SIDE_FRONT] = &vertsMem[OP_SPHERICAL_CUBE_SIDE_FRONT * VertsPerSide];
	verts[OP_SPHERICAL_CUBE_SIDE_BACK] = &vertsMem[OP_SPHERICAL_CUBE_SIDE_BACK * VertsPerSide];

	OPvec3 min;
	OPvec3 max;
	OPvec3 diff;


	//Front face
	// -1,1,-1 to 1,-1,-1
	{
		min = OPvec3Create(OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE);
		max = OPvec3Create(-OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE);
		diff = max - min;
		for (OPint x = 0; x < LWH; x++)
		{
			OPfloat ratioX = (x / (OPfloat)(LWH - 1));
			OPfloat xPos = min.x + ratioX*diff.x;
			for (OPint y = 0; y < LWH; y++)
			{
				OPfloat ratioY = (y / (OPfloat)(LWH - 1));
				OPfloat yPos = min.y + ratioY*diff.y;
				OPvec3 pos = OPvec3Create(xPos, yPos, max.z);

				verts[OP_SPHERICAL_CUBE_SIDE_BACK][x * LWH + y].pos = pos;
				verts[OP_SPHERICAL_CUBE_SIDE_BACK][x * LWH + y].uv = OPvec2Create(x / (float)(LWH - 1), y / (float)(LWH - 1));
			}
		}
	}

	//Back face
	// -1,1,-1 to 1,-1,-1
	{
		min = OPvec3Create(OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE);
		max = OPvec3Create(-OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE);
		diff = max - min;
		for (OPint x = 0; x < LWH; x++)
		{
			OPfloat ratioX = (x / (float)(LWH - 1));
			OPfloat xPos = min.x + ratioX*diff.x;
			for (OPint y = 0; y < LWH; y++)
			{
				OPfloat ratioY = (y / (float)(LWH - 1));
				OPfloat yPos = min.y + ratioY*diff.y;
				OPvec3 pos = OPvec3Create(xPos, yPos, max.z);

				verts[OP_SPHERICAL_CUBE_SIDE_FRONT][x * LWH + y].pos = pos;
				verts[OP_SPHERICAL_CUBE_SIDE_FRONT][x * LWH + y].uv = OPvec2Create(x / (float)(LWH - 1), y / (float)(LWH - 1));
			}
		}
	}

	//Right face
	// -1,1,-1 to 1,-1,-1
	{
		min = OPvec3Create(-OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE);
		max = OPvec3Create(-OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE);
		diff = max - min;
		for (OPint x = 0; x < LWH; x++)
		{
			OPfloat ratioX = (x / (float)(LWH - 1));
			OPfloat xPos = min.z + ratioX*diff.z;
			for (OPint y = 0; y < LWH; y++)
			{
				OPfloat ratioY = (y / (float)(LWH - 1));
				OPfloat yPos = min.y + ratioY*diff.y;
				OPvec3 pos = OPvec3Create(max.x, yPos, xPos);

				verts[OP_SPHERICAL_CUBE_SIDE_LEFT][x * LWH + y].pos = pos;
				verts[OP_SPHERICAL_CUBE_SIDE_LEFT][x * LWH + y].uv = OPvec2Create(x / (float)(LWH - 1), y / (float)(LWH - 1));
			}
		}
	}

	//Left face
	// -1,1,-1 to 1,-1,-1
	{
		min = OPvec3Create(OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE);
		max = OPvec3Create(OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE);
		diff = max - min;
		for (OPint x = 0; x < LWH; x++)
		{
			OPfloat ratioX = (x / (float)(LWH - 1));
			OPfloat xPos = min.z + ratioX*diff.z;
			for (OPint y = 0; y < LWH; y++)
			{
				OPfloat ratioY = (y / (float)(LWH - 1));
				OPfloat yPos = min.y + ratioY*diff.y;
				OPvec3 pos = OPvec3Create(max.x, yPos, xPos);

				verts[OP_SPHERICAL_CUBE_SIDE_RIGHT][x * LWH + y].pos = pos;
				verts[OP_SPHERICAL_CUBE_SIDE_RIGHT][x * LWH + y].uv = OPvec2Create(x / (float)(LWH - 1), y / (float)(LWH - 1));
			}
		}
	}

	//Top face
	// -1,1,-1 to 1,-1,-1
	{
		min = OPvec3Create(OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE);
		max = OPvec3Create(-OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE);
		diff = max - min;
		for (OPint x = 0; x < LWH; x++)
		{
			OPfloat ratioX = (x / (float)(LWH - 1));
			OPfloat xPos = min.x + ratioX * diff.x;
			for (OPint y = 0; y < LWH; y++)
			{
				OPfloat ratioY = (y / (float)(LWH - 1));
				OPfloat yPos = min.z + ratioY*diff.z;
				OPvec3 pos = OPvec3Create(xPos, max.y, yPos);

				verts[OP_SPHERICAL_CUBE_SIDE_TOP][x * LWH + y].pos = pos;
				verts[OP_SPHERICAL_CUBE_SIDE_TOP][x * LWH + y].uv = OPvec2Create(x / (float)(LWH - 1), y / (float)(LWH - 1));
			}
		}
	}

	//Bottom face
	// -1,1,-1 to 1,-1,-1
	{
		min = OPvec3Create(-OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE);
		max = OPvec3Create(OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE);
		diff = max - min;
		for (OPint x = 0; x < LWH; x++)
		{
			OPfloat ratioX = (x / (float)(LWH - 1));
			OPfloat xPos = min.x + ratioX * diff.x;
			for (OPint y = 0; y < LWH; y++)
			{
				OPfloat ratioY = (y / (float)(LWH - 1));
				OPfloat yPos = min.z + ratioY*diff.z;
				OPvec3 pos = OPvec3Create(xPos, max.y, yPos);

				verts[OP_SPHERICAL_CUBE_SIDE_BOTTOM][x * LWH + y].pos = pos;
				verts[OP_SPHERICAL_CUBE_SIDE_BOTTOM][x * LWH + y].uv = OPvec2Create(x / (float)(LWH - 1), y / (float)(LWH - 1));
			}
		}
	}

	// Sphericalize the coordinates
	for (OPint i = 0; i < TotalVerts; i++)
	{
		vertsMem[i].pos = OPvec3Norm(vertsMem[i].pos);
		vertsMem[i].norm = OPvec3Norm(vertsMem[i].pos);
	}

	OPint index = 0;
	OPint IndicesPerSide = (((LWH - 1)*(LWH - 1) * 6) / OPSPHERICALCUBE_SCL);
	OPint totalIndices = IndicesPerSide * 6;
	ui16* indicesMem = (ui16*)OPalloc(sizeof(ui16)* totalIndices);

	ui16* indices[6];
	indices[OP_SPHERICAL_CUBE_SIDE_TOP] = &indicesMem[OP_SPHERICAL_CUBE_SIDE_TOP * IndicesPerSide];
	indices[OP_SPHERICAL_CUBE_SIDE_BOTTOM] = &indicesMem[OP_SPHERICAL_CUBE_SIDE_BOTTOM * IndicesPerSide];
	indices[OP_SPHERICAL_CUBE_SIDE_LEFT] = &indicesMem[OP_SPHERICAL_CUBE_SIDE_LEFT * IndicesPerSide];
	indices[OP_SPHERICAL_CUBE_SIDE_RIGHT] = &indicesMem[OP_SPHERICAL_CUBE_SIDE_RIGHT * IndicesPerSide];
	indices[OP_SPHERICAL_CUBE_SIDE_FRONT] = &indicesMem[OP_SPHERICAL_CUBE_SIDE_FRONT * IndicesPerSide];
	indices[OP_SPHERICAL_CUBE_SIDE_BACK] = &indicesMem[OP_SPHERICAL_CUBE_SIDE_BACK * IndicesPerSide];


	for (OPint i = 0; i < 6; i++) {
		for (OPint x = 0; x < (LWH - 1); x += OPSPHERICALCUBE_SCL)
		{
			for (OPint y = 0; y < (LWH - 1); y += OPSPHERICALCUBE_SCL)
			{
				OPint tscl = OPSPHERICALCUBE_SCL;
				if ((LWH - 1) - y < tscl)
					tscl = (LWH - 1) - y;
				OPint xscl = OPSPHERICALCUBE_SCL;
				if ((LWH - 1) - x < xscl)
					xscl = (LWH - 1) - x;

				indices[i][index++] = x*LWH + y;
				indices[i][index++] = x*LWH + y + tscl;
				indices[i][index++] = (x + xscl)*LWH + y + tscl;
				indices[i][index++] = x*LWH + y;
				indices[i][index++] = (x + xscl)*LWH + y + tscl;
				indices[i][index++] = (x + xscl)*LWH + y;
			}
		}
		index = 0;
	}

	for(ui8 face = 0; face < 6; face++) {
		for (OPint x = 0; x < (LWH - 1); x += OPSPHERICALCUBE_SCL) {
			for (OPint y = 0; y < (LWH - 1); y += OPSPHERICALCUBE_SCL) {
				f32 _x = (f32)faces[face].Data[4 * x * LWH + (y + 0) * 4];
				verts[face][y * LWH + x].pos *= 1.0 + (_x - 128.0f) / 255.0f;
			}
		}
	}


	OPsphericalCube result;

	for (OPint i = 0; i < 6; i++) {
		result.sides[i] = OPmeshCreate();
		OPmeshBind(&result.sides[i]);

		OPmeshBuild(
				sizeof(OPsphericalCubeVertex), sizeof(ui16),
				VertsPerSide, IndicesPerSide,
				(void*)verts[i], (void*)indices[i]
		);
	}

	OPfree(vertsMem);
	OPfree(indicesMem);

	return result;
}

OPsphericalCube OPsphericalCubeCreate(OPint size) {

	OPint LWH = size;
	// Vec 3 Position
	// Vec 3 Normal
	// Vec 2 UV
	OPint VertsPerSide = LWH * LWH;
	OPint PerSideSize = sizeof(OPsphericalCubeVertex)* VertsPerSide;
	OPint TotalVerts = VertsPerSide * 6;
	OPsphericalCubeVertex* vertsMem = (OPsphericalCubeVertex*)OPalloc(PerSideSize * 6);

	OPsphericalCubeVertex* verts[6];
	verts[OP_SPHERICAL_CUBE_SIDE_TOP] = &vertsMem[OP_SPHERICAL_CUBE_SIDE_TOP * VertsPerSide];
	verts[OP_SPHERICAL_CUBE_SIDE_BOTTOM] = &vertsMem[OP_SPHERICAL_CUBE_SIDE_BOTTOM * VertsPerSide];
	verts[OP_SPHERICAL_CUBE_SIDE_LEFT] = &vertsMem[OP_SPHERICAL_CUBE_SIDE_LEFT * VertsPerSide];
	verts[OP_SPHERICAL_CUBE_SIDE_RIGHT] = &vertsMem[OP_SPHERICAL_CUBE_SIDE_RIGHT * VertsPerSide];
	verts[OP_SPHERICAL_CUBE_SIDE_FRONT] = &vertsMem[OP_SPHERICAL_CUBE_SIDE_FRONT * VertsPerSide];
	verts[OP_SPHERICAL_CUBE_SIDE_BACK] = &vertsMem[OP_SPHERICAL_CUBE_SIDE_BACK * VertsPerSide];

	OPvec3 min;
	OPvec3 max;
	OPvec3 diff;

	//Front face
	// -1,1,-1 to 1,-1,-1
	{
		min = OPvec3Create(OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE);
		max = OPvec3Create(-OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE);
		diff = max - min;
		for (OPint x = 0; x < LWH; x++)
		{
			OPfloat ratioX = (x / (OPfloat)(LWH - 1));
			OPfloat xPos = min.x + ratioX*diff.x;
			for (OPint y = 0; y < LWH; y++)
			{
				OPfloat ratioY = (y / (OPfloat)(LWH - 1));
				OPfloat yPos = min.y + ratioY*diff.y;
				OPvec3 pos = OPvec3Create(xPos, yPos, max.z);

				verts[OP_SPHERICAL_CUBE_SIDE_BACK][x * LWH + y].pos = pos;
				verts[OP_SPHERICAL_CUBE_SIDE_BACK][x * LWH + y].uv = OPvec2Create(x / (float)(LWH - 1), y / (float)(LWH - 1));
			}
		}
	}

	//Back face
	// -1,1,-1 to 1,-1,-1
	{
		min = OPvec3Create(OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE);
		max = OPvec3Create(-OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE);
		diff = max - min;
		for (OPint x = 0; x < LWH; x++)
		{
			OPfloat ratioX = (x / (float)(LWH - 1));
			OPfloat xPos = min.x + ratioX*diff.x;
			for (OPint y = 0; y < LWH; y++)
			{
				OPfloat ratioY = (y / (float)(LWH - 1));
				OPfloat yPos = min.y + ratioY*diff.y;
				OPvec3 pos = OPvec3Create(xPos, yPos, max.z);

				verts[OP_SPHERICAL_CUBE_SIDE_FRONT][x * LWH + y].pos = pos;
				verts[OP_SPHERICAL_CUBE_SIDE_FRONT][x * LWH + y].uv = OPvec2Create(x / (float)(LWH - 1), y / (float)(LWH - 1));
			}
		}
	}

	//Right face
	// -1,1,-1 to 1,-1,-1
	{
		min = OPvec3Create(-OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE);
		max = OPvec3Create(-OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE);
		diff = max - min;
		for (OPint x = 0; x < LWH; x++)
		{
			OPfloat ratioX = (x / (float)(LWH - 1));
			OPfloat xPos = min.z + ratioX*diff.z;
			for (OPint y = 0; y < LWH; y++)
			{
				OPfloat ratioY = (y / (float)(LWH - 1));
				OPfloat yPos = min.y + ratioY*diff.y;
				OPvec3 pos = OPvec3Create(max.x, yPos, xPos);

				verts[OP_SPHERICAL_CUBE_SIDE_LEFT][x * LWH + y].pos = pos;
				verts[OP_SPHERICAL_CUBE_SIDE_LEFT][x * LWH + y].uv = OPvec2Create(x / (float)(LWH - 1), y / (float)(LWH - 1));
			}
		}
	}

	//Left face
	// -1,1,-1 to 1,-1,-1
	{
		min = OPvec3Create(OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE);
		max = OPvec3Create(OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE);
		diff = max - min;
		for (OPint x = 0; x < LWH; x++)
		{
			OPfloat ratioX = (x / (float)(LWH - 1));
			OPfloat xPos = min.z + ratioX*diff.z;
			for (OPint y = 0; y < LWH; y++)
			{
				OPfloat ratioY = (y / (float)(LWH - 1));
				OPfloat yPos = min.y + ratioY*diff.y;
				OPvec3 pos = OPvec3Create(max.x, yPos, xPos);

				verts[OP_SPHERICAL_CUBE_SIDE_RIGHT][x * LWH + y].pos = pos;
				verts[OP_SPHERICAL_CUBE_SIDE_RIGHT][x * LWH + y].uv = OPvec2Create(x / (float)(LWH - 1), y / (float)(LWH - 1));
			}
		}
	}

	//Top face
	// -1,1,-1 to 1,-1,-1
	{
		min = OPvec3Create(OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE);
		max = OPvec3Create(-OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE);
		diff = max - min;
		for (OPint x = 0; x < LWH; x++)
		{
			OPfloat ratioX = (x / (float)(LWH - 1));
			OPfloat xPos = min.x + ratioX * diff.x;
			for (OPint y = 0; y < LWH; y++)
			{
				OPfloat ratioY = (y / (float)(LWH - 1));
				OPfloat yPos = min.z + ratioY*diff.z;
				OPvec3 pos = OPvec3Create(xPos, max.y, yPos);

				verts[OP_SPHERICAL_CUBE_SIDE_TOP][x * LWH + y].pos = pos;
				verts[OP_SPHERICAL_CUBE_SIDE_TOP][x * LWH + y].uv = OPvec2Create(x / (float)(LWH - 1), y / (float)(LWH - 1));
			}
		}
	}

	//Bottom face
	// -1,1,-1 to 1,-1,-1
	{
		min = OPvec3Create(-OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE, OPSPHERICALCUBE_SCALE);
		max = OPvec3Create(OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE, -OPSPHERICALCUBE_SCALE);
		diff = max - min;
		for (OPint x = 0; x < LWH; x++)
		{
			OPfloat ratioX = (x / (float)(LWH - 1));
			OPfloat xPos = min.x + ratioX * diff.x;
			for (OPint y = 0; y < LWH; y++)
			{
				OPfloat ratioY = (y / (float)(LWH - 1));
				OPfloat yPos = min.z + ratioY*diff.z;
				OPvec3 pos = OPvec3Create(xPos, max.y, yPos);

				verts[OP_SPHERICAL_CUBE_SIDE_BOTTOM][x * LWH + y].pos = pos;
				verts[OP_SPHERICAL_CUBE_SIDE_BOTTOM][x * LWH + y].uv = OPvec2Create(x / (float)(LWH - 1), y / (float)(LWH - 1));
			}
		}
	}

	// Sphericalize the coordinates
 	for (OPint i = 0; i < TotalVerts; i++)
	{
		vertsMem[i].pos = OPvec3Norm(vertsMem[i].pos);
		vertsMem[i].norm = OPvec3Norm(vertsMem[i].pos);
	}

	OPint index = 0;
	OPint IndicesPerSide = (((LWH - 1)*(LWH - 1) * 6) / OPSPHERICALCUBE_SCL);
	OPint totalIndices = IndicesPerSide * 6;
	ui16* indicesMem = (ui16*)OPalloc(sizeof(ui16)* totalIndices);

	ui16* indices[6];
	indices[OP_SPHERICAL_CUBE_SIDE_TOP] = &indicesMem[OP_SPHERICAL_CUBE_SIDE_TOP * IndicesPerSide];
	indices[OP_SPHERICAL_CUBE_SIDE_BOTTOM] = &indicesMem[OP_SPHERICAL_CUBE_SIDE_BOTTOM * IndicesPerSide];
	indices[OP_SPHERICAL_CUBE_SIDE_LEFT] = &indicesMem[OP_SPHERICAL_CUBE_SIDE_LEFT * IndicesPerSide];
	indices[OP_SPHERICAL_CUBE_SIDE_RIGHT] = &indicesMem[OP_SPHERICAL_CUBE_SIDE_RIGHT * IndicesPerSide];
	indices[OP_SPHERICAL_CUBE_SIDE_FRONT] = &indicesMem[OP_SPHERICAL_CUBE_SIDE_FRONT * IndicesPerSide];
	indices[OP_SPHERICAL_CUBE_SIDE_BACK] = &indicesMem[OP_SPHERICAL_CUBE_SIDE_BACK * IndicesPerSide];


	for (OPint i = 0; i < 6; i++) {
		for (OPint x = 0; x < (LWH - 1); x += OPSPHERICALCUBE_SCL)
		{
			for (OPint y = 0; y < (LWH - 1); y += OPSPHERICALCUBE_SCL)
			{
				OPint tscl = OPSPHERICALCUBE_SCL;
				if ((LWH - 1) - y < tscl)
					tscl = (LWH - 1) - y;
				OPint xscl = OPSPHERICALCUBE_SCL;
				if ((LWH - 1) - x < xscl)
					xscl = (LWH - 1) - x;

				indices[i][index++] = x*LWH + y;
				indices[i][index++] = x*LWH + y + tscl;
				indices[i][index++] = (x + xscl)*LWH + y + tscl;
				indices[i][index++] = x*LWH + y;
				indices[i][index++] = (x + xscl)*LWH + y + tscl;
				indices[i][index++] = (x + xscl)*LWH + y;
			}
		}
		index = 0;
	}

	OPsphericalCube result;

	for (OPint i = 0; i < 6; i++) {
		result.sides[i] = OPmeshCreate();
		OPmeshBind(&result.sides[i]);

		OPmeshBuild(
			sizeof(OPsphericalCubeVertex), sizeof(ui16),
			VertsPerSide, IndicesPerSide,
			(void*)verts[i], (void*)indices[i]
			);
	}

	OPfree(vertsMem);
	OPfree(indicesMem);

	return result;
}

OPvec2 OPsphericalCubePlanePositionSide(const OPvec3 pos, OPsphericalCubeSide side) {

	OPvec3 position = OPVEC3_ZERO;
	OPray3D ray = { OPvec3Create(0,0,0), pos };
	OPplane3D plane;
	OPint result;

	switch(side) {

		// Test Top
		case OP_SPHERICAL_CUBE_SIDE_TOP: {
			plane.position = OPvec3Create(0, 0.5, 0);
			plane.normal = OPvec3Create(0, 1, 0);
			result = OPplane3DIntersects(plane, ray, &position);
			return OPvec2Create(position.x, position.z);
		}

		// Test Bottom
		case OP_SPHERICAL_CUBE_SIDE_BOTTOM: {
			plane.position = OPvec3Create(0, -0.5, 0);
			plane.normal = OPvec3Create(0, -1, 0);
			result = OPplane3DIntersects(plane, ray, &position);
			return OPvec2Create(-position.x, -position.z);
		}

		// Test Front
		case OP_SPHERICAL_CUBE_SIDE_FRONT: {
			plane.position = OPvec3Create(0, 0, 0.5);
			plane.normal = OPvec3Create(0, 0, 1);
			result = OPplane3DIntersects(plane, ray, &position);
			return OPvec2Create(position.x, -position.y);
		}

		// Test Back
		case OP_SPHERICAL_CUBE_SIDE_BACK: {
			plane.position = OPvec3Create(0, 0, -0.5);
			plane.normal = OPvec3Create(0, 0, -1);
			result = OPplane3DIntersects(plane, ray, &position);
			return OPvec2Create(position.x, -position.y);
		}

		// Test Left
		case OP_SPHERICAL_CUBE_SIDE_LEFT: {
			plane.position = OPvec3Create(-0.5, 0, 0);
			plane.normal = OPvec3Create(-1, 0, 0);
			result = OPplane3DIntersects(plane, ray, &position);
			return OPvec2Create(-position.z, -position.y);
		}

		// Test Right
		case OP_SPHERICAL_CUBE_SIDE_RIGHT: {
			plane.position = OPvec3Create(0.5, 0, 0);
			plane.normal = OPvec3Create(1, 0, 0);
			result = OPplane3DIntersects(plane, ray, &position);
			return OPvec2Create(position.z, -position.y);
		}

		case _OP_SPHERICAL_CUBE_SIDE_MAX: {
			ASSERT(false, "Should never use the enumeration count of OPsphericalCube");
		}
	}

	return OPVEC2_ZERO;
}

OPvec2 OPsphericalCubePlanePosition(const OPvec3 pos, OPsphericalCubeSide* side) {

	//ASSERT(pos != OPVEC3_ZERO, "Cannot get position of zero");

	OPvec3 position = OPVEC3_ZERO;
	OPray3D ray = { OPvec3Create(0,0,0), pos };
	OPplane3D plane;
	d64 bounds = OPsqrt(1.0);
	OPint result;
	d64 dist;

	// Test Top
	{
		plane.position = OPvec3Create(0, 0.5, 0);
		plane.normal = OPvec3Create(0, 1, 0);
		result = OPplane3DIntersects(plane, ray, &position);
		if( result && OPabs(position.x) < 0.5 && OPabs(position.z) < 0.5) {
			(*side) = OP_SPHERICAL_CUBE_SIDE_TOP;
			//OPlg("T");
			return OPvec2Create(position.x, position.z);
		}
	}

	// Test Bottom
	{
		plane.position = OPvec3Create(0, -0.5, 0);
		plane.normal = OPvec3Create(0, -1, 0);
		result = OPplane3DIntersects(plane, ray, &position);
		if( result && OPabs(position.x) < 0.5 && OPabs(position.z) < 0.5) {
			(*side) = OP_SPHERICAL_CUBE_SIDE_BOTTOM;
			//OPlg("B");
			return OPvec2Create(-position.x, -position.z);
		}
	}

	// Test Front
	{
		plane.position = OPvec3Create(0, 0, 0.5);
		plane.normal = OPvec3Create(0, 0, 1);
		result = OPplane3DIntersects(plane, ray, &position);
		if( result && OPabs(position.x) < 0.5 && OPabs(position.y) < 0.5) {
			(*side) = OP_SPHERICAL_CUBE_SIDE_FRONT;
			//OPlg("F");
			return OPvec2Create(position.x, -position.y);
		}
	}

	// Test Back
	{
		plane.position = OPvec3Create(0, 0, -0.5);
		plane.normal = OPvec3Create(0, 0, -1);
		result = OPplane3DIntersects(plane, ray, &position);
		if( result && OPabs(position.x) < 0.5 && OPabs(position.y) < 0.5) {
			(*side) = OP_SPHERICAL_CUBE_SIDE_BACK;
			//OPlg("B");
			return OPvec2Create(position.x, -position.y);
		}
	}

	// Test Left
	{
		plane.position = OPvec3Create(-0.5, 0, 0);
		plane.normal = OPvec3Create(-1, 0, 0);
		result = OPplane3DIntersects(plane, ray, &position);
		if( result && OPabs(position.z) < 0.5 && OPabs(position.y) < 0.5) {
			(*side) = OP_SPHERICAL_CUBE_SIDE_LEFT;
			//OPlg("L");
			return OPvec2Create(-position.z, -position.y);
		}
	}

	// Test Right
	{
		plane.position = OPvec3Create(0.5, 0, 0);
		plane.normal = OPvec3Create(1, 0, 0);
		result = OPplane3DIntersects(plane, ray, &position);
		if( result && OPabs(position.z) < 0.5 && OPabs(position.y) < 0.5) {
			(*side) = OP_SPHERICAL_CUBE_SIDE_RIGHT;
			//OPlg("R");
			return OPvec2Create(position.z, -position.y);
		}
	}

	return OPVEC2_ZERO;
}

OPvec3 OPsphericalCubePosition(OPvec3 pos, OPsphericalCubeSide* side)
{
	OPvec3 position = pos;

	d64 x = position.x;
	d64 y = position.y;
	d64 z = position.z;

	OPfloat fx = OPabs(x);
	OPfloat fy = OPabs(y);
	OPfloat fz = OPabs(z);

	d64 inverseSqrt2 = 1.0f / OPsqrt(2.0f);// 0.70710676908493042f;

	d64 two = 2.0;
	d64 twelve = 12.0;
	d64 three = 3.0;

	if (fy >= fx && fy >= fz)
	{
		d64 a2 = x * x * two;
		d64 b2 = z * z * two;
		d64 inner = -a2 + b2 - three;
		d64 innersqrt = -OPsqrt((inner * inner) - twelve * a2);

		if (x > -0.01 && x < 0.01)
			position.x = 0.0f;
		else
			position.x = (OPfloat)(OPsqrt(innersqrt + a2 - b2 + three) * inverseSqrt2);

		if (z > -0.01 && z < 0.01)
			position.z = 0.0f;
		else
			position.z = (OPfloat)(OPsqrt(innersqrt - a2 + b2 + three) * inverseSqrt2);

		if (position.x > 1.0) position.x = 1.0f;
		if (position.z > 1.0) position.z = 1.0f;

		if (x < 0.0) position.x = -position.x;
		if (z < 0.0) position.z = -position.z;

		if (y > 0.0) // top face
		{
			position.y = 1.0f;
			*side = OP_SPHERICAL_CUBE_SIDE_TOP;
		}
		else // bottom face
		{
			position.y = -1.0f;
			*side = OP_SPHERICAL_CUBE_SIDE_BOTTOM;
		}
	}
	else if (fx >= fy && fx >= fz)
	{
		d64 a2 = y * y * two;
		d64 b2 = z * z * two;
		d64 inner = -a2 + b2 - three;
		d64 innersqrt = -OPsqrt((inner * inner) - twelve * a2);

		if (y > -0.01 && y < 0.01)
			position.y = 0.0f;
		else
			position.y = (OPfloat)(OPsqrt(innersqrt + a2 - b2 + three) * inverseSqrt2);

		if (z > -0.01 && z < 0.01)
			position.z = 0.0f;
		else
			position.z = (OPfloat)(OPsqrt(innersqrt - a2 + b2 + three) * inverseSqrt2);

		if (position.y > 1.0) position.y = 1.0f;
		if (position.z > 1.0) position.z = 1.0f;

		if (y < 0) position.y = -position.y;
		if (z < 0) position.z = -position.z;

		if (x > 0) // right face
		{
			position.x = 1.0f;
			*side = OP_SPHERICAL_CUBE_SIDE_RIGHT;
		}
		else // left face
		{
			position.x = -1.0f;
			*side = OP_SPHERICAL_CUBE_SIDE_LEFT;
		}
	}
	else
	{
		d64 a2 = x * x * two;
		d64 b2 = y * y * two;
		d64 inner = -a2 + b2 - three;
		d64 innersqrt = -OPsqrt((inner * inner) - twelve * a2);

		if (x > -0.01 && x < 0.01)
			position.x = 0.0f;
		else
			position.x = (float)(OPsqrt(innersqrt + a2 - b2 + three) * inverseSqrt2);

		if (y > -0.01 && y < 0.01)
			position.y = 0.0f;
		else
			position.y = (float)(OPsqrt(innersqrt - a2 + b2 + three) * inverseSqrt2);


		if (position.x > 1.0) position.x = 1.0f;
		if (position.y > 1.0) position.y = 1.0f;

		if (x < 0) position.x = -position.x;
		if (y < 0) position.y = -position.y;

		if (z > 0) // front face
		{
			position.z = 1.0f;
			*side = OP_SPHERICAL_CUBE_SIDE_FRONT;
		}
		else // back face
		{
			position.z = -1.0f;
			*side = OP_SPHERICAL_CUBE_SIDE_BACK;
		}
	}

	return position;
}

void         OPsphericalCubeDestroy(OPsphericalCube* sphericalCube) {

}
