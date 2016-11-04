#include "./Math/include/OPvec3.h"
#include "./Math/include/OPmat4.h"

const OPvec3 OPVEC3_ZERO    = { 0, 0, 0 };
const OPvec3 OPVEC3_ONE     = { 1, 1, 1 };
const OPvec3 OPVEC3_UP      = { 0, 1, 0 };
const OPvec3 OPVEC3_LEFT = { 1, 0, 0 };
const OPvec3 OPVEC3_FORWARD = { 0, 0, 1 };
const OPvec3 OPVEC3_BACKWARD = { 0, 0, -1 };

OPvec3 OPvec3::operator*=(OPmat4 rhs) 
{
	return OPmat4Transform(*this, rhs);
}

OPfloat OPvec3AngleToTarget(OPvec3 pos, OPvec3 facing, OPvec3 target) {
	OPvec3 _toPos = target - pos;
	OPvec3 _facing = OPvec3Norm(facing);
	_toPos.Norm();

	OPvec3 C = OPvec3Cross(_facing, _toPos);
	//f32 dir = OPvec3Dot(C, facing);
	//
	f32 dot = OPvec3Dot(_facing, _toPos);
	////OPlogErr("Dot: %f", dot);
	f32 angle = OPacos(dot);

	if (C.y < 0) return -angle;
	return angle;
}