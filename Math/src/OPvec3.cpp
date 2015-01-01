#include "./Math/include/OPvec3.h"
#include "./Math/include/OPmat4.h"

const OPvec3 OPVEC3_ZERO    = { 0, 0, 0 };
const OPvec3 OPVEC3_ONE     = { 1, 1, 1 };
const OPvec3 OPVEC3_UP      = { 0, 1, 0 };
const OPvec3 OPVEC3_LEFT    = { 1, 0, 0 };
const OPvec3 OPVEC3_FORWARD = { 0, 0, 1 };

OPvec3 OPvec3::operator*=(OPmat4 rhs) 
{
	return OPmat4Transform(*this, rhs);
}

OPfloat OPvec3AngleToTarget(OPvec3 pos, OPvec3 facing, OPvec3 target) {
	OPvec3 _toPos = target - pos;
	OPvec3 _facing = OPvec3Norm(facing);
	_toPos = OPvec3Norm(_toPos);	
	
	return OPacos(OPvec3Dot(_facing, _toPos));
}