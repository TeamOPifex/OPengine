#include "./Human/include/Rendering/OPquad.h"

//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
OPfloat OPquadVertData[] = {
	 1,  1, 0,
	 0,  1,

	-1,  1, 0,
	 1,  1,

	-1, -1, 0,
	 1,  0,

	 1, -1, 0,
	 0,  0,   1
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
		sizeof(OPfloat) * 6, sizeof(ui16),
		4, 6,
		OPquadVertData, OPquadIndexData
	);
	return mesh;
}
//-----------------------------------------------------------------------------
OPmeshPacked OPquadCreatePacked(){
	return OPrenderCreateMeshPacked(
		sizeof(OPfloat) * 6, sizeof(ui16),
		4, 6,
		OPquadVertData, OPquadIndexData
	);
}