#pragma once

#include "./Human/include/Rendering/OPfontAtlas.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Math/include/Vector2.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Rendering/OPmeshPacked.h"

typedef struct {
	OPvector* glyphs;
	OPfontAtlas* atlas;
	OPtexture* texture;

	// Font Size
	OPfloat size;

	// Use AutoHint
	OPint hinting;

	// Outline Types (0 - None, 1 - Line, 2 - Inner, 3 - Outer)
	OPint outlineType;
	OPfloat outlineThickness;

	// Use LCD Filtering
	OPint filtering;
	ui8 lcdWeights[5];

	// Use Kerning
	OPint kerning;

	// Default Line Spacing
	OPfloat height;

	// ascender - descender + linegap
	OPfloat lineGap;

	// vertical distance from the horizontal baseline
	OPfloat ascender;
	// vertical distance from the horizontal baseline
	OPfloat descender;

	OPfloat underlinePosition;
	OPfloat underlineThickness;
} OPfont;

typedef struct {
	OPmeshPacked* packedMesh;
	OPfloat Width;
} OPfontBuiltTextNode;

#include "./Human/include/Rendering/OPfontGlyph.h"

//OPfont* OPfontFromFile(OPfontAtlas* atlas, OPfloat ptSize, i8* filename);
void OPfontLoad(i8* filename, OPfont** data);
void OPfontUnload(OPfont* font);
//OPint OPfontLoadGlyphs(OPfont* font, i8* charcodes, OPint count);
OPfontGlyph* OPfontGetGlyph(OPfont* font, i8 charcode);
OPmesh OPfontCreateText(OPfont* font, i8* text);
OPfontBuiltTextNode OPfontCreatePackedText(OPfont* font, const i8* text);

