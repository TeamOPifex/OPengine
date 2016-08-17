#include "./Human/include/Rendering/Primitives/OPsphericalCube.h"
#include "./Math/include/OPray3D.h"
#include "./Core/include/Assert.h"
#include "./Core/include/Assert.h"

void OPsphericalCube::Init(OPimage * faces) {

	ASSERT(faces != NULL, "Faces are required");
	ui32 LWH = faces[0].Width;

	for(ui8 face = 0; face < 6; face++) {
		ASSERT(faces[face].Width == LWH && faces[face].Height == LWH, "Textures must be the same size");
	}


	// Vec 3 Position
	// Vec 3 Normal
	// Vec 2 UV
	ui32 VertsPerSide = LWH * LWH;
	ui32 PerSideSize = sizeof(OPsphericalCubeVertex)* VertsPerSide;
	ui32 TotalVerts = VertsPerSide * 6;
	OPsphericalCubeVertex* vertsMem = (OPsphericalCubeVertex*)OPalloc(PerSideSize * 6);

	OPsphericalCubeVertex* verts[6];
	verts[(i32)OPsphericalCubeSide::TOP] = &vertsMem[(i32)OPsphericalCubeSide::TOP * VertsPerSide];
	verts[(i32)OPsphericalCubeSide::BOTTOM] = &vertsMem[(i32)OPsphericalCubeSide::BOTTOM * VertsPerSide];
	verts[(i32)OPsphericalCubeSide::LEFT] = &vertsMem[(i32)OPsphericalCubeSide::LEFT * VertsPerSide];
	verts[(i32)OPsphericalCubeSide::RIGHT] = &vertsMem[(i32)OPsphericalCubeSide::RIGHT * VertsPerSide];
	verts[(i32)OPsphericalCubeSide::FRONT] = &vertsMem[(i32)OPsphericalCubeSide::FRONT * VertsPerSide];
	verts[(i32)OPsphericalCubeSide::BACK] = &vertsMem[(i32)OPsphericalCubeSide::BACK * VertsPerSide];

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

				verts[(i32)OPsphericalCubeSide::BACK][x * LWH + y].pos = pos;
				verts[(i32)OPsphericalCubeSide::BACK][x * LWH + y].uv = OPvec2(x / (float)(LWH - 1), y / (float)(LWH - 1));
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

				verts[(i32)OPsphericalCubeSide::FRONT][x * LWH + y].pos = pos;
				verts[(i32)OPsphericalCubeSide::FRONT][x * LWH + y].uv = OPvec2(x / (float)(LWH - 1), y / (float)(LWH - 1));
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

				verts[(i32)OPsphericalCubeSide::LEFT][x * LWH + y].pos = pos;
				verts[(i32)OPsphericalCubeSide::LEFT][x * LWH + y].uv = OPvec2(x / (float)(LWH - 1), y / (float)(LWH - 1));
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

				verts[(i32)OPsphericalCubeSide::RIGHT][x * LWH + y].pos = pos;
				verts[(i32)OPsphericalCubeSide::RIGHT][x * LWH + y].uv = OPvec2(x / (float)(LWH - 1), y / (float)(LWH - 1));
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

				verts[(i32)OPsphericalCubeSide::TOP][x * LWH + y].pos = pos;
				verts[(i32)OPsphericalCubeSide::TOP][x * LWH + y].uv = OPvec2(x / (float)(LWH - 1), y / (float)(LWH - 1));
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

				verts[(i32)OPsphericalCubeSide::BOTTOM][x * LWH + y].pos = pos;
				verts[(i32)OPsphericalCubeSide::BOTTOM][x * LWH + y].uv = OPvec2(x / (float)(LWH - 1), y / (float)(LWH - 1));
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
	ui32 IndicesPerSide = (((LWH - 1)*(LWH - 1) * 6) / OPSPHERICALCUBE_SCL);
	OPint totalIndices = IndicesPerSide * 6;
	ui16* indicesMem = (ui16*)OPalloc(sizeof(ui16)* totalIndices);

	ui16* indices[6];
	indices[(i32)OPsphericalCubeSide::TOP] = &indicesMem[(i32)OPsphericalCubeSide::TOP * IndicesPerSide];
	indices[(i32)OPsphericalCubeSide::BOTTOM] = &indicesMem[(i32)OPsphericalCubeSide::BOTTOM * IndicesPerSide];
	indices[(i32)OPsphericalCubeSide::LEFT] = &indicesMem[(i32)OPsphericalCubeSide::LEFT * IndicesPerSide];
	indices[(i32)OPsphericalCubeSide::RIGHT] = &indicesMem[(i32)OPsphericalCubeSide::RIGHT * IndicesPerSide];
	indices[(i32)OPsphericalCubeSide::FRONT] = &indicesMem[(i32)OPsphericalCubeSide::FRONT * IndicesPerSide];
	indices[(i32)OPsphericalCubeSide::BACK] = &indicesMem[(i32)OPsphericalCubeSide::BACK * IndicesPerSide];


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

				indices[i][index++] = (ui16)(x*LWH + y);
				indices[i][index++] = (ui16)(x*LWH + y + tscl);
				indices[i][index++] = (ui16)((x + xscl)*LWH + y + tscl);
				indices[i][index++] = (ui16)(x*LWH + y);
				indices[i][index++] = (ui16)((x + xscl)*LWH + y + tscl);
				indices[i][index++] = (ui16)((x + xscl)*LWH + y);
			}
		}
		index = 0;
	}

	for(ui8 face = 0; face < 6; face++) {
		for (OPint x = 0; x < (LWH - 1); x += OPSPHERICALCUBE_SCL) {
			for (OPint y = 0; y < (LWH - 1); y += OPSPHERICALCUBE_SCL) {
				f32 _x = (f32)faces[face].Data[4 * x * LWH + (y + 0) * 4];
				verts[face][y * LWH + x].pos *= 1.0f + (_x - 128.0f) / 255.0f;
			}
		}
	}


	OPsphericalCube result;

	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::NORMAL);
	builder.Add(OPattributes::UV);
	OPvertexLayout vertexLayout = builder.Build();
	for (OPint i = 0; i < 6; i++) {
		sides[i] = OPmesh(vertexLayout);
		sides[i].Build(
			vertexLayout, OPindexSize::SHORT,
				VertsPerSide, IndicesPerSide,
				(void*)verts[i], (void*)indices[i]
		);
	}

	OPfree(vertsMem);
	OPfree(indicesMem);
}

void OPsphericalCube::Init(ui16 size) {

	ui16 LWH = size;
	// Vec 3 Position
	// Vec 3 Normal
	// Vec 2 UV
	ui32 VertsPerSide = LWH * LWH;
	ui32 PerSideSize = sizeof(OPsphericalCubeVertex)* VertsPerSide;
	ui32 TotalVerts = VertsPerSide * 6;
	OPsphericalCubeVertex* vertsMem = (OPsphericalCubeVertex*)OPalloc(PerSideSize * 6);

	OPsphericalCubeVertex* verts[6];
	verts[(i32)OPsphericalCubeSide::TOP] = &vertsMem[(i32)OPsphericalCubeSide::TOP * VertsPerSide];
	verts[(i32)OPsphericalCubeSide::BOTTOM] = &vertsMem[(i32)OPsphericalCubeSide::BOTTOM * VertsPerSide];
	verts[(i32)OPsphericalCubeSide::LEFT] = &vertsMem[(i32)OPsphericalCubeSide::LEFT * VertsPerSide];
	verts[(i32)OPsphericalCubeSide::RIGHT] = &vertsMem[(i32)OPsphericalCubeSide::RIGHT * VertsPerSide];
	verts[(i32)OPsphericalCubeSide::FRONT] = &vertsMem[(i32)OPsphericalCubeSide::FRONT * VertsPerSide];
	verts[(i32)OPsphericalCubeSide::BACK] = &vertsMem[(i32)OPsphericalCubeSide::BACK * VertsPerSide];

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

				verts[(i32)OPsphericalCubeSide::BACK][x * LWH + y].pos = pos;
				verts[(i32)OPsphericalCubeSide::BACK][x * LWH + y].uv = OPvec2(x / (float)(LWH - 1), y / (float)(LWH - 1));
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

				verts[(i32)OPsphericalCubeSide::FRONT][x * LWH + y].pos = pos;
				verts[(i32)OPsphericalCubeSide::FRONT][x * LWH + y].uv = OPvec2(x / (float)(LWH - 1), y / (float)(LWH - 1));
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

				verts[(i32)OPsphericalCubeSide::LEFT][x * LWH + y].pos = pos;
				verts[(i32)OPsphericalCubeSide::LEFT][x * LWH + y].uv = OPvec2(x / (float)(LWH - 1), y / (float)(LWH - 1));
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

				verts[(i32)OPsphericalCubeSide::RIGHT][x * LWH + y].pos = pos;
				verts[(i32)OPsphericalCubeSide::RIGHT][x * LWH + y].uv = OPvec2(x / (float)(LWH - 1), y / (float)(LWH - 1));
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

				verts[(i32)OPsphericalCubeSide::TOP][x * LWH + y].pos = pos;
				verts[(i32)OPsphericalCubeSide::TOP][x * LWH + y].uv = OPvec2(x / (float)(LWH - 1), y / (float)(LWH - 1));
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

				verts[(i32)OPsphericalCubeSide::BOTTOM][x * LWH + y].pos = pos;
				verts[(i32)OPsphericalCubeSide::BOTTOM][x * LWH + y].uv = OPvec2(x / (float)(LWH - 1), y / (float)(LWH - 1));
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
	ui32 IndicesPerSide = (((LWH - 1)*(LWH - 1) * 6) / OPSPHERICALCUBE_SCL);
	OPint totalIndices = IndicesPerSide * 6;
	ui16* indicesMem = (ui16*)OPalloc(sizeof(ui16)* totalIndices);

	ui16* indices[6];
	indices[(i32)OPsphericalCubeSide::TOP] = &indicesMem[(i32)OPsphericalCubeSide::TOP * IndicesPerSide];
	indices[(i32)OPsphericalCubeSide::BOTTOM] = &indicesMem[(i32)OPsphericalCubeSide::BOTTOM * IndicesPerSide];
	indices[(i32)OPsphericalCubeSide::LEFT] = &indicesMem[(i32)OPsphericalCubeSide::LEFT * IndicesPerSide];
	indices[(i32)OPsphericalCubeSide::RIGHT] = &indicesMem[(i32)OPsphericalCubeSide::RIGHT * IndicesPerSide];
	indices[(i32)OPsphericalCubeSide::FRONT] = &indicesMem[(i32)OPsphericalCubeSide::FRONT * IndicesPerSide];
	indices[(i32)OPsphericalCubeSide::BACK] = &indicesMem[(i32)OPsphericalCubeSide::BACK * IndicesPerSide];


	for (OPint i = 0; i < 6; i++) {
		for (ui16 x = 0; x < (LWH - 1); x += OPSPHERICALCUBE_SCL)
		{
			for (ui16 y = 0; y < (LWH - 1); y += OPSPHERICALCUBE_SCL)
			{
				ui16 tscl = OPSPHERICALCUBE_SCL;
				if ((LWH - 1) - y < tscl)
					tscl = (LWH - 1) - y;
				ui16 xscl = OPSPHERICALCUBE_SCL;
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
	
	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::NORMAL);
	builder.Add(OPattributes::UV);
	OPvertexLayout vertexLayout = builder.Build();

	for (OPint i = 0; i < 6; i++) {
		sides[i] = OPmesh(vertexLayout);
		sides[i].Build(
			vertexLayout, OPindexSize::SHORT,
			VertsPerSide, IndicesPerSide,
			(void*)verts[i], (void*)indices[i]
			);
	}

	OPfree(vertsMem);
	OPfree(indicesMem);
}

OPvec2 OPsphericalCube::PlanePositionSide(const OPvec3 pos, OPsphericalCubeSide side) {

	OPvec3 position = OPVEC3_ZERO;
	OPray3D ray = { OPvec3Create(0,0,0), pos };
	OPplane3D plane;
	OPint result;

	switch(side) {

		// Test Top
		case OPsphericalCubeSide::TOP: {
			plane.position = OPvec3Create(0, 0.5, 0);
			plane.normal = OPvec3Create(0, 1, 0);
			result = OPplane3DIntersects(plane, ray, &position);
			return OPvec2(position.x, position.z);
		}

		// Test Bottom
		case OPsphericalCubeSide::BOTTOM: {
			plane.position = OPvec3Create(0, -0.5, 0);
			plane.normal = OPvec3Create(0, -1, 0);
			result = OPplane3DIntersects(plane, ray, &position);
			return OPvec2(-position.x, -position.z);
		}

		// Test Front
		case OPsphericalCubeSide::FRONT: {
			plane.position = OPvec3Create(0, 0, 0.5);
			plane.normal = OPvec3Create(0, 0, 1);
			result = OPplane3DIntersects(plane, ray, &position);
			return OPvec2(position.x, -position.y);
		}

		// Test Back
		case OPsphericalCubeSide::BACK: {
			plane.position = OPvec3Create(0, 0, -0.5);
			plane.normal = OPvec3Create(0, 0, -1);
			result = OPplane3DIntersects(plane, ray, &position);
			return OPvec2(position.x, -position.y);
		}

		// Test Left
		case OPsphericalCubeSide::LEFT: {
			plane.position = OPvec3Create(-0.5, 0, 0);
			plane.normal = OPvec3Create(-1, 0, 0);
			result = OPplane3DIntersects(plane, ray, &position);
			return OPvec2(-position.z, -position.y);
		}

		// Test Right
		case OPsphericalCubeSide::RIGHT: {
			plane.position = OPvec3Create(0.5, 0, 0);
			plane.normal = OPvec3Create(1, 0, 0);
			result = OPplane3DIntersects(plane, ray, &position);
			return OPvec2(position.z, -position.y);
		}

		case OPsphericalCubeSide::_MAX: {
			ASSERT(false, "Should never use the enumeration count of OPsphericalCube");
		}
	}

	return OPVEC2_ZERO;
}

OPvec2 OPsphericalCube::PlanePosition(const OPvec3 pos, OPsphericalCubeSide* side) {

	//ASSERT(pos != OPVEC3_ZERO, "Cannot get position of zero");

	OPvec3 position = OPVEC3_ZERO;
	OPray3D ray = { OPvec3Create(0,0,0), pos };
	OPplane3D plane;
	d64 bounds = OPsqrt(1.0);
	OPint result;

	// Test Top
	{
		plane.position = OPvec3Create(0, 0.5, 0);
		plane.normal = OPvec3Create(0, 1, 0);
		result = OPplane3DIntersects(plane, ray, &position);
		if( result && OPabs(position.x) < 0.5 && OPabs(position.z) < 0.5) {
			(*side) = OPsphericalCubeSide::TOP;
			//OPlg("T");
			return OPvec2(position.x, position.z);
		}
	}

	// Test Bottom
	{
		plane.position = OPvec3Create(0, -0.5, 0);
		plane.normal = OPvec3Create(0, -1, 0);
		result = OPplane3DIntersects(plane, ray, &position);
		if( result && OPabs(position.x) < 0.5 && OPabs(position.z) < 0.5) {
			(*side) = OPsphericalCubeSide::BOTTOM;
			//OPlg("B");
			return OPvec2(-position.x, -position.z);
		}
	}

	// Test Front
	{
		plane.position = OPvec3Create(0, 0, 0.5);
		plane.normal = OPvec3Create(0, 0, 1);
		result = OPplane3DIntersects(plane, ray, &position);
		if( result && OPabs(position.x) < 0.5 && OPabs(position.y) < 0.5) {
			(*side) = OPsphericalCubeSide::FRONT;
			//OPlg("F");
			return OPvec2(position.x, -position.y);
		}
	}

	// Test Back
	{
		plane.position = OPvec3Create(0, 0, -0.5);
		plane.normal = OPvec3Create(0, 0, -1);
		result = OPplane3DIntersects(plane, ray, &position);
		if( result && OPabs(position.x) < 0.5 && OPabs(position.y) < 0.5) {
			(*side) = OPsphericalCubeSide::BACK;
			//OPlg("B");
			return OPvec2(position.x, -position.y);
		}
	}

	// Test Left
	{
		plane.position = OPvec3Create(-0.5, 0, 0);
		plane.normal = OPvec3Create(-1, 0, 0);
		result = OPplane3DIntersects(plane, ray, &position);
		if( result && OPabs(position.z) < 0.5 && OPabs(position.y) < 0.5) {
			(*side) = OPsphericalCubeSide::LEFT;
			//OPlg("L");
			return OPvec2(-position.z, -position.y);
		}
	}

	// Test Right
	{
		plane.position = OPvec3Create(0.5, 0, 0);
		plane.normal = OPvec3Create(1, 0, 0);
		result = OPplane3DIntersects(plane, ray, &position);
		if( result && OPabs(position.z) < 0.5 && OPabs(position.y) < 0.5) {
			(*side) = OPsphericalCubeSide::RIGHT;
			//OPlg("R");
			return OPvec2(position.z, -position.y);
		}
	}

	return OPVEC2_ZERO;
}

OPvec3 OPsphericalCube::Position(OPvec3 pos, OPsphericalCubeSide* side)
{
	OPvec3 position = pos;

	d64 x = position.x;
	d64 y = position.y;
	d64 z = position.z;

	OPfloat fx = (OPfloat)OPabs(x);
	OPfloat fy = (OPfloat)OPabs(y);
	OPfloat fz = (OPfloat)OPabs(z);

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
			*side = OPsphericalCubeSide::TOP;
		}
		else // bottom face
		{
			position.y = -1.0f;
			*side = OPsphericalCubeSide::BOTTOM;
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
			*side = OPsphericalCubeSide::RIGHT;
		}
		else // left face
		{
			position.x = -1.0f;
			*side = OPsphericalCubeSide::LEFT;
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
			*side = OPsphericalCubeSide::FRONT;
		}
		else // back face
		{
			position.z = -1.0f;
			*side = OPsphericalCubeSide::BACK;
		}
	}

	return position;
}

void OPsphericalCube::Destroy() {
	for (ui32 i = 0; i < (ui32)OPsphericalCubeSide::_MAX; i++) {
		sides[i].Destroy();
	}
}
