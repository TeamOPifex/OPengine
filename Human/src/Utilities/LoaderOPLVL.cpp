#include "./Human/include/Utilities/LoaderOPLVL.h"

#include "./Data/include/OPstring.h"

OPLevelEntity* OPlevelGetEntity(OPLevel* level, OPchar* name) {
	for (i32 i = 0; i < level->count; i++) {
		if (OPstringEquals(name, level->entities[i].name)) {
			return &level->entities[i];
		}
	}
	return NULL;
}

OPint OPlevelload(const OPchar* filename, OPLevel** level) {
	OPstream* str = OPreadFile(filename);

	ui16 version = OPreadui16(str);
	if (version == 1) {
		ui16 count = OPreadui16(str);
		OPLevelEntity* entities = (OPLevelEntity*)OPalloc(sizeof(OPLevelEntity)* count);

		for (i32 i = 0; i < count; i++) {

			entities[i].name = OPreadstring(str);
			if (OPreadui16(str)) { // Has Resource?
				entities[i].resource = OPreadstring(str);
			}

			entities[i].position = OPvec3str(str);
			entities[i].rotation = OPvec3str(str);
			entities[i].scale = OPvec3str(str);

			entities[i].collision = OPreadui16(str);
			entities[i].physics = OPreadui16(str);

		}

		(*level) = (OPLevel*)OPalloc(sizeof(OPLevel));
		(*level)->count = count;
		(*level)->entities = entities;
		return 1;
	}

	return 0;
}

OPint OPlevelReload(const OPchar* filename, OPLevel** level) {
	return 1;
}

OPint OPlevelUnload(void* level) {
	OPLevel* lvl = (OPLevel*)level;
	OPfree(lvl->entities);
	OPfree(lvl);
	return 0;
}