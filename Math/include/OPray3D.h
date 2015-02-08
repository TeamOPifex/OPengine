#ifndef OP_MATH_RAY3D
#define OP_MATH_RAY3D

#include "./Math/include/OPvec3.h"

typedef struct {
	OPvec3 position;
	OPvec3 direction;
} OPray3D;

typedef struct {
	OPvec3 position;
	OPvec3 normal;
} OPplane3D;

inline OPint OPplane3DIntersects(OPplane3D plane, OPray3D ray, OPvec3* position) {
	// position_along_line = point_on_line + some_float * line_direction;
	// dot(point_in_plane, plane_normal) = distance_to_origin
	// point_in_plane = position_along_line
	// dot(point_on_line + some_float * line_direction, plane_normal) = distance_to_origin

	// t = (distance_to_origin - dot(point_on_line, plane_normal)) / dot(line_direction, plane_normal)	
	
	OPfloat distance_to_origin = OPvec3Len(plane.position);
	OPlog("Distance to Origin: %f", distance_to_origin);

	ray.direction = OPvec3Norm(ray.direction);
	OPfloat DdotN = OPvec3Dot(ray.direction, plane.normal);
	OPlog("Dot Product: %f", DdotN);

	if(DdotN == 0) return 0; // Does not intersect

	OPfloat t = (distance_to_origin - OPvec3Dot(ray.position, plane.normal)) / DdotN;

	OPlog("Distance along %f", t);

	if(t > 0) {
		(*position) = ray.position;
		(*position) += ray.direction * t;
		return 1;
	}
	return 0; // Does not intersect
}

#endif