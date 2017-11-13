#include "./Human/include/Rendering/Primitives/OPquad.h"

//-----------------------------------------------------------------------------
//   _____ _       _           _
//  / ____| |     | |         | |
// | |  __| | ___ | |__   __ _| |___
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/

OPfloat OPquadVertData[] = {
	 1,  1, 0,
	 1,  1,

	-1,  1, 0,
	 0,  0,

	-1, -1, 0,
	 0,  1,

	 1, -1, 0,
	 1,  0
};

OPfloat OPquadVertNormData[] = {
	 1,  1, 0,
	 0,  0, -1,
	 0,  1,

	-1,  1, 0,
	 0,  0, -1,
	 1,  1,

	-1, -1, 0,
	 0,  0, -1,
	 1,  0,

	 1, -1, 0,
	 0,  0, -1,
	 0,  0
};

ui16 OPquadIndexData[] = {
	2, 1, 0,
	3, 2, 0
};

//-----------------------------------------------------------------------------
// ______                _   _
//|  ____|              | | (_)
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

OPmodel* OPquadCreate(){
	// OPvertexLayoutBuilder builder;
	// builder.Init();
	// builder.Add(OPattributes::POSITION);
	// builder.Add(OPattributes::UV);
	// OPvertexLayout vertexLayout = builder.Build();
	// OPmodel* mesh = OPNEW(OPmodel(1, vertexLayout));
	// mesh->Build(
	// 	4, 6, OPindexSize::SHORT,
	// 	OPquadVertData, OPquadIndexData
	// );
	// return mesh;

    return OPquadCreate(1.0f, 1.0f, OPVEC2_ZERO, OPVEC2_ZERO, OPVEC2_ONE);
}

struct QuadPoint {
	OPfloat x, y, z, u, v;
};

void SetQuadPoint(QuadPoint* point, f32 x, f32 y, f32 z, f32 u, f32 v) {
	point->x = x;
	point->y = y;
	point->z = z;
	point->u = u;
	point->v = v;
}

OPmodel* OPquadCreate(OPfloat width, OPfloat height) {
	return OPquadCreate(width, height, OPVEC2_ZERO, OPVEC2_ZERO, OPVEC2_ONE);
}

OPmodel* OPquadCreate(OPfloat width, OPfloat height, OPvec2 offset) {
	return OPquadCreate(width, height, offset, OPVEC2_ZERO, OPVEC2_ONE);
}
OPmodel* OPquadCreate(OPfloat width, OPfloat height, OPvec2 texcoordStart, OPvec2 texcoordEnd) {
	return OPquadCreate(width, height, OPVEC2_ZERO, texcoordStart, texcoordEnd);
}

OPmodel* OPquadCreate(OPfloat width, OPfloat height, OPvec2 offset, OPvec2 texcoordStart, OPvec2 texcoordEnd) {
	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::UV);
	OPvertexLayout layout = builder.Build();
	OPmodel* mesh = OPNEW(OPmodel(1, layout));
	mesh->Bind();

	// 1, 1,
	// 0, 1,
	// 0, 0,
	// 1, 0

	// 0.5, 0.0
	// 1.0, 1.0
	QuadPoint verts[4];
	SetQuadPoint(&verts[0], offset.x - width, offset.y + height, 0, texcoordStart.x, texcoordStart.y);
	SetQuadPoint(&verts[1], offset.x + width, offset.y + height, 0, texcoordEnd.x, texcoordStart.y);
	SetQuadPoint(&verts[2], offset.x + width, offset.y - height, 0, texcoordEnd.x, texcoordEnd.y);
	SetQuadPoint(&verts[3], offset.x - width, offset.y - height, 0, texcoordStart.x, texcoordEnd.y);

    OPlogErr("OFFSET: %f, %f", offset.x, offset.y);
    OPlogErr("QUAD: %f, %f", verts[0].x, verts[0].y);
    OPlogErr("QUAD: %f, %f", verts[1].x, verts[1].y);
    OPlogErr("QUAD: %f, %f", verts[2].x, verts[2].y);
    OPlogErr("QUAD: %f, %f", verts[3].x, verts[3].y);

	mesh->Build(
		4, 6, OPindexSize::SHORT,
		verts, OPquadIndexData
		);

	return mesh;
}

//-----------------------------------------------------------------------------
//OPmeshPacked OPquadCreatePacked(){
//	OPvertexLayoutBuilder builder;
//	builder.Init();
//	builder.Add(OPattributes::POSITION);
//	builder.Add(OPattributes::UV);
//	OPvertexLayout vertexLayout = builder.Build();
//	OPmeshPacked result = OPmeshPacked(
//		vertexLayout, OPindexSize::SHORT,
//		4, 6,
//		OPquadVertData, OPquadIndexData);
//	return result;
//}
//-----------------------------------------------------------------------------
OPmodel* OPquadNormCreate(){
	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::NORMAL);
	builder.Add(OPattributes::UV);
	OPvertexLayout vertexLayout = builder.Build();
	OPmodel* mesh = OPNEW(OPmodel(1, vertexLayout));
	mesh->Build(
		4, 6, OPindexSize::SHORT,
		OPquadVertNormData, OPquadIndexData
	);
	return mesh;
}
//-----------------------------------------------------------------------------
//OPmeshPacked OPquadNormCreatePacked(){
//
//	OPvertexLayoutBuilder builder;
//	builder.Init();
//	builder.Add(OPattributes::POSITION);
//	builder.Add(OPattributes::NORMAL);
//	builder.Add(OPattributes::UV);
//	OPvertexLayout vertexLayout = builder.Build();
//	return OPmeshPacked(
//		vertexLayout, OPindexSize::SHORT,
//		4, 6,
//		OPquadVertNormData, OPquadIndexData
//	);
//}
//-----------------------------------------------------------------------------
void OPquadDestroy(OPmesh* quad){

}


OPmodel* OPquadCreateZPlane() {
	return OPquadCreateZPlane(1, 1, OPvec2(0, 0), OPvec2(1, 1));
}
OPmodel* OPquadCreateZPlane(OPfloat width, OPfloat depth) {
	return OPquadCreateZPlane(width, depth, OPvec2(0, 0), OPvec2(1, 1));
}

OPmodel* OPquadCreateZPlane(OPfloat width, OPfloat depth, OPvec2 texcoordStart, OPvec2 texcoordEnd, ui32 features) {
	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	if ((features & (ui32)OPattributes::NORMAL) > 0) {
		builder.Add(OPattributes::NORMAL);
	}
	if ((features & (ui32)OPattributes::TANGENT) > 0) {
		builder.Add(OPattributes::TANGENT);
	}
	if ((features & (ui32)OPattributes::BITANGENT) > 0) {
		builder.Add(OPattributes::BITANGENT);
	}
	if ((features & (ui32)OPattributes::UV) > 0) {
		builder.Add(OPattributes::UV);
	}
	OPvertexLayout layout = builder.Build();
	OPmodel* mesh = OPNEW(OPmodel(1, layout));
	mesh->Bind();

	f32* verts = (f32*)OPalloc(sizeof(f32) * layout.stride * 4);
	ui32 offset = 0;

	verts[offset++] = -width; verts[offset++] = 0; verts[offset++] = depth;
	if ((features & (ui32)OPattributes::NORMAL) > 0) {
		verts[offset++] = 0; verts[offset++] = 1; verts[offset++] = 0;
	}
	if ((features & (ui32)OPattributes::TANGENT) > 0) {
		verts[offset++] = 0; verts[offset++] = 0; verts[offset++] = 1;
	}
	if ((features & (ui32)OPattributes::BITANGENT) > 0) {
		verts[offset++] = 1; verts[offset++] = 0; verts[offset++] = 0;
	}
	if ((features & (ui32)OPattributes::UV) > 0) {
		verts[offset++] = texcoordStart.x; verts[offset++] = texcoordStart.y;
	}

	verts[offset++] = width; verts[offset++] = 0; verts[offset++] = depth;
	if ((features & (ui32)OPattributes::NORMAL) > 0) {
		verts[offset++] = 0; verts[offset++] = 1; verts[offset++] = 0;
	}
	if ((features & (ui32)OPattributes::TANGENT) > 0) {
		verts[offset++] = 0; verts[offset++] = 0; verts[offset++] = 1;
	}
	if ((features & (ui32)OPattributes::BITANGENT) > 0) {
		verts[offset++] = 1; verts[offset++] = 0; verts[offset++] = 0;
	}
	if ((features & (ui32)OPattributes::UV) > 0) {
		verts[offset++] = texcoordEnd.x; verts[offset++] = texcoordStart.y;
	}

	verts[offset++] = width; verts[offset++] = 0; verts[offset++] = -depth;
	if ((features & (ui32)OPattributes::NORMAL) > 0) {
		verts[offset++] = 0; verts[offset++] = 1; verts[offset++] = 0;
	}
	if ((features & (ui32)OPattributes::TANGENT) > 0) {
		verts[offset++] = 0; verts[offset++] = 0; verts[offset++] = 1;
	}
	if ((features & (ui32)OPattributes::BITANGENT) > 0) {
		verts[offset++] = 1; verts[offset++] = 0; verts[offset++] = 0;
	}
	if ((features & (ui32)OPattributes::UV) > 0) {
		verts[offset++] = texcoordEnd.x; verts[offset++] = texcoordEnd.y;
	}

	verts[offset++] = -width; verts[offset++] = 0; verts[offset++] = -depth;
	if ((features & (ui32)OPattributes::NORMAL) > 0) {
		verts[offset++] = 0; verts[offset++] = 1; verts[offset++] = 0;
	}
	if ((features & (ui32)OPattributes::TANGENT) > 0) {
		verts[offset++] = 0; verts[offset++] = 0; verts[offset++] = 1;
	}
	if ((features & (ui32)OPattributes::BITANGENT) > 0) {
		verts[offset++] = 1; verts[offset++] = 0; verts[offset++] = 0;
	}
	if ((features & (ui32)OPattributes::UV) > 0) {
		verts[offset++] = texcoordStart.x; verts[offset++] = texcoordEnd.y;
	}

	mesh->Build(
		4, 6, OPindexSize::SHORT,
		verts, OPquadIndexData
	);

	return mesh;
}

OPmodel* OPquadCreateZPlane(OPfloat width, OPfloat depth, OPvec2 texcoordStart, OPvec2 texcoordEnd) {
/*
	QuadPoint verts[4] = {
		{ -width, 0, -depth, texcoordEnd.x, texcoordEnd.y },
		{ width, 0, -depth, texcoordStart.x, texcoordEnd.y },
		{ width, 0, depth, texcoordStart.x, texcoordStart.y },
		{ -width, 0, depth, texcoordEnd.x, texcoordStart.y }
	};
	ui16 indicies[6] = {
		2, 1, 0,
		3, 2, 0
	};

	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::UV);
	OPvertexLayout vertexLayout = builder.Build();
	OPmodel* mesh = OPNEW(OPmodel(1, vertexLayout));
	mesh->Build(
		4, 6, OPindexSize::SHORT,
		verts, indicies
		);

	mesh->vertexLayout.stride = sizeof(OPfloat) * 5;

	return mesh;*/




	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::UV);
	OPvertexLayout layout = builder.Build();
	OPmodel* mesh = OPNEW(OPmodel(1, layout));
	mesh->Bind();

	// 1, 1,
	// 0, 1,
	// 0, 0,
	// 1, 0

	// 0.5, 0.0
	// 1.0, 1.0
	QuadPoint verts[4];
	SetQuadPoint(&verts[0], -width, 0, depth, texcoordStart.x, texcoordStart.y);
	SetQuadPoint(&verts[1], width, 0, depth, texcoordEnd.x, texcoordStart.y);
	SetQuadPoint(&verts[2], width, 0, -depth, texcoordEnd.x, texcoordEnd.y);
	SetQuadPoint(&verts[3], -width, 0, -depth, texcoordStart.x, texcoordEnd.y);

	mesh->Build(
		4, 6, OPindexSize::SHORT,
		verts, OPquadIndexData
	);

	return mesh;
}


OPmodel* OPquadCreate(OPfloat width, OPfloat depth, ui32 segmentsWidth, ui32 segmentsDepth) {
	if (segmentsWidth == 0 || segmentsDepth == 0) {
		return NULL;
	}

	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::UV);
	OPvertexLayout layout = builder.Build();
	OPmodel* mesh = OPNEW(OPmodel(1, layout));
	mesh->Bind();
	

	ui32 totalVertices = (segmentsWidth + 1) * (segmentsDepth + 1);
	QuadPoint* vertices = OPALLOC(QuadPoint, totalVertices);

	f32 halfWidth = width / 2.0f;
	f32 halfDepth = depth / 2.0f;

	OPfloat w = segmentsWidth;
	OPfloat d = segmentsDepth;

	for (ui32 i = 0; i < segmentsWidth + 1; i++) {
		for (ui32 j = 0; j < segmentsDepth + 1; j++) {

			ui64 offset = j * segmentsWidth + i;
			QuadPoint* qp = &vertices[offset];
			
			qp->x = -halfWidth + ((j / (OPfloat)segmentsWidth) * width);
			qp->y = 0;
			qp->z = -halfDepth + ((i / (OPfloat)segmentsDepth) * depth);

			qp->u = (OPfloat)j / d;
			qp->v = (OPfloat)i / w;
		}
	}

	ui32 totalIndices = (segmentsWidth) * (segmentsDepth) * 6;
	ui16* indices = (ui16*)OPalloc(sizeof(ui16) * totalIndices);

	ui32 off = 0;
	for (ui32 i = 0; i < segmentsWidth; i++) {
		for (ui32 j = 0; j < segmentsDepth; j++) {

			indices[off++] = (j + 1) * (segmentsDepth) + i + 1;
			indices[off++] = (j + 1) * (segmentsDepth) + i + 0;
			indices[off++] = j * (segmentsDepth) + i + 0;


			indices[off++] = j * (segmentsDepth) + i + 1;
			indices[off++] = (j + 1) * (segmentsDepth) + i + 1;
			indices[off++] = j * (segmentsDepth) + i + 0;
		}
	}

	mesh->Build(
		totalVertices, totalIndices, OPindexSize::SHORT,
		vertices, indices
	);

	OPfree(vertices);
	OPfree(indices);

	return mesh;
}
