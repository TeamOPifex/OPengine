#pragma once

#include "Vector3.h"
#include "Vector4.h"

class Matrix4 {
public:
	Matrix4(){ SetIdentity(); }
	Matrix4(Vector4& col0, Vector4& col1, Vector4& col2, Vector4& col3);
	Matrix4(OPfloat c00,OPfloat c01,OPfloat c02,OPfloat c03,
			OPfloat c10,OPfloat c11,OPfloat c12,OPfloat c13,
			OPfloat c20,OPfloat c21,OPfloat c22,OPfloat c23,
			OPfloat c30,OPfloat c31,OPfloat c32,OPfloat c33);
	
	OPfloat* ptr(){	return cols[0].ptr(); }
	Matrix4 Multiply(Matrix4 m);

	// Helper methods
	void SetIdentity();
	static Matrix4 RotateX(f32 x);
	static Matrix4 RotateY(f32 x);
	static Matrix4 RotateZ(f32 x);
	static Matrix4 Translate(f32 x, f32 y, f32 z);
	static Matrix4 Scale(f32 x);
	static Matrix4 Scale(f32 x, f32 y, f32 z);
	static Matrix4 CreatePerspective(float fovy, float aspect, float near, float far);
	static Matrix4 CreateOrthographic( const OPfloat left, const OPfloat right,
		const OPfloat bottom, const OPfloat top,
	    const OPfloat zNear, const OPfloat zFar );
	static Matrix4 CreateLook(Vector3 campos, Vector3 look, Vector3 up);
	void transpose();
	
	// Operator overloads
	Matrix4& operator*=(const Matrix4 &rhs);
	Vector4& operator[](int idx);
	Matrix4 operator*(Matrix4& right);
	Matrix4 operator*=(Matrix4& right);
private:
	// Data Structure 4 * 4 = 16 floats
	//		32 bit = 16 * 32 = 512 bits or 64 bytes
	//		64 bit = 16 * 64 = 1024 bits or 128 bytes
	Vector4 cols[4];
};

