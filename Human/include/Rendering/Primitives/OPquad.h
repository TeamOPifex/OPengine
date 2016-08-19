#pragma once

struct OPquad;
typedef struct OPquad OPquad;

#include "./Human/include/Rendering/OPrenderBuffer.h"
#include "./Human/include/Rendering/OPmodel.h"
#include "./Human/include/Rendering/OPmeshPacker.h"
#include "./Human/include/Rendering/OPmeshPacked.h"
#include "./Math/include/OPvec2.h"

struct OPquad {

};

// OPmesh       OPquadCreate(OPint features, OPvec2 size);
OPmodel*       OPquadCreate();
OPmodel*       OPquadCreate(OPfloat width, OPfloat height);
OPmodel*       OPquadCreate(OPfloat width, OPfloat height, OPvec2 offset);
OPmodel*		 OPquadCreate(OPfloat width, OPfloat height, OPvec2 texcoordStart, OPvec2 texcoordEnd);
OPmodel*       OPquadCreate(OPfloat width, OPfloat height, OPvec2 offset, OPvec2 texcoordStart, OPvec2 texcoordEnd);
OPmodel*       OPquadCreateZPlane();
OPmodel*       OPquadCreateZPlane(OPfloat width, OPfloat depth);
OPmodel*       OPquadCreateZPlane(OPfloat width, OPfloat depth, OPvec2 texcoordStart, OPvec2 texcoordEnd);
//OPmeshPacked OPquadCreatePacked();
OPmodel*       OPquadNormCreate();
//OPmeshPacked OPquadNormCreatePacked();
void         OPquadDestroy(OPmesh* quad);
