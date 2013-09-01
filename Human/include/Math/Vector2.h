#pragma once
#include "Core/include/Types.h"

#define vec2add(dst, a, b){\
	dst.x = a.x + b.x;\
	dst.y = a.y + b.y;\
}\

#define vec2sub(dst, a, b){\
	dst.x = a.x - b.x;\
	dst.y = a.y - b.y;\
}\

#define vec2mul(dst, a, b){\
	dst.x = a.x * b.x;\
	dst.y = a.y * b.y;\
}\

#define vec2scl(dst, s){\
	dst.x *= s;\
	dst.y *= s;\
}\

#define vec2div(dst, a, b){\
	dst.x = a.x / b.x;\
	dst.y = a.y / b.y;\
}\

#define vec2dot(dst, a, b){\
	dst = a.x * b.x + a.y * b.y;\
}\

#define vec2norm(dst, a){\
	OPfloat l = sqrt(a.x * a.x + a.y * a.y);\
	dst.x = a.x / l;\
	dst.y = a.y / l;\
}\

#define vec2perp(dst, a){\
	dst.x = a.y;\
	dst.y = a.x;\
}\

class Vector2{
public:
	Vector2(){
		x = 0;		y = 0;
	}
	Vector2(OPfloat s){
		x = s;		y = s;
	}
	Vector2(OPfloat x, OPfloat y){
		x = x;		y = y;
	}

	//Vector2& operator-(const Vector2 &rhs);
	//Vector2& operator+(const Vector2 &rhs);	
	OPfloat& operator[](int idx);

//private:
	// Data Structure 2 floats
	//		32 bit = 2 * 32 = 64 bits or 8 bytes
	//		64 bit = 2 * 64 = 128 bits or 16 bytes
	union 
	{
		struct{OPfloat x, y; };
	};
};