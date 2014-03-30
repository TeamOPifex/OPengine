#ifndef OP_MATH_INTERSECTION2D
#define OP_MATH_INTERSECTION2D

#include "../include/Line2D.h"
#include "../include/Ray2D.h"
#include "../include/BoundingBox2D.h"

enum OPintersection {
	OPintersectNone = 0,
	OPintersectIntersects = 1,
	OPintersectContains = 2
};

inline OPintersection OPintersectLine2DBoundBox2D(OPline2D* line, OPboundingBox2D* bb) {
	// If line start and line end are in the bounding box it returns 2 (OPintersectContains)
	// If line start or line end are in the bound box it returns 1 (OPintersectIntersects)
	// Otherwise it returns 0 (OPintersectNone)
	return (OPintersection)(
		// Contains Line Start
		(line->start.x >= bb->min.x && line->start.x <= bb->max.x && line->start.y >= bb->min.y && line->start.y <= bb->max.y) 
		+ 
		// Contains Line End
		(line->end.x >= bb->min.x && line->end.x <= bb->max.x && line->end.y >= bb->min.y && line->end.y <= bb->max.y));
}

#endif