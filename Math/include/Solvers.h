#ifndef OPIFEX_MATH_SOLVERS
#define OPIFEX_MATH_SOLVERS

#include "./Core/include/Types.h"
#include "./Core/include/MathHelpers.h"
#include "Vector2.h"
#include "Vector3.h"

inline OPint OPsolveIntersect(OPvec2* A, OPvec2* B, OPvec2* C, OPvec2* D, OPvec2* intersect){
	OPvec2 delta1 = *B - *A, delta2 = *C - *D;
	OPfloat m1 = delta1.y / delta1.x, m2 = delta2.y / delta2.x;
	OPfloat b1 = A->y - m1 * A->x, b2 = C->y - m2 * C->x;

	OPfloat x = -(b1 - b2) / (m1 - m2);

	if(OPabs(x - A->x) < delta1.x && OPabs(x - B->x) < delta1.x){
		intersect->x = x;
		intersect->y = x * m1 + b1;
		return 1;
	}
	else
		return 0;
};

inline OPint OPsolveCircleIntersect(OPvec2* A, OPvec2* B, OPvec2* CirclePos, OPfloat radius, OPvec2* intersect){
	OPvec2 dir = *B - *A;
	OPfloat radSqr = radius * radius;
	OPfloat dist = OPvec2valDot(&dir, &dir);
	OPfloat p = radSqr / dist;

	*intersect = dir * p + *CirclePos;

	if(OPabs(dist - radSqr) < 0.0001f){
		return 1;
	}

	return 0;
}

inline OPint OPsolveIntersect(OPvec3* v1, OPvec3* v2, OPvec3* intersect){
	
};

#endif