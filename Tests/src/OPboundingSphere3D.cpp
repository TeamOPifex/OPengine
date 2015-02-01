#include "Tests/include/helpers.h"
#include "Core/include/OPmath.h"
#include "Math/include/OPboundingSphere3D.h"
OP_TEST_INIT

//-----------------------------------------------------------------------------
//    _______        _     ______                      
//   |__   __|      | |   |  ____|                     
//      | | ___  ___| |_  | |__ _   _ _ __   ___ ___   
//      | |/ _ \/ __| __| |  __| | | | '_ \ / __/ __|  
//      | |  __/\__ \ |_  | |  | |_| | | | | (__\__ \_ 
//      |_|\___||___/\__| |_|   \__,_|_| |_|\___|___(_)                                               

OPint boundingSphere3DsShouldCollide(void* args){
	OPboundingSphere3D bs1 = {
		{ 0, 0, 0},
		1.0
	};	

	OPboundingSphere3D bs2 = {
		{ 0.25, 0, 0},
		1.0
	};

	return !OPboundingSphere3DCollision(&bs1, &bs2);
}

OPint boundingSphere3DsShouldNotCollide(void* args){
	OPboundingSphere3D bs1 = {
		{ 0, 0, 0},
		1.0
	};	

	// Right on the edge
	OPboundingSphere3D bs2 = {
		{ 2.000001, 0, 0},
		1.0
	};

	return OPboundingSphere3DCollision(&bs1, &bs2);
}

//-----------------------------------------------------------------------------
//    _______        _     _____                             
//   |__   __|      | |   |  __ \                            
//      | | ___  ___| |_  | |__) |   _ _ __  _ __   ___ _ __ 
//      | |/ _ \/ __| __| |  _  / | | | '_ \| '_ \ / _ \ '__|
//      | |  __/\__ \ |_  | | \ \ |_| | | | | | | |  __/ |   
//      |_|\___||___/\__| |_|  \_\__,_|_| |_|_| |_|\___|_|   

int main(void){
	int result = 0;

	// Run sequence of test function invocations here
	result |= OP_TEST(boundingSphere3DsShouldCollide, "boundingSphere3DsShouldCollide", NULL);
	result |= OP_TEST(boundingSphere3DsShouldNotCollide, "boundingSphere3DsShouldNotCollide", NULL);

	return result; // value to be read by test script
}
