#pragma once

#include "./Data/include/OPstream.h"
#include "./Data/include/OPcman.h"
#include "./Human/include/Rendering/OPmodel.h"

OPint OPMloader(OPstream* str, void** model);
OPint OPMreload(OPstream* str, void** model);
OPint OPMunload(OPmodel* model);

extern OPassetLoader OPASSETLOADER_OPM;
