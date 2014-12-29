#include "Tests/include/helpers.h"
#include "Math/include/Math.h"
OP_TEST_INIT

//-----------------------------------------------------------------------------
//    _______        _     ______                      
//   |__   __|      | |   |  ____|                     
//      | | ___  ___| |_  | |__ _   _ _ __   ___ ___   
//      | |/ _ \/ __| __| |  __| | | | '_ \ / __/ __|  
//      | |  __/\__ \ |_  | |  | |_| | | | | (__\__ \_ 
//      |_|\___||___/\__| |_|   \__,_|_| |_|\___|___(_)                                               

OPint funcShouldInvertMatrix(void* args){

	OPmat4 temp = {
		2, 2, 2, 1,
		2, 1, 0, 2,
		2, 0, 1, 2,
		1, 2, 2, 2
	};

	OPmat4 inverse;
	OPmat4Inverse(&inverse, temp);
	OPmat4Log("Inverse: ", &inverse);

	OPmat4 result = inverse * temp;
	OPmat4Log("Result: ", &result);

	if(result[0][0] == 1 && result[1][1] == 1 && result[2][2] == 1 && result[3][3] == 1){
		return 0;
	}
	else{
		return 1;	
	}
}

OPint funcShouldTransposeMatrix(void* args){

	OPmat4 temp = {
		1, 5, 9, 13,
		2, 6, 10, 14,
		3, 7, 11, 15,
		4, 8, 12, 16
	};
	OPmat4Log("Orig: ", &temp);
	temp = OPmat4Transpose(temp);
	OPmat4Log("Res: ", &temp);

	if(temp[0][3] == 4 && temp[3][0] == 13){
		return 0;
	}
	else{
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
	result |= OP_TEST(funcShouldInvertMatrix, "Should Invert Matrix 4", NULL);
	result |= OP_TEST(funcShouldTransposeMatrix, "Should Transpose Matrix 4", NULL);

	return result; // value to be read by test script
}