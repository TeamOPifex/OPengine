#include "./Math/include/Vector3.h"

#include "./Math/include/Matrix4.h"

OPvec3 OPvec3::operator*=(OPmat4 rhs) 
{
	OPmat4transform(this, this, &rhs);
	return *this; 
}