#ifndef OPENGINE_HUMAN_RENDERER_PRIMITIVES_SPHERICALCUBE
#define OPENGINE_HUMAN_RENDERER_PRIMITIVES_SPHERICALCUBE

#include "./Human/include/Rendering/OPrenderBuffer.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPmeshPacker.h"
#include "./Human/include/Rendering/OPmeshPacked.h"
#include "./Math/include/OPvec2.h"

#define OPSPHERICALCUBE_SCALE 1.0f
#define OPSPHERICALCUBE_SCL 1

enum OPsphericalCubeSide
{
	OP_SPHERICAL_CUBE_SIDE_TOP = 0,
	OP_SPHERICAL_CUBE_SIDE_BOTTOM = 1,
	OP_SPHERICAL_CUBE_SIDE_LEFT = 2,
	OP_SPHERICAL_CUBE_SIDE_RIGHT = 3,
	OP_SPHERICAL_CUBE_SIDE_FRONT = 4,
	OP_SPHERICAL_CUBE_SIDE_BACK = 5,
	_OP_SPHERICAL_CUBE_SIDE_MAX
};

//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/

struct OPsphericalCubeVertex {
	OPvec3 pos;
	OPvec3 norm;
	OPvec2 uv;
};

struct OPsphericalCube {
	OPmesh sides[_OP_SPHERICAL_CUBE_SIDE_MAX];
};


//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _               
// |  __ \                   |  __ \                |  __ \(_)             | | (_)              
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___ 
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/   

OPsphericalCube       OPsphericalCubeCreate(OPint size); 
OPvec3 OPsphericalCubePosition(OPvec3 pos, OPsphericalCubeSide* side);
OPvec2 OPsphericalCubePlanePosition(const OPvec3 pos, OPsphericalCubeSide* side);
void         OPsphericalCubeDestroy(OPsphericalCube* sphericalCube);

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/


#endif

