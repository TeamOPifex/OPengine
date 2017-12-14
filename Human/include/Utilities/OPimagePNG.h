#pragma once

#include "./Human/include/Rendering/Enums/OPtextureFilter.h"
#include "./Data/include/OPstream.h"
struct OPtexture;
struct OPimage;

void OPimagePNG24WriteStream(ui8* imageData, i32 width, i32 height, ui8** data, OPuint* dataSize);
void OPimagePNG32WriteStream(ui8* imageData, i32 width, i32 height, ui8** data, OPuint* dataSize);
void OPimagePNGCreate24(ui8* imageData, i32 width, i32 height, OPchar* filename);
void OPimagePNGCreate32(ui8* imageData, i32 width, i32 height, OPchar* filename);
i32 OPimagePNGLoadStream(OPstream* str, OPuint offset, OPtexture** image, OPtextureFilter::Enum filter);
i32 OPimagePNGLoadStream(OPstream* str, OPuint offset, OPtexture** image);
OPimage OPimagePNGLoadData(const OPchar* filename);
