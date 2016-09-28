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

OPvec3 OPquat::Rot(OPvec3 p) {
	OPvec3 qp(x, y, z);

	OPvec3 c1 = OPvec3Cross(p, qp) + p * w;
	OPvec3 c2 = OPvec3Cross(c1, qp);

	return p + c2 * 2.0;
}


OPquat OPquat::CreateRot(OPvec3 axis, OPfloat angle) {
	OPfloat c = OPcos(0.5f * angle), s = OPsin(0.5f * angle);
	OPvec3 n = OPvec3Norm((OPvec3)axis);
	OPvec3 v = n * s;

	OPquat out = {
		v.x,
		v.y,
		v.z,
		c
	};

	return out;
}

OPquat OPquat::CreateLookAt(OPvec3 eye, OPvec3 target) {
	OPvec3 forward = eye - target;
	OPvec3 forwardUnit = OPvec3Norm(forward);

	OPfloat dot = OPvec3Dot(forwardUnit, OPVEC3_FORWARD);

	if (OPabs(dot + 1.0f) < 0.00001f) {
		return OPquat(OPVEC3_UP, OPpi);
	}
	else if (OPabs(dot - 1.0f) < 0.00001f) {
		return OPQUAT_IDENTITY;
	}

	OPfloat rotAngle = OPacos(dot);

	OPvec3 rotAxis = OPvec3Cross(forwardUnit, OPVEC3_FORWARD);
	rotAxis = OPvec3Norm(rotAxis);

	return OPquat::CreateRot(rotAxis, rotAngle);
}

OPvec3 OPquat::Orthogonal(OPvec3 v) {
	f32 x = OPabs(v.x);
	f32 y = OPabs(v.y);
	f32 z = OPabs(v.z);
	OPvec3 other = x < y ? (x < z ? OPvec3Create(1, 0, 0) : OPvec3Create(0, 0, 1)) : (y < z ? OPvec3Create(0, 1, 0) : OPvec3Create(0, 0, 1));
	return OPvec3Cross(v, other);
}

OPquat OPquat::Lerp(OPquat b, OPfloat p) {
	OPquat result;
	result.x = x + (b.x - x) * p;
	result.y = y + (b.y - y) * p;
	result.z = z + (b.z - z) * p;
	result.w = w + (b.w - w) * p;
	return result;
}

OPquat OPquat::Slerp(OPquat b, OPfloat p) {
	// cosine theta
	f32 cosom = x * b.x + y * b.y + z * b.z + w * b.w;

	// adjust signs
	OPquat end = b;
	if (cosom < 0.0f) {
		cosom = -cosom;
		end.x = -end.x;
		end.y = -end.y;
		end.z = -end.z;
		end.w = -end.w;
	}

	// Coefficients
	f32 sclp, sclq;
	if ((1.0f - cosom) > 0.0001f) {
		// Standard slerp
		f32 omega, sinom;
		omega = OPacos(cosom);
		sinom = OPsin(omega);
		sclp = OPsin((1.0f - p) * omega) / sinom;
		sclq = OPsin(p * omega) / sinom;
	}
	else {
		// very close interp (just linear)
		sclp = 1.0f - p;
		sclq = p;
	}

	OPquat result;
	result.x = sclp * x + sclq * end.x;
	result.y = sclp * y + sclq * end.y;
	result.z = sclp * z + sclq * end.z;
	result.w = sclp * w + sclq * end.w;
	return result;
}