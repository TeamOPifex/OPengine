#pragma once
#ifndef OP_MATH_QUATERNION
#define OP_MATH_QUATERNION

#include "./Core/include/Types.h"
#include "./Core/include/DynamicMemory.h"
#include "./Core/include/MathHelpers.h"
#include "./Math/include/Vector3.h"


struct OPquat {
	OPfloat x, y, z, w;
	OPquat operator=(OPquat vhs) {
		OPmemcpy(this, &vhs, sizeof(OPquat)); return *this;
	}
};

extern const OPquat OPquatIdentity;

inline OPquat OPquatCreate(OPfloat x, OPfloat y, OPfloat z, OPfloat w) {
	OPquat tmp = { x, y, z, w };
	return tmp;
}

inline OPquat OPquatCreate(OPvec3* axis, OPfloat w) {
	OPquat tmp = { axis->x, axis->y, axis->z, w };
	return tmp;
}

inline OPquat OPquatAdd(OPquat* a, OPquat* b);
inline OPquat OPquatSub(OPquat* a, OPquat* b);
inline OPquat OPquatMul(OPquat* a, OPquat* b);
inline OPquat OPquatScl(OPquat* a, OPfloat s);
inline OPquat OPquatConj(OPquat* a);
inline OPquat OPquatNorm(OPquat* a);
inline OPfloat OPquatLen(OPquat* a);
inline OPfloat OPquatDot(OPquat* a, OPquat* b);
inline OPfloat OPquatAngularDif(OPquat* a, OPquat* b);
inline OPquat OPquatCreateRot(OPvec3* axis, OPfloat angle);
inline OPquat OPquatCreateLookAt(OPvec3* eye, OPvec3* target);
inline OPvec3 OPquatRot(OPquat* q, OPvec3* v);

inline OPquat OPquatAdd(OPquat* a, OPquat* b){
	OPquat out = {
		a->x + b->x,
		a->y + b->y,
		a->z + b->z,
		a->w + b->w
	};
	return out;
}

inline OPquat OPquatSub(OPquat* a, OPquat* b){
	OPquat out = {
		a->x - b->x,
		a->y - b->y,
		a->z - b->z,
		a->w - b->w
	};
	return out;
}

inline OPquat OPquatMul(OPquat* a, OPquat* b){
	register OPfloat dot = a->x * b->x + a->y * b->y + a->z * b->z;
	OPvec3 cross;
	OPvec3 vb = OPvec3scl((OPvec3*)b, a->w);
	OPvec3 va = OPvec3scl((OPvec3*)a, b->w);
	OPvec3 sum = vb + va;
	OPvec3cross(&cross, (OPvec3*)a, (OPvec3*)b);
	sum += cross;

	OPquat out = {
		sum.x,
		sum.y,
		sum.z,
		a->w * b->w - dot
	};

	return out;
}

inline OPquat OPquatScl(OPquat* a, OPfloat s){
	OPquat out = {
		a->x * s,
		a->y * s,
		a->z * s,
		a->w * s
	};

	return out;
}

inline OPquat OPquatConj(OPquat* a){
	OPquat out = {
		-a->x,
		-a->y,
		-a->z,
		a->w
	};

	return out;	
}

inline OPquat OPquatNorm(OPquat* a){
	OPquat conj = OPquatConj(a);
	return OPquatMul(a, &conj);
}

inline OPfloat OPquatDot(OPquat* a, OPquat* b){
	return a->x * b->x +
	       a->y * b->y +
	       a->z * b->z +
	       a->w * b->w; 
}

inline OPvec3 OPquatRot(OPquat* q, OPvec3* p){
	OPvec3 qp = { q->x, q->y, q->z };
	// OPquat conj = OPquatConj(q);
	// conj = OPquatMul(&qp, &conj);
	// OPquat rot = OPquatMul(q, &conj);
	// OPvec3 out = { rot.x, rot.y, rot.z };

	OPvec3 c1 = OPvec3cross(p, &qp) + q->w * *p;
	OPvec3 c2 = OPvec3cross(&c1, &qp);

	return *p + 2.0 * c2;
}

inline OPfloat OPquatAngularDif(OPquat* a, OPquat* b){
	OPfloat dot = OPquatDot(a, b);
	return OPacos(dot / OPquatLen(a) * OPquatLen(b)) * 2;
}

inline OPquat OPquatCreateRot(OPvec3* axis, OPfloat angle){
	OPfloat c = OPcos(0.5f * angle), s = OPsin(0.5f * angle);
	OPvec3 n = { 0, 0, 0 };

	OPvec3norm(&n, (OPvec3*)axis);
	OPvec3 v = OPvec3scl(&n, s);
	OPquat out = {
		v.x,
		v.y,
		v.z,
		c
	};

	return out;
}

inline OPquat OPquatCreateLookAt(OPvec3* eye, OPvec3* target){
	OPvec3 forward = *eye - *target, forwardUnit;
	OPvec3norm(&forwardUnit, &forward);

	OPfloat dot = OPvec3dot(&forwardUnit, (OPvec3*)&OPvec3Forward);
	
	if(OPabs(dot + 1.0) < 0.00001){
		return OPquatCreate((OPvec3*)&OPvec3Up, OPpi);
	}
	else if(OPabs(dot - 1.0) < 0.00001){
		return OPquatIdentity;
	}

	OPfloat rotAngle = OPacos(dot);
	OPvec3 rotAxis;

	OPvec3cross(&rotAxis, &forwardUnit, (OPvec3*)&OPvec3Forward);
	OPvec3norm(&rotAxis, &rotAxis);

	return OPquatCreateRot(&rotAxis, rotAngle);
}

inline OPfloat OPquatLen(OPquat* a){
	return OPsqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
}

inline OPquat OPquatLerp(OPquat* a, OPquat* b, OPfloat p){
	OPquat as = OPquatScl(a, p);
	OPquat bs = OPquatScl(b, 1 - p);
	return OPquatAdd(&as, &bs);
}

#endif