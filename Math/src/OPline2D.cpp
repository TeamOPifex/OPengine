#include "./Math/include/OPline2D.h"
#include "./Math/include/OPboundingBox2D.h"

OPintersectionType OPline2D::Intersect(OPboundingBox2D* bb) {
	// If line start and line end are in the bounding box it returns 2 (OPintersectContains)
	// If line start or line end are in the bound box it returns 1 (OPintersectIntersects)
	// Otherwise it returns 0 (OPintersectNone)
	return (OPintersectionType)(
		// Contains Line Start
		(start.x >= bb->min.x && start.x <= bb->max.x && start.y >= bb->min.y && start.y <= bb->max.y)
		+
		// Contains Line End
		(end.x >= bb->min.x && end.x <= bb->max.x && end.y >= bb->min.y && end.y <= bb->max.y));
}