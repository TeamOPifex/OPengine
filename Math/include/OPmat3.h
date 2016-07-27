#pragma once

#include "./Math/include/OPvec3.h"
#include "./Core/include/OPmemory.h"

struct OPmat3;
inline void OPmat3Identity(OPmat3* dst);

struct OPmat3 {
	OPvec3 cols[3];
	OPmat3 operator=(OPmat3 vhs) { 
		OPmemcpy(this, &vhs, sizeof(OPmat3)); return *this;
	}

	inline OPvec3* OPmat3Index(int idx) {
		return &((OPvec3*)(this))[idx];
	}
};


inline void OPmat3Identity(OPmat3* dst) {
	dst->cols[0].x = 1;
	dst->cols[1].y = 1;
	dst->cols[2].z = 1;
}