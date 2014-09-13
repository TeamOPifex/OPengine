#ifndef OPENGINE_HUMAN_UTILITIES_LOADER_OPLVL
#define OPENGINE_HUMAN_UTILITIES_LOADER_OPLVL

#include "./Math/include/Vector3.h"

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	i8* name;
	i8* resource;
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

OPLevelEntity* OPlevelGetEntity(OPLevel* level, i8* name);

OPint OPlevelload(const OPchar* filename, OPLevel** level);
OPint OPlevelReload(const OPchar* filename, OPLevel** level);
OPint OPlevelUnload(void* image);


// prevent name mangling if compiling with c++
#ifdef __cplusplus
};
#endif

#endif