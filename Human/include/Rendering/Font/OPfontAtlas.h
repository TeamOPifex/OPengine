#pragma once

struct OPfontAtlas;
typedef struct OPfontAtlas OPfontAtlas;

#include "./Human/include/Rendering/Font/OPfontAtlasNode.h"
#include "./Human/include/Rendering/Font/OPfontAtlasRegion.h"
#include "./Human/include/Rendering/Font/OPfont.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Data/include/OPvector.h"

struct OPfontAtlas {
	OPvector* nodes;
	i32 width;
	i32 height;
	OPint depth;
	OPint used;
	ui8* data;

	OPfontAtlas() { }
	OPfontAtlas(i32 width, i32 height, OPint depth) { Init(width, height, depth); }

	void Init(i32 width, i32 height, OPint depth);
	void SetRegion(OPint x, OPint y, OPint width, OPint height, ui8* data, OPint stride);
	OPint Fit(OPint index, OPint width, OPint height);
	void Merge();
	OPfontAtlasRegion GetRegion(i32 width, i32 height);
	OPtexture Texture();
	void SavePNG(OPchar* filename);
	void Destroy();

	inline static OPfontAtlas* Create(i32 width, i32 height, OPint depth) {
		return OPNEW(OPfontAtlas(width, height, depth));
	}
};