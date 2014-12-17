#pragma once

#include "./Math/include/OPvec3.h"
#include "./Core/include/OPmemory.h"

struct OPmat3;
inline OPvec3* OPmat3index(OPmat3* m, int idx);
inline void OPmat3identity(OPmat3* dst);

struct OPmat3 {
	OPvec3 cols[3];
	OPmat3 operator=(OPmat3 vhs) { 
		OPmemcpy(this, &vhs, sizeof(OPmat3)); return *this;
	}
};

inline OPvec3* OPmat3index(OPmat3* m, int idx){
	return &((OPvec3*)(m))[idx];
}

inline void OPmat3identity(OPmat3* dst) {
	OPbzero(dst, sizeof(OPmat3));
	dst->cols[0].x = 1;
	dst->cols[1].y = 1;
	dst->cols[2].z = 1;
}