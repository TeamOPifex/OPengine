#pragma once

struct OPfont;
typedef struct OPfont OPfont;

#include "./Human/include/Rendering/Font/OPfontAtlas.h"
#include "./Human/include/Rendering/Font/OPfontBuiltTextNode.h"
#include "./Human/include/Rendering/Font/OPfontUserTextNode.h"

#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Rendering/Font/OPfontGlyph.h"
#include "./Math/include/OPvec2.h"
#include "./Human/include/Rendering/OPMvertex.h"

struct OPfont {
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

	OPfontUserTextNode dummyTextNode;
	OPvector* vertices;
	OPvector* indices;
	OPvertexLayout vertexLayout;

	void Init();
	void Destroy();

	OPfontGlyph* GetGlyph(OPchar charcode);
	OPmodel CreateText(OPchar* text);
	OPfontBuiltTextNode CreatePackedText(const OPchar* text);
	OPfontBuiltTextNode CreatePackedText(const OPchar* text, OPfloat scale);
	OPfontUserTextNode CreateUserText(const OPchar* text);
	OPfontUserTextNode CreateUserText(const OPchar* text, OPfloat scale);
	OPvec2 GetSize(const OPchar* text, OPfloat scale);
};

OPint OPfontLoad(OPstream* str, OPfont** data);
OPint OPfontUnload(OPfont* font);
