#pragma once
#include "./Core/include/Types.h"

#define vec3add(dst, a, b){\
	dst.x = a.x + b.x;\
	dst.y = a.y + b.y;\
	dst.z = a.z + b.z;\
}\

#define vec3sub(dst, a, b){\
	dst.x = a.x - b.x;\
	dst.y = a.y - b.y;\
	dst.z = a.z - b.z;\
}\

#define vec3mul(dst, a, b){\
	dst.x = a.x * b.x;\
	dst.y = a.y * b.y;\
	dst.z = a.z * b.z;\
}\

#define vec3scl(dst, s){\
	dst.x *= s;\
	dst.y *= s;\
	dst.z *= s;\
}\

#define vec3div(dst, a, b){\
	dst.x = a.x / b.x;\
	dst.y = a.y / b.y;\
	dst.z = a.z / b.z;\
}\

#define vec3dot(dst, a, b){\
	dst = a.x * b.x + a.y * b.y + a.z * b.z;\
}\

#define vec3norm(dst, a){\
	OPfloat l = sqrt(a.x * a.x + a.y * a.y);\
	dst.x = a.x / l;\
	dst.y = a.y / l;\
	dst.z = a.z / l;\
}\

// TODO
#define vec3cross(dst, a, b){\
	dst.x = a.y;\
	dst.y = a.x;\
}\

class Vector3{
public:
	Vector3(){
		x = 0;		y = 0;		z = 0;
	}
	Vector3(OPfloat s){
		x = s;		y = s;		z = s;
	}
	Vector3(OPfloat x, OPfloat y, OPfloat z){
		this->x = x;		this->y = y;		this->z = z;
	}
	
	OPfloat* ptr(){	return &x;	}

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
		struct{OPfloat x, y, z;};
	};
};
