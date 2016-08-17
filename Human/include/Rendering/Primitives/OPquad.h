#pragma once

struct OPquad;
typedef struct OPquad OPquad;

#include "./Human/include/Rendering/OPrenderBuffer.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPmeshPacker.h"
#include "./Human/include/Rendering/OPmeshPacked.h"
#include "./Math/include/OPvec2.h"

struct OPquad {

};

// OPmesh       OPquadCreate(OPint features, OPvec2 size);
OPmesh*       OPquadCreate();
OPmesh*       OPquadCreate(OPfloat width, OPfloat height);
OPmesh*       OPquadCreate(OPfloat width, OPfloat height, OPvec2 offset);
OPmesh*		 OPquadCreate(OPfloat width, OPfloat height, OPvec2 texcoordStart, OPvec2 texcoordEnd);
OPmesh*       OPquadCreate(OPfloat width, OPfloat height, OPvec2 offset, OPvec2 texcoordStart, OPvec2 texcoordEnd);
OPmesh*       OPquadCreateZPlane();
OPmesh*       OPquadCreateZPlane(OPfloat width, OPfloat depth);
OPmesh*       OPquadCreateZPlane(OPfloat width, OPfloat depth, OPvec2 texcoordStart, OPvec2 texcoordEnd);
OPmeshPacked OPquadCreatePacked();
OPmesh*       OPquadNormCreate();
OPmeshPacked OPquadNormCreatePacked();
void         OPquadDestroy(OPmesh* quad);
