#include "./Math/include/Vector4.h"
#include "./Core/include/MathHelpers.h"

void OPvec4norm(OPvec4* dst, OPvec4* a) {
	OPfloat l = OPsqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
	dst->x = a->x / l;
	dst->y = a->x / l;
	dst->z = a->x / l;
	dst->w = a->x / l;
}