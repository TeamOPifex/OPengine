#pragma once

struct OPfontGlyph;
typedef struct OPfontGlyph OPfontGlyph;

#include "./Human/include/Rendering/Font/OPfontKerning.h"
#include "./Math/include/OPvec4.h"
#include "./Data/include/OPvector.h"

struct OPfontGlyph {
	OPchar charcode;
	OPint width;
	OPint height;
	OPint offsetX;
	OPint offsetY;
	OPfloat advanceX;
	OPfloat advanceY;
	OPvec4 textureCoordinates;
	OPfontKerning* kerning;
	ui32 kerningCount = 0;
	OPint outlineType;
	OPfloat outlineThickness;

	void Init();
	OPfloat GetKerning(OPchar charcode);
	void Destroy();

	inline static OPfontGlyph* Create() {
		OPfontGlyph* result = OPNEW(OPfontGlyph());
		result->Init();
		return result;
	}
};
