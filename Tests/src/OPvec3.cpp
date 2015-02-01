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
inline void addOne(OPvec3 v)
{
	v[0] += 1.0f;
}

OPint funcShouldModifyVector(void* args){

	OPvec3 v = { 0, 0, 0 };
	OPfloat oldx = v[0];
	addOne(v);
	if(oldx != v[0]){
		OP_RTMSG("Vector modified!");
		return 0;
	}
	else{
		OP_RTMSG("Vector unchanged...");
		return 1;	
	}
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
	result |= OP_TEST(funcShouldModifyVector, "TestInline", NULL);

	return result; // value to be read by test script
}