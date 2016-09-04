#pragma once

#include "./Human/include/Rendering/Font/OPfont.h"
#include "./Data/include/OPcman.h"

OPint OPfontLoad(OPstream* str, OPfont** data);
OPint OPfontReload(OPstream* str, OPfont** image);
OPint OPfontUnload(OPfont* font);

extern OPassetLoader OPASSETLOADER_OPF;