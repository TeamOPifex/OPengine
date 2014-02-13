#include "./Human/include/Rendering/Primitives/Quad.h"

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
	0, 2, 3, 
	2, 0, 1
};

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPmesh OPquadCreate(){
	OPmesh mesh = OPrenderCreateMesh();
	OPrenderBindMesh(&mesh);
	OPrenderBuildMesh(
		sizeof(OPfloat) * 5, sizeof(ui16),
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

OPmesh OPquadCreateCustom(OPfloat width, OPfloat height, OPvec2 offset, OPvec2 texcoordStart, OPvec2 texcoordEnd) {
	OPmesh mesh = OPrenderCreateMesh();
	OPrenderBindMesh(&mesh);

		//1, 1,
		//0, 1,
		//0, 0,
		//1, 0

	// 0.5, 0.0
	// 1.0, 1.0
	QuadPoint* verts = (QuadPoint*)OPalloc(sizeof(QuadPoint)* 4);
	SetQuadPoint(&verts[0], offset.x + width, offset.y + height, 0, texcoordEnd.x, texcoordEnd.y);
	SetQuadPoint(&verts[1], offset.x - width, offset.y + height, 0, texcoordStart.x, texcoordEnd.y);
	SetQuadPoint(&verts[2], offset.x - width, offset.y - height, 0, texcoordStart.x, texcoordStart.y);
	SetQuadPoint(&verts[3], offset.x + width, offset.y - height, 0, texcoordEnd.x, texcoordStart.y);
	
	OPrenderBuildMesh(
		sizeof(OPfloat)* 5, sizeof(ui16),
		4, 6,
		verts, OPquadIndexData
		);
	return mesh;
}

//-----------------------------------------------------------------------------
OPmeshPacked OPquadCreatePacked(){
	return OPrenderCreateMeshPacked(
		sizeof(OPfloat) * 5, sizeof(ui16),
		4, 6,
		OPquadVertData, OPquadIndexData
	);
}
//-----------------------------------------------------------------------------
OPmesh OPquadNormCreate(){
	OPmesh mesh = OPrenderCreateMesh();
	OPrenderBindMesh(&mesh);
	OPrenderBuildMesh(
		sizeof(OPfloat) * 8, sizeof(ui16),
		4, 6,
		OPquadVertNormData, OPquadIndexData
	);
	return mesh;
}
//-----------------------------------------------------------------------------
OPmeshPacked OPquadNormCreatePacked(){
	return OPrenderCreateMeshPacked(
		sizeof(OPfloat) * 8, sizeof(ui16),
		4, 6,
		OPquadVertNormData, OPquadIndexData
	);
}