#pragma once

#include "./Human/include/Rendering/Skinning/OPskeleton.h"

void OPskeletonAddLoader();
OPint OPloaderOPskeletonLoad(OPstream* str, OPskeleton** skeleton);
OPint OPloaderOPskeletonUnload(OPskeleton* skeleton);