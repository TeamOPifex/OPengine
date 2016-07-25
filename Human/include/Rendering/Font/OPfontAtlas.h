#ifndef OPENGINE_HUMAN_RENDERER_FONT_ATLAS
#define OPENGINE_HUMAN_RENDERER_FONT_ATLAS

#include "./Data/include/OPvector.h"
#include "./Core/include/OPtypes.h"

struct OPfontAtlasNode {
	i32 x;
	i32 y;
	i32 z;
};
typedef struct OPfontAtlasNode OPfontAtlasNode;

struct OPfontAtlasRegion {
	i32 x;
	i32 y;
	i32 width;
	i32 height;
};
typedef struct OPfontAtlasRegion OPfontAtlasRegion;

struct OPfontAtlas {
	OPvector* nodes;
	i32 width;
	i32 height;
	OPint depth;
	OPint used;
	ui8* data;
};
typedef struct OPfontAtlas OPfontAtlas;

#include "./Human/include/Rendering/Font/OPfont.h"
#include "./Human/include/Rendering/OPtexture.h"

OPfontAtlas* OPfontAtlasCreate(i32 width, i32 height, OPint depth);
void OPfontAtlasDestroy(OPfontAtlas* atlas);
void OPfontAtlasSetRegion(OPfontAtlas* atlas, OPint x, OPint y, OPint width, OPint height, ui8* data, OPint stride);
OPint OPfontAtlasFit(OPfontAtlas* atlas, OPint index, OPint width, OPint height);
void OPfontAtlasMerge(OPfontAtlas* atlas);
OPfontAtlasRegion OPfontAtlasGetRegion(OPfontAtlas* atlas, i32 width, i32 height);
OPtextureOLD OPfontAtlasTexture(OPfontAtlas* atlas);
void OPfontAtlasSavePNG(OPfontAtlas* atlas, OPchar* filename);

#endif
