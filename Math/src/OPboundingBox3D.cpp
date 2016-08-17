#include "./Math/include/OPboundingBox3D.h"
#include "./Math/include/OPray3D.h"

OPintersectionType OPboundingBox3D::Collision(OPboundingBox3D bounds) {
	// TODO: AABB testing in 3D

	if (min.x <= bounds.min.x && max.x >= bounds.max.x &&
		min.y <= bounds.min.y && max.y >= bounds.max.y &&
		min.z <= bounds.min.z && max.z >= bounds.max.z)
		return OPintersectionType::CONTAINS;

	if (max.x > bounds.min.x &&
		min.x < bounds.max.x &&
		max.y > bounds.min.y &&
		min.y < bounds.max.y &&
		max.z > bounds.min.z &&
		min.z < bounds.max.z) {
		return OPintersectionType::INTERSECTS;
	}

	return OPintersectionType::NONE;
}

bool OPboundingBox3D::Intersects(OPray3D ray) {
	double tmin = -INFINITY, tmax = INFINITY;

	for (int i = 0; i < 3; ++i) {
		if (ray.direction[i] != 0.0) {
			double t1 = (min[i] - ray.position[i]) / ray.direction[i];
			double t2 = (max[i] - ray.position[i]) / ray.direction[i];

			tmin = OPMAX(tmin, OPMIN(t1, t2));
			tmax = OPMIN(tmax, OPMAX(t1, t2));
		}
		else if (ray.position[i] <= min[i] || ray.position[i] >= max[i]) {
			return false;
		}
	}

	return tmax > tmin && tmax > 0.0;
}