#pragma once

#include "./Human/include/Rendering/OPtexture.h"

i32 OPimageBMPLoad(OPstream* str, OPtexture** image);
i32 OPimageBMPReload(OPstream* str, OPtexture** image);
i32 OPimageBMPUnload(void* image);
