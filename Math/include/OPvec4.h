#pragma once

struct OPvec4;
typedef struct OPvec4 OPvec4;

#include "./Core/include/OPtypes.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPmath.h"
#include "./Math/include/OPvec3.h"

struct OPvec4 {
	union{
		struct {
			OPfloat x, y, z, w;
		};
		OPfloat row[4];
	};

	OPvec4() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	}

	OPvec4(OPfloat x) {
		this->x = x;
		this->y = x;
		this->z = x;
		this->w = x;
	}

	OPvec4(OPfloat x, OPfloat y, OPfloat z, OPfloat w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	OPvec4(OPvec3 xyz, OPfloat w) {
		this->x = xyz.x;
		this->y = xyz.y;
		this->z = xyz.z;
		this->w = w;
	}


	inline OPvec4 Add(OPvec4 b) {
		x = x + b.x;
		y = y + b.y;
		z = z + b.z;
		w = w + b.w;
		return *this;
	}

	inline OPvec4 Add(OPvec4 a, OPvec4 b) {
		OPvec4 t = a;
		return t.Add(b);
	}

	inline OPvec4 Subtract(OPvec4 b) {
		x = x - b.x;
		y = y - b.y;
		z = z - b.z;
		w = w - b.w;
		return *this;
	}

	inline OPvec4 Subtract(OPvec4 a, OPvec4 b) {
		OPvec4 t = a;
		return t.Subtract(b);
	}

	inline OPvec4 Divide(OPvec4 b) {
		x = x / b.x;
		y = y / b.y;
		z = z / b.z;
		w = w / b.w;
		return *this;
	}

	inline OPvec4 Divide(OPvec4 a, OPvec4 b) {
		OPvec4 t = a;
		return t.Divide(b);
	}

	inline OPvec4 Divide(OPfloat b) {
		x = x / b;
		y = y / b;
		z = z / b;
		w = w / b;
		return *this;
	}

	inline OPvec4 Divide(OPvec4 a, OPfloat b) {
		OPvec4 t = a;
		return t.Divide(b);
	}

	inline OPvec4 Multiply(OPvec4 b) {
		x = x * b.x;
		y = y * b.y;
		z = z * b.z;
		w = w * b.w;
		return *this;
	}

	inline OPvec4 Multiply(OPvec4 a, OPvec4 b) {
		OPvec4 t = a;
		return t.Multiply(b);
	}

	inline OPvec4 Multiply(OPfloat b) {
		x = x * b;
		y = y * b;
		z = z * b;
		w = w * b;
		return *this;
	}

	inline OPvec4 Multiply(OPvec4 a, OPfloat b) {
		OPvec4 t = a;
		return t.Multiply(b);
	}

	OPvec4 operator=(OPvec4 vhs) {
		OPmemcpy(this, &vhs, sizeof(OPvec4)); return *this;
	}

	inline OPvec4 operator+=(OPvec4 vhs) {
		Add(vhs);
		return *this;
	}

	inline OPvec4 operator-=(OPvec4 vhs) {
		Subtract(vhs);
		return *this;
	}

	//OPvec4 operator*=(OPmat4 rhs);

	inline OPvec4 operator*=(OPvec4 vhs) {
		return Multiply(vhs);
	}

	inline OPvec4 operator*=(OPfloat vhs) {
		return Multiply(vhs);
	}

	inline OPvec4 operator/=(OPvec4 vhs) {
		return Divide(vhs);
	}

	inline OPvec4 operator/=(OPfloat vhs) {
		return Divide(vhs);
	}

	inline OPfloat& operator[](i32 i) {
		return row[i];
	}


	inline OPvec4 operator+(OPvec4 vhs) {
		return Add(*this, vhs);
	}

	inline OPvec4 operator-(OPvec4 vhs) {
		return Subtract(*this, vhs);
	}

	inline OPvec4 operator*(OPvec4 vhs) {
		return Multiply(*this, vhs);
	}

	inline OPvec4 operator*(OPfloat vhs) {
		return Multiply(*this, vhs);
	}

	inline OPvec4 operator/(OPvec4 vhs) {
		return Divide(*this, vhs);
	}

	inline OPvec4 operator/(OPfloat vhs) {
		return Divide(*this, vhs);
	}


	inline OPfloat Len() {
		return (OPfloat)OPsqrt(x * x + y * y + z * z + w * w);
	}

	inline OPfloat Dist(OPvec4 b) {
		OPvec4 tmp = OPvec4(x - b.x, y - b.y, z - b.z, w - b.w);
		return tmp.Len();
	}

	inline OPfloat OPvec4Dot(OPvec4 a, OPvec4 b) {
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	inline OPvec4 GetNormal() {
		OPfloat l = Len();
		return OPvec4(
			x / l,
			y / l,
			z / l,
			w / l
		);
	}

	inline void Normalize() {
		OPfloat l = Len();
		x = x / l;
		y = y / l;
		z = z / l;
		w = w / l;
	}

	inline static OPvec4 Read(OPstream* str) {
		return OPvec4(
			str->F32(),
			str->F32(),
			str->F32(),
			str->F32());
	}
	
	inline void Write(OPstream* str) {
		str->Write(&x);
		str->Write(&y);
		str->Write(&z);
		str->Write(&w);
	}

	inline void Log(const OPchar* m) {
		OPlog("%s: [%f, %f, %f, %f]", m, x, y, z, w);
	}

	inline static OPvec4 Random(f32 min, f32 max) {
		f32 minMax = max - min;
		return OPvec4(
			min + OPrandom() * minMax,
			min + OPrandom() * minMax,
			min + OPrandom() * minMax,
			min + OPrandom() * minMax);
	}
};

extern const OPvec4 OPVEC4_ZERO;
extern const OPvec4 OPVEC4_ONE;