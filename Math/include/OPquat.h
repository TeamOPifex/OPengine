#pragma once

struct OPquat;
typedef struct OPquat OPquat;

extern const OPquat OPQUAT_IDENTITY;

#include "./Math/include/OPvec3.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPmath.h"

#define OPQUAT_AXIS(q) *((OPvec3*)(&q))

struct OPquat {
	OPfloat x, y, z, w;
	OPquat() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 1;
	}

	OPquat(OPfloat x, OPfloat y, OPfloat z, OPfloat w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	OPquat(OPvec3 axis, OPfloat w) {
		this->x = axis.x;
		this->y = axis.y;
		this->z = axis.z;
		this->w = w;
	}

	OPvec3 Rot(OPvec3 p);
	OPquat Lerp(OPquat b, OPfloat p);
	OPquat Slerp(OPquat b, OPfloat p);

	static OPquat CreateRot(OPvec3 axis, OPfloat angle);
	static OPquat CreateLookAt(OPvec3 eye, OPvec3 target);
	static OPvec3 Orthogonal(OPvec3 v);

	OPquat operator=(OPquat vhs) {
		OPmemcpy(this, &vhs, sizeof(OPquat)); return *this;
	}

	inline OPquat operator+(OPquat rhs) {
		return OPquat(
			x + rhs.x,
			y + rhs.y,
			z + rhs.z,
			w + rhs.w);
	}

	inline OPquat operator-(OPquat rhs) {
		return OPquat(
			x - rhs.x,
			y - rhs.y,
			z - rhs.z,
			w - rhs.w);
	}

	OPquat operator*(OPquat rhs);

	inline OPquat* Scl(OPfloat s) {
		this->x *= s;
		this->y *= s;
		this->z *= s;
		this->w *= s;
		return this;
	}

	inline OPfloat AngularDif(OPquat b) {
		OPfloat dot = this->Dot(b);
		return OPacos(dot / this->Len() * b.Len()) * 2;
	}

	inline OPfloat Len() {
		return (OPfloat)OPsqrt(x * x + y * y + z * z + w * w);
	}

	inline OPquat Conj() {
		OPquat out = {
			-x,
			-y,
			-z,
			w
		};

		return out;
	}

	inline OPquat* Norm() {
		*this = *this *  this->Conj();
		return this;
	}

	inline OPfloat Dot(OPquat b) {
		return x * b.x +
			y * b.y +
			z * b.z +
			w * b.w;
	}


	inline OPvec3 Forward()
	{
		return OPvec3Create(2 * (x * z + w * y),
			2 * (y * x - w * x),
			1 - 2 * (x * x + y * y));
	}

	inline OPvec3 Up(OPquat a)
	{
		return OPvec3Create(2 * (x * y - w * z),
			1 - 2 * (x * x + z * z),
			2 * (y * z + w * x));
	}

	inline OPvec3 Right(OPquat a)
	{
		return OPvec3Create(1 - 2 * (y * y + z * z),
			2 * (x * y + w * z),
			2 * (x * z - w * y));
	}

};



// Test methods
inline OPquat OPquatRotationBetween(OPvec3 start, OPvec3 dest) {
	start = OPvec3Norm(start);
	dest = OPvec3Norm(dest);

	f32 cosTheta = OPvec3Dot(start, dest);
	OPvec3 rotationAxis;

	if (cosTheta < -1 + 0.001f) {
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = OPvec3Cross(OPvec3Create(0.0f, 0.0f, 1.0f), start);
		f32 len = OPvec3Len(rotationAxis);
		if ((len * len) < 0.01) { // bad luck, they were parallel, try again!
			rotationAxis = OPvec3Cross(OPvec3Create(1.0f, 0.0f, 0.0f), start);
		}
		rotationAxis = OPvec3Norm(rotationAxis);
		return OPquat(rotationAxis, 180.0f);
	}

	rotationAxis = OPvec3Cross(start, dest);

	f32 s = (f32)OPsqrt((1 + cosTheta) * 2);
	f32 invs = 1 / s;

	return OPquat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}

inline OPquat OPquatRotationBetween2(OPvec3 start, OPvec3 dest) {
	start = OPvec3Norm(start);
	dest = OPvec3Norm(dest);

	if (start.x == -dest.x && start.y == -dest.y && start.z == -dest.z) {
		return OPquat(OPvec3Norm(OPquat::Orthogonal(start)), 0);
	}

	OPvec3 half = OPvec3Norm(start + dest);
	return OPquat(OPvec3Cross(start, half), OPvec3Dot(start, half));
}

inline OPquat OPquatRotationBetween3(OPvec3 start, OPvec3 dest) {
	start = OPvec3Norm(start);
	dest = OPvec3Norm(dest);

	OPvec3 axis = OPvec3Cross(start, dest);
	f32 len = OPvec3Len(axis);
	if (len == 0) {
		return OPquat(0, 0, 0, 1);
	}

	f32 angle = OPasin(len);
	axis *= 1 / len;
	//OPvec3Norm(axis);
	//OPmat4RotY(axis.y)
	return OPquat(axis, angle);
}

inline OPquat OPquatRotationBetween4(OPvec3 from, OPvec3 to)
{
	OPquat result;
	OPvec3 H = OPvec3Norm(from + to);

	result.x = from.y*H.z - from.z*H.y;
	result.y = from.z*H.x - from.x*H.z;
	result.z = from.x*H.y - from.y*H.x;
	result.w = OPvec3Dot(OPvec3Norm(from), H);

	return result;
}

inline OPquat OPquatRotationBetween5(OPvec3 normal) {
	OPvec3 up = OPVEC3_UP;
	normal = OPvec3Norm(normal);
	OPvec3 axis = OPvec3Norm(OPvec3Cross(up, normal));
	axis = OPvec3Norm(axis);

	f32 dot = OPvec3Dot(OPVEC3_UP, normal);
	f32 phi = OPacos(dot);

	//	OPquat result = {
	//		axis.x * OPsin(phi / 2.0f),
	//		axis.y * OPsin(phi / 2.0f),
	//		axis.z * OPsin(phi / 2.0f),
	//		OPcos(phi / 2.0f)
	//	};
	//	return result;
	return OPquat(axis, phi);
}