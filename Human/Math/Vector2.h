#pragma once
#include "Core\include\Types.h"

class Vector2{
public:
	Vector2(){
		_x = 0;		_y = 0;
	}
	Vector2(OPfloat s){
		_x = s;		_y = s;
	}
	Vector2(OPfloat x, OPfloat y){
		_x = x;		_y = y;
	}
private:
	// Data Structure 2 floats
	//		32 bit = 2 * 32 = 64 bits or 8 bytes
	//		64 bit = 2 * 64 = 128 bits or 16 bytes
	union 
	{
		struct{OPfloat _x, _y; };
	};
};