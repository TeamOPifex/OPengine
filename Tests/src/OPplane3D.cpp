#include "Tests/include/helpers.h"
#include "Math/OPmath.h"
OP_TEST_INIT

//-----------------------------------------------------------------------------
//    _______        _     ______                      
//   |__   __|      | |   |  ____|                     
//      | | ___  ___| |_  | |__ _   _ _ __   ___ ___   
//      | |/ _ \/ __| __| |  __| | | | '_ \ / __/ __|  
//      | |  __/\__ \ |_  | |  | |_| | | | | (__\__ \_ 
//      |_|\___||___/\__| |_|   \__,_|_| |_|\___|___(_)                                               

OPint funcShouldInteresect(void* args){

	OPvec3 position;
	OPray3D ray = {
		OPvec3Create(0,0,0),
		OPvec3Create(0,1,0)
	};
	OPplane3D plane = {
		OPvec3Create(0,1,0),
		OPvec3Create(0,1,0)
	};

	OPint result = OPplane3DIntersects(plane, ray, &position);
	if(result) {
		OPvec3Log("At", position);
	}

	return !result;

}

OPint funcShouldNotInteresect(void* args){

	OPvec3 position;
	OPray3D ray = {
		OPvec3Create(0,0,0),
		OPvec3Create(1,0,0)
	};
	OPplane3D plane = {
		OPvec3Create(0,1,0),
		OPvec3Create(0,1,0)
	};

	OPint result = OPplane3DIntersects(plane, ray, &position);
	if(result) {
		OPvec3Log("At", position);
	}

	return result;

}

OPint funcShouldInteresect2(void* args){

	OPvec3 position;
	OPray3D ray = {
		OPvec3Create(0,0,0),
		OPvec3Create(-1,1,0.5)
	};
	OPplane3D plane = {
		OPvec3Create(0,0.5,0),
		OPvec3Create(0,1,0)
	};

	OPint result = OPplane3DIntersects(plane, ray, &position);
	if(result) {
		OPvec3Log("At", position);
		OPfloat bounds = OPsqrt(1);
		OPfloat dist = OPsqrt(OPabs(position.x) + OPabs(position.z));
		if( dist <= bounds) {
			OPlog("%f Within the bounds %f", dist, bounds);
		} else {
			OPlog("%f Outside of the bounds %f", dist, bounds);
		}
	}

	return !result;

}

OPint funcShouldInteresect3(void* args){

	OPvec3 position;
	OPray3D ray = {
		OPvec3Create(0,0,0),
		OPvec3Create(1,-1,0)
	};
	OPplane3D plane = {
		OPvec3Create(0,-0.5,0),
		OPvec3Create(0,-1,0)
	};

	OPint result = OPplane3DIntersects(plane, ray, &position);
	if(result) {
		OPvec3Log("At", position);
		OPfloat bounds = OPsqrt(1);
		OPfloat dist = OPsqrt(OPabs(position.x) + OPabs(position.z));
		if( dist <= bounds) {
			OPlog("%f Within the bounds %f", dist, bounds);
		} else {
			OPlog("%f Outside of the bounds %f", dist, bounds);
		}
	}

	return !result;

}

/*
//-----------------------------------------------------------------------------
//    _______        _     _____                             
//   |__   __|      | |   |  __ \                            
//      | | ___  ___| |_  | |__) |   _ _ __  _ __   ___ _ __ 
//      | |/ _ \/ __| __| |  _  / | | | '_ \| '_ \ / _ \ '__|
//      | |  __/\__ \ |_  | | \ \ |_| | | | | | | |  __/ |   
//      |_|\___||___/\__| |_|  \_\__,_|_| |_|_| |_|\___|_|   
*/
int main(void){
	int result = 0;

	// Run sequence of test function invocations here
	result |= OP_TEST(funcShouldInteresect, "funcShouldInteresect", NULL);
	result |= OP_TEST(funcShouldInteresect2, "funcShouldInteresect2", NULL);
	result |= OP_TEST(funcShouldInteresect3, "funcShouldInteresect3", NULL);
	result |= OP_TEST(funcShouldNotInteresect, "funcShouldNotInteresect", NULL);

	return result; // value to be read by test script
}