#pragma once

struct OPquat;
typedef struct OPquat OPquat;

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
};

extern const OPquat OPQUAT_IDENTITY;

inline OPquat OPquatScl(OPquat a, OPfloat s);
inline OPquat OPquatConj(OPquat a);
inline OPquat OPquatNorm(OPquat a);
inline OPfloat OPquatLen(OPquat a);
inline OPfloat OPquatDot(OPquat a, OPquat b);
inline OPfloat OPquatAngularDif(OPquat a, OPquat b);
inline OPquat OPquatCreateRot(OPvec3 axis, OPfloat angle);
inline OPquat OPquatCreateLookAt(OPvec3 eye, OPvec3 target);
inline OPvec3 OPquatRot(OPquat q, OPvec3 v);


inline OPquat OPquatScl(OPquat a, OPfloat s){
	OPquat out = {
		a.x * s,
		a.y * s,
		a.z * s,
		a.w * s
	};

	return out;
}

inline OPquat OPquatConj(OPquat a){
	OPquat out = {
		-a.x,
		-a.y,
		-a.z,
		a.w
	};

	return out;
}

inline OPquat OPquatNorm(OPquat a){
	OPquat conj = OPquatConj(a);
	return a * conj;
}

inline OPfloat OPquatDot(OPquat a, OPquat b){
	return a.x * b.x +
	       a.y * b.y +
	       a.z * b.z +
	       a.w * b.w;
}

inline OPvec3 OPquatForward(OPquat a)
{
	return OPvec3Create(2 * (a.x * a.z + a.w * a.y),
		2 * (a.y * a.x - a.w * a.x),
		1 - 2 * (a.x * a.x + a.y * a.y));
}

inline OPvec3 OPquatUp(OPquat a)
{
	return OPvec3Create(2 * (a.x * a.y - a.w * a.z),
		1 - 2 * (a.x * a.x + a.z * a.z),
		2 * (a.y * a.z + a.w * a.x));
}

inline OPvec3 OPquatRight(OPquat a)
{
	return OPvec3Create(1 - 2 * (a.y * a.y + a.z * a.z),
		2 * (a.x * a.y + a.w * a.z),
		2 * (a.x * a.z - a.w * a.y));
}

inline OPvec3 OPquatRot(OPquat q, OPvec3 p){
	OPvec3 qp( q.x, q.y, q.z );
	// OPquat conj = OPquatConj(q);
	// conj = OPquatMul(&qp, &conj);
	// OPquat rot = OPquatMul(q, &conj);
	// OPvec3 out = { rot.x, rot.y, rot.z };

	OPvec3 c1 = OPvec3Cross(p, qp) + p * q.w;
	OPvec3 c2 = OPvec3Cross(c1, qp);

	return p + c2 * 2.0;
}

inline OPfloat OPquatAngularDif(OPquat a, OPquat b){
	OPfloat dot = OPquatDot(a, b);
	return OPacos(dot / OPquatLen(a) * OPquatLen(b)) * 2;
}

inline OPquat OPquatCreateRot(OPvec3 axis, OPfloat angle){
	OPfloat c = OPcos(0.5f * angle), s = OPsin(0.5f * angle);
	OPvec3 n = OPvec3Norm((OPvec3)axis);
	OPvec3 v = n * s;

	OPquat out = {
		v.x,
		v.y,
		v.z,
		c
	};

	return out;
}

inline OPquat OPquatCreateLookAt(OPvec3 eye, OPvec3 target){
	OPvec3 forward = eye - target;
	OPvec3 forwardUnit = OPvec3Norm(forward);

	OPfloat dot = OPvec3Dot(forwardUnit, OPVEC3_FORWARD);

	if(OPabs(dot + 1.0f) < 0.00001f){
		return OPquat(OPVEC3_UP, OPpi);
	}
	else if(OPabs(dot - 1.0f) < 0.00001f){
		return OPQUAT_IDENTITY;
	}

	OPfloat rotAngle = OPacos(dot);

	OPvec3 rotAxis = OPvec3Cross(forwardUnit, OPVEC3_FORWARD);
	rotAxis = OPvec3Norm(rotAxis);

	return OPquatCreateRot(rotAxis, rotAngle);
}

inline OPquat OPquatRotationBetween(OPvec3 start, OPvec3 dest) {
	start = OPvec3Norm(start);
	dest = OPvec3Norm(dest);

	f32 cosTheta = OPvec3Dot(start, dest);
	OPvec3 rotationAxis;

	if (cosTheta < -1 + 0.001f){
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = OPvec3Cross(OPvec3Create(0.0f, 0.0f, 1.0f), start);
		f32 len = OPvec3Len(rotationAxis);
		if ((len * len) < 0.01 ) { // bad luck, they were parallel, try again!
			rotationAxis = OPvec3Cross(OPvec3Create(1.0f, 0.0f, 0.0f), start);
		}
		rotationAxis = OPvec3Norm(rotationAxis);
		return OPquat(rotationAxis, 180.0f);
	}

	rotationAxis = OPvec3Cross(start, dest);

	f32 s = (f32)OPsqrt( (1+cosTheta)*2 );
	f32 invs = 1 / s;

	return OPquat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}

inline OPvec3 OPvec3Orthogonal(OPvec3 v) {
	f32 x = OPabs(v.x);
	f32 y = OPabs(v.y);
	f32 z = OPabs(v.z);
	OPvec3 other = x < y ? (x < z ? OPvec3Create(1,0,0) : OPvec3Create(0,0,1)) : (y < z ? OPvec3Create(0,1,0) : OPvec3Create(0,0,1));
	return OPvec3Cross(v, other);
}

inline OPquat OPquatRotationBetween2(OPvec3 start, OPvec3 dest) {
	start = OPvec3Norm(start);
	dest = OPvec3Norm(dest);

	if(start.x == -dest.x && start.y == -dest.y && start.z == -dest.z) {
		return OPquat(OPvec3Norm(OPvec3Orthogonal(start)), 0);
	}

	OPvec3 half = OPvec3Norm(start + dest);
	return OPquat(OPvec3Cross(start, half), OPvec3Dot(start, half));
}

inline OPquat OPquatRotationBetween3(OPvec3 start, OPvec3 dest) {
	start = OPvec3Norm(start);
	dest = OPvec3Norm(dest);

	OPvec3 axis = OPvec3Cross(start, dest);
	f32 len = OPvec3Len(axis);
	if(len == 0) {
		return OPquat(0,0,0,1);
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

inline OPfloat OPquatLen(OPquat a){
	return (OPfloat)OPsqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

inline OPquat OPquatLerp(OPquat a, OPquat b, OPfloat p){
	OPquat as = OPquatScl(a, p);
	OPquat bs = OPquatScl(b, 1 - p);
	return as + bs;
}