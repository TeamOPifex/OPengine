#pragma once
#include "./Core/include/Types.h"

class Vector4{
public:
	Vector4(){
		x = 0;		y = 0;		z = 0;		w = 0;
	}
	Vector4(OPfloat s){
		x = s;		y = s;		z = s;		w = s;
	}
	Vector4(OPfloat x, OPfloat y, OPfloat z, OPfloat w){
		this->x = x;		this->y = y;		this->z = z;		this->w = w;
	}

	OPfloat* ptr(){	return &x;	}
	
	// Helpers
	Vector4* normalize();
	static Vector4& normalize(Vector4& source);	

	// Operator overloads
	OPfloat& operator[](int idx);
	Vector4* operator*=(const Vector4 &rhs);

	union 
	{
		struct{OPfloat x, y, z, w;};
	};
};
