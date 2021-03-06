#pragma once

#include "./Core/include/OPtypes.h"
#include "./Data/include/OPcman.h"

struct OPvecInt3 {
	OPint x, y, z;
};

struct OPvoxels {
	OPvecInt3 size;
	OPvecInt3 offset;
	OPvecInt3* voxels;
};

OPint OPvoxelsLoad(OPstream* path, void** asset);
OPvecInt3 OPvoxelsGet(OPvoxels* voxels, OPint x, OPint y, OPint z);
OPassetLoader* OPvoxelsLoader();