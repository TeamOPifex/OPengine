#include "./Math/include/Vector3.h"

#include "./Math/include/Matrix4.h"

OPvec3 const OPvec3Zero = {0, 0, 0};
OPvec3 const OPvec3One  = {1, 1, 1};

OPvec3 OPvec3::operator*=(OPmat4 rhs) 
{
	OPmat4transform(this, this, &rhs);
	return *this; 
}