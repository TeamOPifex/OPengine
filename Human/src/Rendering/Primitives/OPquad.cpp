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

OPmesh OPquadCreate(){
	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::UV);
	OPvertexLayout vertexLayout = builder.Build();
	OPmesh mesh = OPmeshCreate(vertexLayout);
	mesh.Bind();
	OPmeshBuild(
		vertexLayout, OPindexSize::SHORT,
		4, 6,
		OPquadVertData, OPquadIndexData
	);
	return mesh;
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

OPmesh OPquadCreate(OPfloat width, OPfloat height) {
	return OPquadCreate(width, height, OPvec2(0, 0), OPvec2(0, 0), OPvec2(1, 1));
}

OPmesh OPquadCreate(OPfloat width, OPfloat height, OPvec2 offset) {
	return OPquadCreate(width, height, offset, OPvec2(0, 0), OPvec2(1, 1));
}
OPmesh OPquadCreate(OPfloat width, OPfloat height, OPvec2 texcoordStart, OPvec2 texcoordEnd) {
	return OPquadCreate(width, height, OPvec2(0, 0), texcoordStart, texcoordEnd);
}

OPmesh OPquadCreate(OPfloat width, OPfloat height, OPvec2 offset, OPvec2 texcoordStart, OPvec2 texcoordEnd) {
	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::UV);
	OPmesh mesh = OPmeshCreate(builder.Build());
	mesh.Bind();

	// 1, 1,
	// 0, 1,
	// 0, 0,
	// 1, 0

	// 0.5, 0.0
	// 1.0, 1.0
	QuadPoint* verts = (QuadPoint*)OPalloc(sizeof(QuadPoint)* 4);
	SetQuadPoint(&verts[0], offset.x + width, offset.y + height, 0, texcoordEnd.x, texcoordEnd.y);
	SetQuadPoint(&verts[1], offset.x - width, offset.y + height, 0, texcoordStart.x, texcoordEnd.y);
	SetQuadPoint(&verts[2], offset.x - width, offset.y - height, 0, texcoordStart.x, texcoordStart.y);
	SetQuadPoint(&verts[3], offset.x + width, offset.y - height, 0, texcoordEnd.x, texcoordStart.y);

	OPmeshBuild(
		mesh.vertexLayout, OPindexSize::SHORT,
		4, 6,
		verts, OPquadIndexData
		);
	return mesh;
}

//-----------------------------------------------------------------------------
OPmeshPacked OPquadCreatePacked(){
	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::UV);
	OPvertexLayout vertexLayout = builder.Build();
	return OPmeshPackedCreate(
		vertexLayout, OPindexSize::SHORT,
		4, 6,
		OPquadVertData, OPquadIndexData
	);
}
//-----------------------------------------------------------------------------
OPmesh OPquadNormCreate(){
	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::NORMAL);
	builder.Add(OPattributes::UV);
	OPvertexLayout vertexLayout = builder.Build();
	OPmesh mesh = OPmeshCreate(vertexLayout);
	mesh.Bind();
	OPmeshBuild(
		vertexLayout, OPindexSize::SHORT,
		4, 6,
		OPquadVertNormData, OPquadIndexData
	);
	return mesh;
}
//-----------------------------------------------------------------------------
OPmeshPacked OPquadNormCreatePacked(){

	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::NORMAL);
	builder.Add(OPattributes::UV);
	OPvertexLayout vertexLayout = builder.Build();
	return OPmeshPackedCreate(
		vertexLayout, OPindexSize::SHORT,
		4, 6,
		OPquadVertNormData, OPquadIndexData
	);
}
//-----------------------------------------------------------------------------
void OPquadDestroy(OPmesh* quad){
	
}


OPmesh OPquadCreateZPlane() {
	return OPquadCreateZPlane(1, 1, OPvec2(0, 0), OPvec2(1, 1));
}
OPmesh OPquadCreateZPlane(OPfloat width, OPfloat depth) {
	return OPquadCreateZPlane(width, depth, OPvec2(0, 0), OPvec2(1, 1));
}
OPmesh OPquadCreateZPlane(OPfloat width, OPfloat depth, OPvec2 texcoordStart, OPvec2 texcoordEnd) {

	QuadPoint verts[4] = {
		{ -width, 0, -depth, texcoordEnd.x, texcoordEnd.y },
		{ width, 0, -depth, texcoordStart.x, texcoordEnd.y },
		{ width, 0, depth, texcoordStart.x, texcoordStart.y },
		{ -width, 0, depth, texcoordEnd.x, texcoordStart.y }
	};
	ui16 indicies[6] = {
		0, 1, 2,
		0, 2, 3
	};

	OPvertexLayoutBuilder builder;
	builder.Init();
	builder.Add(OPattributes::POSITION);
	builder.Add(OPattributes::UV);
	OPvertexLayout vertexLayout = builder.Build();
	OPmesh mesh = OPmeshCreate(vertexLayout);
	mesh.Bind();
	OPmeshBuild(
		vertexLayout, OPindexSize::SHORT,
		4, 6,
		verts, indicies
		);
	
	mesh.vertexLayout.stride = sizeof(OPfloat) * 5;

	return mesh;
}