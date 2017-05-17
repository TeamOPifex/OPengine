#pragma once

#include "./Human/include/Rendering/OPtexture.h"
#include "./Data/include/OPcman.h"

OPint OPloaderTextureFromStreamOffset(OPstream* str, OPuint offset, OPtexture** image, OPtextureFilter filter);
OPint OPloaderTextureFromFile(const OPchar* filename, OPtexture** image);
OPint OPloaderTextureFromStream(OPstream* str, OPtexture** image);
OPint OPloaderTextureReload(OPstream* str, OPtexture** image);
OPint OPloaderTextureUnload(OPtexture* image);

extern OPassetLoader OPASSETLOADER_TEXTURE_JPG;
extern OPassetLoader OPASSETLOADER_TEXTURE_JPEG;
extern OPassetLoader OPASSETLOADER_TEXTURE_PNG;
extern OPassetLoader OPASSETLOADER_TEXTURE_TGA;
extern OPassetLoader OPASSETLOADER_TEXTURE_BMP;
extern OPassetLoader OPASSETLOADER_TEXTURE_PSD;
extern OPassetLoader OPASSETLOADER_TEXTURE_GIF;
