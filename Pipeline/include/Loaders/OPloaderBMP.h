#pragma once

#include "./Human/include/Rendering/OPtexture.h"
#include "./Data/include/OPcman.h"

OPint OPimageBMPLoad(OPstream* str, OPtexture** image);
OPint OPimageBMPReload(OPstream* str, OPtexture** image);
OPint OPimageBMPUnload(OPtexture* image);

extern OPassetLoader OPASSETLOADER_BMP;