#pragma once

#include "./Human/include/Rendering/OPfont.h"

#include <ft2build.h>
#include <freetype.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
// #include FT_ADVANCES_H
#include FT_LCD_FILTER_H

#define HRES  64
#define HRESf 64.f
#define DPI   72

static OPint OPfontLoadFace(OPfont* font, i8* filename, OPfloat size, FT_Library *library, FT_Face *face)
{
	FT_Error error;
	FT_Matrix matrix = {
		(int)((1.0 / HRES) * 0x10000L),
		(int)((0.0) * 0x10000L),
		(int)((0.0) * 0x10000L),
		(int)((1.0) * 0x10000L) };

	/* Initialize library */
	error = FT_Init_FreeType(library);
	if (error) {
		return 0;
	}

	/* Load face */
	error = FT_New_Face(*library, filename, 0, face);

	if (error) {
		FT_Done_FreeType(*library);
		return 0;
	}

	/* Select charmap */
	error = FT_Select_Charmap(*face, FT_ENCODING_UNICODE);
	if (error) {
		FT_Done_Face(*face);
		FT_Done_FreeType(*library);
		return 0;
	}

	/* Set char size */
	error = FT_Set_Char_Size(*face, (int)(size * HRES), 0, DPI * HRES, DPI);

	if (error) {
		FT_Done_Face(*face);
		FT_Done_FreeType(*library);
		return 0;
	}

	/* Set transform matrix */
	FT_Set_Transform(*face, &matrix, NULL);

	return 1;
}

static OPint OPfontGetFaceSize(OPfont* font, i8* filename, OPfloat size, FT_Library *library, FT_Face *face)
{
	return OPfontLoadFace(font, filename, size, library, face);
}

static OPint OPfontGetFace(OPfont* font, i8* filename, FT_Library *library, FT_Face *face)
{
	return OPfontGetFaceSize(font, filename, font->size, library, face);
}

static OPint OPfontGetHiResFace(OPfont* font, i8* filename, FT_Library *library, FT_Face *face)
{
	return OPfontGetFaceSize(font, filename, font->size * 100.f, library, face);
}