#pragma once

struct OPmat3;
typedef struct OPmat3 OPmat3;

#include "./Math/include/OPvec3.h"
#include "./Core/include/OPmemory.h"

struct OPmat3 {
	OPvec3 cols[3];

	OPmat3 operator=(OPmat3 vhs) { 
		OPmemcpy(this, &vhs, sizeof(OPmat3)); return *this;
	}

	inline OPvec3* Index(int idx) {
		return &((OPvec3*)(this))[idx];
	}

	inline void Identity(OPmat3* dst) {
		dst->cols[0] = OPvec3(1,0,0);
		dst->cols[1] = OPvec3(0,1,0);
		dst->cols[2] = OPvec3(0,0,1);
	}
};
