#pragma once

#include "./Data/include/OPstream.h"
#include "./Data/include/OPcman.h"

OPint OPMloader(OPstream* str, void** model);
OPint OPMreload(OPstream* str, void** model);
OPint OPMunload(void* model);

extern OPassetLoader OPASSETLOADER_OPM;
