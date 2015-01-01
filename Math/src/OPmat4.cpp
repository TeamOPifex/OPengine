#include "./Math/include/OPmat4.h"

const OPmat4 OPMAT4ZERO = {
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
};

const OPmat4 OPMAT4IDENTITY = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

OPmat4 OPmat4Ortho(OPfloat left, OPfloat right, OPfloat bottom, OPfloat top, OPfloat zNear, OPfloat zFar){
	OPmat4 m = OPMAT4IDENTITY;
	m[0][0] = 2.0f / (right - left);
	m[1][1] = -2.0f / (top - bottom);
	m[2][2] = 2.0f / (zNear - zFar);
	m[3][3] = 1.0f;
	m[3][0] = -(right + left) / (right - left);
	m[3][1] = -(top + bottom) / (top - bottom);
	m[3][2] = -(zFar + zNear) / (zFar - zNear);
	return m;
}

OPmat4 OPmat4LookAt(OPvec3 eye, OPvec3 at, OPvec3 up) {
	OPmat4 m = OPMAT4IDENTITY;
	OPvec3 dist = at - eye;
	OPvec3 f = OPvec3Norm(dist);
	OPvec3 u = OPvec3Norm(up);
	OPvec3 s = OPvec3Cross(f, u);
	s = OPvec3Norm(s);
	u = OPvec3Cross(s, f);

	OPfloat sDot = OPvec3Dot(s, eye);
	OPfloat uDot = OPvec3Dot(u, eye);
	OPfloat fDot = OPvec3Dot(f, eye);

	m[0].x = s.x;
	m[1].x = s.y;
	m[2].x = s.z;
	m[0].y = u.x;
	m[1].y = u.y;
	m[2].y = u.z;
	m[0].z = -f.x;
	m[1].z = -f.y;
	m[2].z = -f.z;
	m[3].x = -sDot;
	m[3].y = -uDot;
	m[3].z = fDot;

	return m;
}

OPmat4 OPmat4Perspective(OPfloat fovy, OPfloat aspect, OPfloat nearVal, OPfloat farVal) {
	OPmat4 m;
	OPfloat top = OPtan(fovy * OPpi / 360.0f) * nearVal;
	OPfloat right = top * aspect;
	OPfloat range = farVal - nearVal;
	OPvec4 c0 = { nearVal / right, 0, 0, 0 };
	OPvec4 c1 = { 0, nearVal / top, 0, 0 };
	OPvec4 c2 = { 0, 0, -(farVal + nearVal) / range, -1 };
	OPvec4 c3 = { 0, 0, -2.0f*farVal*nearVal / range, 1 };
	m[0] = c0;
	m[1] = c1;
	m[2] = c2;
	m[3] = c3;
	return m;
}

// TODO: Refactor into appropriate namespace method call
OPint OPmat4Inverse(OPmat4* dst, OPmat4 a) {

	OPmat4 b = a;
	OPfloat* m = (OPfloat*)&b;


	// get cofactors of minor matrices
	OPfloat cofactor0 = OPmat4GetCofactor(m[5], m[6], m[7], m[9], m[10], m[11], m[13], m[14], m[15]);
	OPfloat cofactor1 = OPmat4GetCofactor(m[4], m[6], m[7], m[8], m[10], m[11], m[12], m[14], m[15]);
	OPfloat cofactor2 = OPmat4GetCofactor(m[4], m[5], m[7], m[8], m[9], m[11], m[12], m[13], m[15]);
	OPfloat cofactor3 = OPmat4GetCofactor(m[4], m[5], m[6], m[8], m[9], m[10], m[12], m[13], m[14]);

	// get determinant
	OPfloat determinant = m[0] * cofactor0 - m[1] * cofactor1 + m[2] * cofactor2 - m[3] * cofactor3;
	if (OPabsf(determinant) <= 0.00001f)
	{
		*dst = OPMAT4IDENTITY;
		return 0;
	}

	// get rest of cofactors for adj(M)
	OPfloat cofactor4 = OPmat4GetCofactor(m[1], m[2], m[3], m[9], m[10], m[11], m[13], m[14], m[15]);
	OPfloat cofactor5 = OPmat4GetCofactor(m[0], m[2], m[3], m[8], m[10], m[11], m[12], m[14], m[15]);
	OPfloat cofactor6 = OPmat4GetCofactor(m[0], m[1], m[3], m[8], m[9], m[11], m[12], m[13], m[15]);
	OPfloat cofactor7 = OPmat4GetCofactor(m[0], m[1], m[2], m[8], m[9], m[10], m[12], m[13], m[14]);

	OPfloat cofactor8 = OPmat4GetCofactor(m[1], m[2], m[3], m[5], m[6], m[7], m[13], m[14], m[15]);
	OPfloat cofactor9 = OPmat4GetCofactor(m[0], m[2], m[3], m[4], m[6], m[7], m[12], m[14], m[15]);
	OPfloat cofactor10 = OPmat4GetCofactor(m[0], m[1], m[3], m[4], m[5], m[7], m[12], m[13], m[15]);
	OPfloat cofactor11 = OPmat4GetCofactor(m[0], m[1], m[2], m[4], m[5], m[6], m[12], m[13], m[14]);

	OPfloat cofactor12 = OPmat4GetCofactor(m[1], m[2], m[3], m[5], m[6], m[7], m[9], m[10], m[11]);
	OPfloat cofactor13 = OPmat4GetCofactor(m[0], m[2], m[3], m[4], m[6], m[7], m[8], m[10], m[11]);
	OPfloat cofactor14 = OPmat4GetCofactor(m[0], m[1], m[3], m[4], m[5], m[7], m[8], m[9], m[11]);
	OPfloat cofactor15 = OPmat4GetCofactor(m[0], m[1], m[2], m[4], m[5], m[6], m[8], m[9], m[10]);

	// build inverse matrix = adj(M) / det(M)
	// adjugate of M is the transpose of the cofactor matrix of M
	OPfloat invDeterminant = 1.0f / determinant;
	m[0] = invDeterminant * cofactor0;
	m[1] = -invDeterminant * cofactor4;
	m[2] = invDeterminant * cofactor8;
	m[3] = -invDeterminant * cofactor12;

	m[4] = -invDeterminant * cofactor1;
	m[5] = invDeterminant * cofactor5;
	m[6] = -invDeterminant * cofactor9;
	m[7] = invDeterminant * cofactor13;

	m[8] = invDeterminant * cofactor2;
	m[9] = -invDeterminant * cofactor6;
	m[10] = invDeterminant * cofactor10;
	m[11] = -invDeterminant * cofactor14;

	m[12] = -invDeterminant * cofactor3;
	m[13] = invDeterminant * cofactor7;
	m[14] = -invDeterminant * cofactor11;
	m[15] = invDeterminant * cofactor15;

	*dst = b;

	return 1;
}