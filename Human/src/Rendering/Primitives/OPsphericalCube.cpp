#include "./Human/include/Rendering/Primitives/OPsphericalCube.h"

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
		//OPvec3 pos = OPVEC3_ZERO;
		//pos.x = p.x*(float)OPsqrt(1 - (p.y*p.y) / 2 - (p.z*p.z) / 2 + (p.y*p.y*p.z*p.z) / 3);
		//pos.y = p.y*(float)OPsqrt(1 - (p.z*p.z) / 2 - (p.x*p.x) / 2 + (p.x*p.x*p.z*p.z) / 3);
		//pos.z = p.z*(float)OPsqrt(1 - (p.x*p.x) / 2 - (p.y*p.y) / 2 + (p.y*p.y*p.x*p.x) / 3);
		vertsMem[i].pos = OPvec3Norm(vertsMem[i].pos);
	}
	


		//VertexBuffers[j] = new VertexBuffer(Engine.GraphicsDevice, CustomVertex.VertexDeclaration, verts[j].Length, BufferUsage.None);
		//VertexBuffers[j].SetData<CustomVertex>(verts[j], 0, verts[j].Length);
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

	//IndexBuffers[i] = new IndexBuffer(Engine.GraphicsDevice, IndexElementSize.ThirtyTwoBits, indices[i].Length, BufferUsage.None);
	//IndexBuffers[i].SetData(indices[i], 0, indices[i].Length);


	//CalculateNormals(verts, SetUpIndices());

	for (OPint i = 0; i < VertsPerSide * 6; i++)
	{
		vertsMem[i].norm = OPVEC3_ZERO;
	}

	for (OPint j = 0; j < 6; j++) {
		for (OPint i = 0; i < IndicesPerSide / 3; i++)
		{
			ui16 index1 = indices[j][i * 3];
			ui16 index2 = indices[j][i * 3 + 1];
			ui16 index3 = indices[j][i * 3 + 2];

			OPvec3 side1 = verts[j][index1].pos - verts[j][index3].pos;
			OPvec3 side2 = verts[j][index1].pos - verts[j][index2].pos;
			OPvec3 normal = OPvec3Cross(side1, side2);

			verts[j][index1].norm += normal;
			verts[j][index2].norm += normal;
			verts[j][index3].norm += normal;
		}
	}

	for (OPint i = 0; i < VertsPerSide * 6; i++) {
		vertsMem[i].norm = OPvec3Norm(vertsMem[i].norm);
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

	return result;
}