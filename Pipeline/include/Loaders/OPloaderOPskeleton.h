#ifndef OPENGINE_PIPELINE_LOADERS_OPSKELETON
#define OPENGINE_PIPELINE_LOADERS_OPSKELETON

#include "./Human/include/Rendering/Skinning/OPskeleton.h"

void OPskeletonAddLoader();
OPint OPloaderOPskeletonLoad(OPstream* filename, OPskeleton** skeleton);
OPint OPloaderOPskeletonUnload(OPskeleton* skeleton);

#endif