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
	 0,  1,

	-1, -1, 0,
	 0,  0,

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
	0, 1, 2,
	0, 2, 3
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
OPmodel* OPquadCreateZPlane(OPfloat width, OPfloat depth, OPvec2 texcoordStart, OPvec2 texcoordEnd) {

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

	return mesh;
}
