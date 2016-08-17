#include "./Math/include/OPmat4.h"

const OPmat4 OPMAT4_ZERO = {
	OPvec4(0, 0, 0, 0),
	OPvec4(0, 0, 0, 0),
	OPvec4(0, 0, 0, 0),
	OPvec4(0, 0, 0, 0)
};

const OPmat4 OPMAT4_IDENTITY = {
	OPvec4(1, 0, 0, 0),
	OPvec4(0, 1, 0, 0),
	OPvec4(0, 0, 1, 0),
	OPvec4(0, 0, 0, 1)
};

OPmat4 OPmat4Ortho(OPfloat left, OPfloat right, OPfloat bottom, OPfloat top, OPfloat zNear, OPfloat zFar){
	OPmat4 m = OPMAT4_IDENTITY;
	OPfloat w = (right - left);
	OPfloat h = (top - bottom);

	//m[0][0] = 2.0f / w;
	//m[1][1] = 2.0f / h;
	//m[2][2] = 1.0f / (zFar - zNear);
	//m[3][0] = -((right + left) / (right - left));
	//m[3][1] = -((top + bottom) / (top - bottom));
	////m[3][2] = -(zFar + zNear) / (zFar - zNear);
	//m[3][2] = -((zNear) / (zFar - zNear));

	m[0][0] = 2.0f / (right - left);
	m[1][1] = 2.0f / (top - bottom);
	m[2][2] = -2.0f / (zFar - zNear);
	m[3][0] = -(right + left) / (right - left);
	m[3][1] = -(top + bottom) / (top - bottom);
	m[3][2] = -(zFar + zNear) / (zFar - zNear);

	return m;
}

OPmat4 OPmat4LookAt(OPvec3 eye, OPvec3 center, OPvec3 up) {
	OPmat4 m = OPMAT4_IDENTITY;

	OPvec3 dist = center - eye;
	OPvec3 f = OPvec3Norm(dist);
	OPvec3 crossed = OPvec3Cross(f, up);
	OPvec3 s = OPvec3Norm(crossed);
	OPvec3 u = OPvec3Cross(s, f);

	m[0][0] = s.x;
	m[1][0] = s.y;
	m[2][0] = s.z;
	m[0][1] = u.x;
	m[1][1] = u.y;
	m[2][1] = u.z;
	m[0][2] =-f.x;
	m[1][2] =-f.y;
	m[2][2] =-f.z;
	m[3][0] =-OPvec3Dot(s, eye);
	m[3][1] =-OPvec3Dot(u, eye);
	m[3][2] = OPvec3Dot(f, eye);


	//OPvec3 X, Y, Z;

	//Z = eye - center;
	//Z = OPvec3Norm(Z);

	//Y = up;
	//X = OPvec3Cross(Y, Z);

	//Y = OPvec3Cross(Z, X);

	//X = OPvec3Norm(X);
	//Y = OPvec3Norm(Y);

	//m[0][0] = X.x;
	//m[1][0] = X.y;
	//m[2][0] = X.z;
	//m[3][0] = -OPvec3Dot(X, eye);
	//m[0][1] = Y.x;
	//m[1][1] = Y.y;
	//m[2][1] = Y.z;
	//m[3][1] = -OPvec3Dot(Y, eye);
	//m[0][2] = Z.x;
	//m[1][2] = Z.y;
	//m[2][2] = Z.z;
	//m[3][2] = -OPvec3Dot(Z, eye);
	//m[0][3] = 0;
	//m[1][3] = 0;
	//m[2][3] = 0;
	//m[3][3] = 1.0f;

	return m;
}

OPmat4 OPmat4Perspective(OPfloat fovy, OPfloat aspect, OPfloat nearVal, OPfloat farVal) {
	OPmat4 m = OPMAT4_IDENTITY;

	OPfloat tanHalfFovy = OPtan(fovy / 2.0f);
	m[0][0] = 1.0f / ( aspect * tanHalfFovy);
	m[1][1] = 1.0f / tanHalfFovy;
	m[2][2] = -(farVal + nearVal) / ( farVal - nearVal);
	m[2][3] = -1.0f;
	m[3][2] = -(2.0f * farVal * nearVal) / ( farVal - nearVal);

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
		*dst = OPMAT4_IDENTITY;
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

OPmat4 OPmat4Interpolate(OPmat4 a, OPmat4 b, OPfloat percent) {
	OPmat4 result;

	for (i32 j = 0; j < 4; j++) {
		for (i32 k = 0; k < 4; k++) {
			result[j][k] = a[j][k] * (1.0f - percent) + b[j][k] * percent;
		}
	}

	return result;
}
