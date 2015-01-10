#ifndef OPENGINE_HUMAN_UTILITIES_LOADER_OPLVL
#define OPENGINE_HUMAN_UTILITIES_LOADER_OPLVL

#include "./Math/include/OPvec3.h"

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	OPchar* name;
	OPchar* resource;
	OPvec3 position;
	OPvec3 rotation;
	OPvec3 scale;

	ui16 collision;
	ui16 physics;
} OPLevelEntity;

typedef struct {
	ui16 count;
	OPLevelEntity* entities;
} OPLevel;

EXPORT_METHOD OPLevelEntity* OPlevelGetEntity(OPLevel* level, OPchar* name);

EXPORT_METHOD OPint OPlevelload(const OPchar* filename, OPLevel** level);
EXPORT_METHOD OPint OPlevelReload(const OPchar* filename, OPLevel** level);
EXPORT_METHOD OPint OPlevelUnload(void* image);


// prevent name mangling if compiling with c++
#ifdef __cplusplus
};
#endif

#endif