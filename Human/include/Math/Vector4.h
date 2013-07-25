#pragma once
#include "./Core/include/Types.h"

class Vector4{
public:
	Vector4(){
		_x = 0;		_y = 0;		_z = 0;		_w = 0;
	}
	Vector4(OPfloat s){
		_x = s;		_y = s;		_z = s;		_w = s;
	}
	Vector4(OPfloat x, OPfloat y, OPfloat z, OPfloat w){
		_x = x;		_y = y;		_z = z;		_w = w;
	}

	OPfloat* ptr(){	return &_x;	}
	
	// Helpers
	Vector4* normalize();
	static Vector4& normalize(Vector4& source);	

	// Operator overloads
	OPfloat& operator[](int idx);
	Vector4* operator*=(const Vector4 &rhs);

	union 
	{
		struct{OPfloat _x, _y, _z, _w;};
	};
};
