#include "Tests/include/helpers.h"
#include "Core/include/MathHelpers.h"
#include "Math/include/OPquat.h"
OP_TEST_INIT

//-----------------------------------------------------------------------------
//    _______        _     ______                      
//   |__   __|      | |   |  ____|                     
//      | | ___  ___| |_  | |__ _   _ _ __   ___ ___   
//      | |/ _ \/ __| __| |  __| | | | '_ \ / __/ __|  
//      | |  __/\__ \ |_  | |  | |_| | | | | (__\__ \_ 
//      |_|\___||___/\__| |_|   \__,_|_| |_|\___|___(_)                                               
OPint quaternionShallRotateVector(void* args){
	OPvec3 yAxis = { 0, 1, 0 };
	OPvec3 p = { 1, 0, 0 };
	OPquat rot90onY = OPquatCreateRot(&yAxis, -OPpi / 2);

	OPlog("\tq = (%f, %f, %f, %f)", rot90onY.x, rot90onY.y, rot90onY.z, rot90onY.w);

	OPvec3 rotated = OPquatRot(&rot90onY, &p);
	OPvec3 expectedRotation =  { 0, 0, 1 };

	OPvec3log("\tp = ", &p);
	OPvec3log("\tp * q = ", &rotated);

	for(OPint i = 3; i--;){
		OPfloat r = ((float*)&rotated)[i];
		OPfloat e = ((float*)&expectedRotation)[i];

		// bias for floating point nonsense
		if(!OPnear(r, e, 0.0001f)){
			OP_RTMSG("Rotated p does not match expected!");
			return -1;
		}
	}

	OP_RTMSG("Rotation successful");

	return 0;
}

OPint quaternionsDotedShouldReturnZero(void* args){
	OPvec3 yAxis = { 0, 1, 0 };

	OPquat q1 = { 0, 1, 0, 0 };
	OPquat q2 = { 1, 0, 0, 0 };

	OP_RTMSG("q1 = (%f, %f, %f, %f)", q1.x, q1.y, q1.z, q1.w);
	OP_RTMSG("q2 = (%f, %f, %f, %f)", q2.x, q2.y, q2.z, q2.w);

	OPfloat dot = OPquatDot(&q1, &q2);

	OP_RTMSG("q1 • q2 = %f", dot);

	if(!OPnear(dot, 0.0f, 0.0001f)){
		OP_RTMSG("Dot does not match expected 0.0!");
		return -1;
	}

	OP_RTMSG("Dot computed successfully!");

	return 0;
}

OPint quaternionsAngleExpectedPiOver2(void* args){
	OPvec3 yAxis = { 0, 1, 0 };

	OPquat q1 = OPquatCreateRot(&yAxis, OPpi / 4);
	OPquat q2 = OPquatCreateRot(&yAxis, OPpi / 2);

	OP_RTMSG("q1 = (%f, %f, %f, %f)", q1.x, q1.y, q1.z, q1.w);
	OP_RTMSG("q2 = (%f, %f, %f, %f)", q2.x, q2.y, q2.z, q2.w);

	OPfloat ang = OPquatAngularDif(&q1, &q2);

	OP_RTMSG("OPquatAngularDif(q1, q2) = %f", ang);

	if(!OPnear(ang, OPpi / 4, 0.0001f)){
		OP_RTMSG("Angle does not match expected %f!", OPpi / 4);
		return -1;
	}

	OP_RTMSG("Dot computed successfully!");

	return 0;
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
	result |= OP_TEST(quaternionShallRotateVector, "quaternionShallRotateVector", NULL);
	result |= OP_TEST(quaternionsDotedShouldReturnZero, "quaternionsDotedShouldReturnOne", NULL);
	result |= OP_TEST(quaternionsAngleExpectedPiOver2, "quaternionsAngleExpectedPiOver2", NULL);

	return result; // value to be read by test script
}
