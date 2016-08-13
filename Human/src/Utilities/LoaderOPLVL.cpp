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
	OPstream* str = OPfile::ReadFromFile(filename);

	ui16 version = str->UI16();
	if (version == 1) {
		ui16 count = str->UI16();
		OPLevelEntity* entities = (OPLevelEntity*)OPalloc(sizeof(OPLevelEntity)* count);

		for (i32 i = 0; i < count; i++) {

			entities[i].name = str->String();
			if (str->UI16()) { // Has Resource?
				entities[i].resource = str->String();
			}

			entities[i].position = OPvec3Read(str);
			entities[i].rotation = OPvec3Read(str);
			entities[i].scale = OPvec3Read(str);

			entities[i].collision = str->UI16();
			entities[i].physics = str->UI16();

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