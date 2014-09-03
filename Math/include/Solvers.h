#ifndef OPIFEX_MATH_SOLVERS
#define OPIFEX_MATH_SOLVERS

#include "./Core/include/Types.h"
#include "./Core/include/MathHelpers.h"
#include "Vector2.h"
#include "Vector3.h"

/**
 * OPlineLineInter2() - Computes the interesction point of two 2D-line
 *                         segments.
 * @param A Vertex 1 in line segment 1
 * @param B Vertex 2 in line segment 1
 * @param C Vertex 1 in line segment 2
 * @param D Vertex 2 in line segment 2
 * @param intersection Computed intersection point between the lines
 * @return 1 if intersection, 0 if none. 
 */
inline OPint OPlineLineInter2(
	OPvec2* A, OPvec2* B,
	OPvec2* C, OPvec2* D,
	OPvec2* intersect)
{
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

/**
 * OPrayLineInter2() - Computes the interesction point of a 2D-line
 *                     segment and a 2D-ray.
 * @param O Starting point of the ray 
 * @param D Direction vector of the ray
 * @param A Vertex 1 in line segment
 * @param B Vertex 2 in line segment
 * @param intersection Computed intersection point between the lines
 * @return 1 if intersection, 0 if none. 
 */
inline OPint OPrayLineInter2(
	OPvec2* O, OPvec2* D,
	OPvec2* A, OPvec2* B,
	OPvec2* intersect)
{
	OPvec2  l   = *B - *A;
	OPvec2  OmA = *O - *A;
	OPvec2  ldlcD;
	OPfloat lcD = 0.0f, u = -1.0f;

	OPvec2cross(&lcD, &l, D);	
	ldlcD = l / lcD;
	OPvec2cross(&u, &OmA, &ldlcD); 
	
	//*intersect = l * u + A;

	return 1;
};
/**
 * OPlineCircleInter2() - Computes the interesction point of a 2D-line
 *                         segment and a circle.
 * @param A Vertex 1 in line segment
 * @param B Vertex 2 in line segment
 * @param CirclePos Two dimensional position of the circle
 * @param radSqr Square of the circle's radius
 * @param intersection Computed intersection point between the lines
 * @return 1 if intersection, 0 if none. 
 */
inline OPint OPlineCircleInter2(
	OPvec2* A, OPvec2* B,
	OPvec2* CirclePos,
	OPfloat radSqr, OPvec2* intersection)
{
	OPvec2 dir = *B - *A;
	OPfloat dist = OPvec2valDot(&dir, &dir);
	OPfloat p = radSqr / dist;

	//*intersect = dir * p + *CirclePos;

	if(OPabs(dist - radSqr) < 0.0001f){
		return 1;
	}

	return 0;
}

#endif
