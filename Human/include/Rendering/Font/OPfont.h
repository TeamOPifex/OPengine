#ifndef OPENGINE_HUMAN_RENDERER_FONT
#define OPENGINE_HUMAN_RENDERER_FONT

#include "./Human/include/Rendering/Font/OPfontAtlas.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Rendering/OPmeshPacked.h"
#include "./Human/include/Rendering/Font/OPfontGlyph.h"
#include "./Math/include/OPvec2.h"
#include "./Human/include/Rendering/OPMvertex.h"

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

typedef struct {
	OPmesh mesh;
	OPfloat Width;
} OPfontUserTextNode;

void OPfontLoad(OPchar* filename, OPfont** data);
void OPfontUnload(OPfont* font);
OPfontGlyph* OPfontGetGlyph(OPfont* font, OPchar charcode);
OPmesh OPfontCreateText(OPfont* font, OPchar* text);
OPfontBuiltTextNode OPfontCreatePackedText(OPfont* font, const OPchar* text);
OPfontUserTextNode OPfontCreateUserText(OPfont* font, const OPchar* text);

#endif