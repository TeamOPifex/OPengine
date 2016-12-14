#pragma once

#include "./Core/include/OPtypes.h"

OPchar* OPdirCurrent();
OPchar* OPdirExecutable();
void OPdirFiles(const OPchar* dir, OPchar*** files, ui32* count);