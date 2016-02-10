//
// Created by Garrett Hoofman on 11/28/15.
//

#ifndef OPIFEXENGINE_OPTRIANGLE_H
#define OPIFEXENGINE_OPTRIANGLE_H

#include "./Math/include/OPvec3.h"

struct OPtriangle {
    OPvec3 pointOne;
    OPvec3 pointTwo;
    OPvec3 pointThree;
};
typedef OPtriangle OPtriangle;

#include "./Math/include/OPray3D.h"

#define EPSILON 0.000001

i8 OPtriangleRayIntersect(OPtriangle* triangle, OPray3D* ray, OPvec3* hit) {
    OPvec3 edge1, edge2, tvec, pvec, qvec;
    f32 det, inv_det;

    OPvec3Sub(&edge1, &triangle->pointTwo, &triangle->pointOne);
    OPvec3Sub(&edge2, &triangle->pointThree, &triangle->pointOne);

    pvec = OPvec3Cross(ray->direction, edge2);

    det = OPvec3Dot(edge1, pvec);

    // Do cull check
    if(det < EPSILON) {
        return 0;
    }

    OPvec3Sub(&tvec, &ray->position, &triangle->pointOne);
    hit->y = OPvec3Dot(tvec, pvec);
    if(hit->y < 0.0 || hit->y > det) {
        return 0;
    }

    qvec = OPvec3Cross(tvec, edge1);

    hit->z = OPvec3Dot(ray->direction, qvec);
    if(hit->z < 0.0 || (hit->z + hit->y) > det) {
        return 0;
    }

    hit->x = OPvec3Dot(edge2, qvec);
    inv_det = 1.0 / det;
    hit->x *= inv_det;
    hit->y *= inv_det;
    hit->z *= inv_det;

    return 1;
}

#endif //OPIFEXENGINE_OPTRIANGLE_H
