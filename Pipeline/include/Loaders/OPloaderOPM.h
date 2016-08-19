#pragma once

#include "./Data/include/OPstream.h"

OPint OPMloader(OPstream* str, void** model);
OPint OPMreload(OPstream* str, void** model);
OPint OPMunload(void* model);