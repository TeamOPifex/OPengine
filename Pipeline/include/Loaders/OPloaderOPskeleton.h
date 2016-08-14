#pragma once

#include "./Human/include/Rendering/Skinning/OPskeleton.h"

void OPskeletonAddLoader();
OPint OPloaderOPskeletonLoad(OPstream* filename, OPskeleton** skeleton);
OPint OPloaderOPskeletonUnload(OPskeleton* skeleton);