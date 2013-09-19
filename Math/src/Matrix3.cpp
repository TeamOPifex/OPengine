
#include "./Math/include/Matrix3.h"
#include "./Core/include/DynamicMemory.h"

void OPmat3identity(OPmat3* dst) {
	OPbzero(dst, sizeof(OPmat3));
	dst->cols[0].x = 1;
	dst->cols[1].y = 1;
	dst->cols[2].z = 1;
}