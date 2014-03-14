#include "Tests/include/helpers.h"
OP_TEST_INIT

//-----------------------------------------------------------------------------
//    _______        _     ______                      
//   |__   __|      | |   |  ____|                     
//      | | ___  ___| |_  | |__ _   _ _ __   ___ ___   
//      | |/ _ \/ __| __| |  __| | | | '_ \ / __/ __|  
//      | |  __/\__ \ |_  | |  | |_| | | | | (__\__ \_ 
//      |_|\___||___/\__| |_|   \__,_|_| |_|\___|___(_)                                               
OPint passingTest(void* args){
	OP_RTMSG("I passed! So %d", *((int*)args));
	return 0;
}
//-----------------------------------------------------------------------------
OPint failingTest(void* args){
	OP_RTMSG("I suck, not even going to try.");
	return 1;
}

//-----------------------------------------------------------------------------
//    _______        _     _____                             
//   |__   __|      | |   |  __ \                            
//      | | ___  ___| |_  | |__) |   _ _ __  _ __   ___ _ __ 
//      | |/ _ \/ __| __| |  _  / | | | '_ \| '_ \ / _ \ '__|
//      | |  __/\__ \ |_  | | \ \ |_| | | | | | | |  __/ |   
//      |_|\___||___/\__| |_|  \_\__,_|_| |_|_| |_|\___|_|   
OPint main(void){
	OPint result = 0;
	OPint leet = 1337;

	// Run sequence of test function invocations here
	result |= OP_TEST(passingTest, "PassingTest", (void*)&leet);
	result |= OP_TEST(failingTest, "FailingTest", NULL);

	return result; // value to be read by test script
}