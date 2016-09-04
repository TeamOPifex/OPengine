#pragma once

#include "./Human/include/Rendering/OPtexture.h"
#include "./Data/include/OPcman.h"

i32 OPimagePNGLoad(OPstream* str, OPtexture** image);
i32 OPimagePNGReload(OPstream* str, OPtexture** image);
i32 OPimagePNGUnload(OPtexture* image);

extern OPassetLoader OPASSETLOADER_PNG;