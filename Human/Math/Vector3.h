#pragma once
#include "./Core/include/Types.h"

class Vector3{
public:
	Vector3(){
		_x = 0;		_y = 0;		_z = 0;
	}
	Vector3(OPfloat s){
		_x = s;		_y = s;		_z = s;
	}
	Vector3(OPfloat x, OPfloat y, OPfloat z){
		_x = x;		_y = y;		_z = z;
	}
	
	OPfloat* ptr(){	return &_x;	}

	// Helpers
	float Length();
	Vector3* Normalize();
	Vector3* Cross(Vector3 right);
	Vector3* Subtract(Vector3 right);
	Vector3* Add(Vector3 right);
	Vector3* Multiply(OPfloat scaler);
	
	static Vector3 Normalize(Vector3 left);
	static Vector3 Cross(Vector3 left, Vector3 right);
	static Vector3 Subtract(Vector3 left, Vector3 right);
	static Vector3 Add(Vector3 left, Vector3 right);
	static Vector3 Multiply(Vector3 left, OPfloat scaler);
	static Vector3 Multiply(Vector3 left, Vector3 right);

	static OPfloat Distance(Vector3& left, Vector3& right);
	static OPfloat Dot(Vector3 left, Vector3 right);

	// Operator Overloads
	Vector3* operator-=(Vector3 &rhs);
	Vector3* operator+=(Vector3 &rhs);	
	OPfloat& operator[](int idx);
//private:
	// Data Structure 3 floats
	//		32 bit = 3 * 32 = 96 bits or 12 bytes
	//		64 bit = 3 * 64 = 192 bits or 24 bytes
	union 
	{
		struct{OPfloat _x, _y, _z;};
	};
};
