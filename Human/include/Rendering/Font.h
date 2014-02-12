#pragma once

#include "./Human/include/Rendering/FontAtlas.h"
#include "./Human/include/Rendering/Mesh.h"
#include "./Math/include/Vector2.h"
#include "./Human/include/Rendering/Texture.h"
#include "./Human/include/Rendering/MeshPacked.h"
#include "./Human/include/Rendering/FontGlyph.h"

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

void OPfontLoad(i8* filename, OPfont** data);
void OPfontUnload(OPfont* font);
OPfontGlyph* OPfontGetGlyph(OPfont* font, i8 charcode);
OPmesh OPfontCreateText(OPfont* font, i8* text);
OPfontBuiltTextNode OPfontCreatePackedText(OPfont* font, const i8* text);

