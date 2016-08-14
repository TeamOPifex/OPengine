#pragma once

struct OPLevel;
struct OPLevelEntity;

typedef struct OPLevel OPLevel;
typedef struct OPLevelEntity OPLevelEntity;

#include "./Math/include/OPvec3.h"

struct OPLevelEntity{
	OPchar* name;
	OPchar* resource;
	OPvec3 position;
	OPvec3 rotation;
	OPvec3 scale;

	ui16 collision;
	ui16 physics;
};

struct OPLevel {
	ui16 count;
	OPLevelEntity* entities;
};

OPLevelEntity* OPlevelGetEntity(OPLevel* level, OPchar* name);

OPint OPlevelload(const OPchar* filename, OPLevel** level);
OPint OPlevelReload(const OPchar* filename, OPLevel** level);
OPint OPlevelUnload(void* image);