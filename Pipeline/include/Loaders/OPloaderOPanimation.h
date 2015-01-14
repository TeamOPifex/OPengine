#ifndef OPENGINE_PIPELINE_LOADERS_OPANIMATION
#define OPENGINE_PIPELINE_LOADERS_OPANIMATION

#include "./Human/include/Rendering/Skinning/OPskeletonAnimation.h"

void OPskeletonAnimationAddLoader();
OPint OPloaderOPanimationLoad(const OPchar* filename, OPskeletonAnimation** animation);
OPint OPloaderOPanimationUnload(OPskeletonAnimation* animation);

#endif