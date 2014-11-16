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

inline OPquat OPquatCreate(OPfloat x, OPfloat y, OPfloat z, OPfloat w) {
	OPquat tmp = { x, y, z, w };
	return tmp;
}

inline OPquat OPquatAdd(OPquat* a, OPquat* b);
inline OPquat OPquatSub(OPquat* a, OPquat* b);
inline OPquat OPquatMul(OPquat* a, OPquat* b);
inline OPquat OPquatScl(OPquat* a, OPfloat s);
inline OPquat OPquatConj(OPquat* a);
inline OPquat OPquatNorm(OPquat* a);
inline OPfloat OPquatDot(OPquat* a, OPquat* b);
inline OPquat OPquatCreateRot(OPvec3* axis, OPfloat angle);
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
	OPvec3 vb = OPvec3valScl((OPvec3*)b, a->w);
	OPvec3 va = OPvec3valScl((OPvec3*)a, b->w);
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
	OPquat qp = { p->x, p->y, p->z, 0 };
	OPquat conj = OPquatConj(q);
	conj = OPquatMul(&qp, &conj);
	OPquat rot = OPquatMul(q, &conj);
	OPvec3 out = { rot.x, rot.y, rot.z };

	return out;
}

inline OPquat OPquatCreateRot(OPvec3* axis, OPfloat angle){
	OPfloat c = OPcos(0.5f * angle), s = OPsin(0.5f * angle);
	OPvec3 n;

	OPvec3norm(&n, axis);
	OPvec3 v = OPvec3valScl(&n, s);
	OPquat out = {
		v.x,
		v.y,
		v.z,
		c
	};

	return out;
}

#endif