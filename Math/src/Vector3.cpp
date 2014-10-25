#include "./Math/include/Vector3.h"

#include "./Math/include/Matrix4.h"

OPvec3 const OPvec3Zero = { 0, 0, 0 };
OPvec3 const OPvec3One = { 1, 1, 1 };
OPvec3 const OPvec3Up = { 0, 1, 0 };

OPvec3 OPvec3::operator*=(OPmat4 rhs) 
{
	OPmat4transform(this, this, &rhs);
	return *this; 
}

OPfloat OPvec3angleToTarget(OPvec3 pos, OPvec3 facing, OPvec3 target) {
	OPvec3 _facing, _toPos;
	f32 angle, dot;	
	
	_toPos = target - pos;
	OPvec3norm(&_facing, &facing);
	OPvec3norm(&_toPos, &_toPos);	
	OPvec3dot(&dot, &_facing, &_toPos);
	return OPacos(dot);
}