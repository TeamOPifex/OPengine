#include "./Math/include/OPquat.h"

const OPquat OPQUAT_IDENTITY = { 0, 0, 0, 1 };

OPquat OPquat::operator*(OPquat rhs) {
	OPfloat dot = x * rhs.x + y * rhs.y + z * rhs.z;
	OPvec3 vb = OPQUAT_AXIS(rhs) * w;
	OPvec3 va = OPQUAT_AXIS(*this) * rhs.w;
	OPvec3 sum = vb + va;
	OPvec3 cross = OPvec3Cross(OPQUAT_AXIS(*this), OPQUAT_AXIS(rhs));
	sum += cross;

	return OPquat(sum, w * rhs.w - dot);
}