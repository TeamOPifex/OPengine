#pragma once
#ifndef OP_MATH_QUATERNION
#define OP_MATH_QUATERNION

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPmath.h"
#include "./Math/include/OPvec3.h"

#define OPQUAT_AXIS(q) *((OPvec3*)(&q))

struct OPquat {
	OPfloat x, y, z, w;
	OPquat operator=(OPquat vhs) {
		OPmemcpy(this, &vhs, sizeof(OPquat)); return *this;
	}
};

extern const OPquat OPQUAT_IDENTITY;

inline OPquat OPquatCreate(OPfloat x, OPfloat y, OPfloat z, OPfloat w) {
	OPquat tmp = { x, y, z, w };
	return tmp;
}

inline OPquat OPquatCreate(OPvec3 axis, OPfloat w) {
	OPquat tmp = { axis.x, axis.y, axis.z, w };
	return tmp;
}

inline OPquat OPquatAdd(OPquat a, OPquat b);
inline OPquat OPquatSub(OPquat a, OPquat b);
inline OPquat OPquatMul(OPquat a, OPquat b);
inline OPquat OPquatScl(OPquat a, OPfloat s);
inline OPquat OPquatConj(OPquat a);
inline OPquat OPquatNorm(OPquat a);
inline OPfloat OPquatLen(OPquat a);
inline OPfloat OPquatDot(OPquat a, OPquat b);
inline OPfloat OPquatAngularDif(OPquat a, OPquat b);
inline OPquat OPquatCreateRot(OPvec3 axis, OPfloat angle);
inline OPquat OPquatCreateLookAt(OPvec3 eye, OPvec3 target);
inline OPvec3 OPquatRot(OPquat q, OPvec3 v);

inline OPquat OPquatAdd(OPquat a, OPquat b){
	OPquat out = {
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
		a.w + b.w
	};
	return out;
}

inline OPquat OPquatSub(OPquat a, OPquat b){
	OPquat out = {
		a.x - b.x,
		a.y - b.y,
		a.z - b.z,
		a.w - b.w
	};
	return out;
}

inline OPquat OPquatMul(OPquat a, OPquat b){
	OPfloat dot = a.x * b.x + a.y * b.y + a.z * b.z;
	OPvec3 vb = OPQUAT_AXIS(b) * a.w;
	OPvec3 va = OPQUAT_AXIS(a) * b.w;
	OPvec3 sum = vb + va;
	OPvec3 cross = OPvec3Cross(OPQUAT_AXIS(a), OPQUAT_AXIS(b));
	sum += cross;

	OPquat out = {
		sum.x,
		sum.y,
		sum.z,
		a.w * b.w - dot
	};

	return out;
}

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
	return OPquatMul(a, conj);
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
	OPvec3 qp = { q.x, q.y, q.z };
	// OPquat conj = OPquatConj(q);
	// conj = OPquatMul(&qp, &conj);
	// OPquat rot = OPquatMul(q, &conj);
	// OPvec3 out = { rot.x, rot.y, rot.z };

	OPvec3 c1 = OPvec3Cross(p, qp) + q.w * p;
	OPvec3 c2 = OPvec3Cross(c1, qp);

	return p + 2.0 * c2;
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

	if(OPabs(dot + 1.0) < 0.00001){
		return OPquatCreate(OPVEC3_UP, OPpi);
	}
	else if(OPabs(dot - 1.0) < 0.00001){
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
		return OPquatCreate(rotationAxis, 180.0f);
	}

	rotationAxis = OPvec3Cross(start, dest);

	f32 s = (f32)OPsqrt( (1+cosTheta)*2 );
	f32 invs = 1 / s;

	return OPquatCreate(
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
		return OPquatCreate(OPvec3Norm(OPvec3Orthogonal(start)), 0);
	}

	OPvec3 half = OPvec3Norm(start + dest);
	return OPquatCreate(OPvec3Cross(start, half), OPvec3Dot(start, half));
}

inline OPquat OPquatRotationBetween3(OPvec3 start, OPvec3 dest) {
	start = OPvec3Norm(start);
	dest = OPvec3Norm(dest);

	OPvec3 axis = OPvec3Cross(start, dest);
	f32 len = OPvec3Len(axis);
	if(len == 0) {
		return OPquatCreate(0,0,0,1);
	}

	f32 angle = OPasin(len);
	axis *= 1 / len;
	//OPvec3Norm(axis);
	//OPmat4RotY(axis.y)
	return OPquatCreate(axis, angle);
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
	return OPquatCreate(axis, phi);
}

inline OPfloat OPquatLen(OPquat a){
	return OPsqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

inline OPquat OPquatLerp(OPquat a, OPquat b, OPfloat p){
	OPquat as = OPquatScl(a, p);
	OPquat bs = OPquatScl(b, 1 - p);
	return OPquatAdd(as, bs);
}
#endif
