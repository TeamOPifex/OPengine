#pragma once

#include "./Human/include/Rendering/OPtexture.h"
#include "./Data/include/OPcman.h"

i32 OPimageTGALoad(OPstream* str, OPtexture** image);
i32 OPimageTGAReload(OPstream* str, OPtexture** image);
i32 OPimageTGAUnload(void* image);

extern OPassetLoader OPASSETLOADER_TGA;