#pragma once
#include "Vector3.h"

class Matrix3 {
public:
	Matrix3(){	
		SetIdentity();
	}
	void SetIdentity();
private:
	// Data Structure 3 * 3 = 9 floats
	//		32 bit = 9 * 32 = 288 bits or 36 bytes
	//		64 bit = 9 * 64 = 576 bits or 72 bytes
	Vector3 cols[3];
};

