#pragma once

struct OPspriteSheet;
typedef struct OPspriteSheet OPspriteSheet;

#include "./Human/include/Rendering/Sprite/OPsprite.h"
#include "./Data/include/OPstream.h"

// Used to maintain and clean up sprites that
// have been stored in the manager
struct OPspriteSheet {
	OPint      Width;
	OPint      Height;
	OPint      Sprites;
	OPchar**   Names;
	OPtexture* Sheet;
};


OPint OPspriteSheetLoad(OPstream* str, OPspriteSheet** ss);
OPint OPspriteSheetUnload(void* ss);