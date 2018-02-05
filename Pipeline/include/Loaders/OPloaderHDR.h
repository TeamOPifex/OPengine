#pragma once

#include "./Human/include/Rendering/OPtexture.h"
#include "./Data/include/OPcman.h"

i32 OPimageHDRLoad(OPstream* str, OPtexture** image);
i32 OPimageHDRReload(OPstream* str, OPtexture** image);
i32 OPimageHDRUnload(OPtexture* image);

extern OPassetLoader OPASSETLOADER_HDR;