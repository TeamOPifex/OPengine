#pragma once

#include "./Human/include/Rendering/Skinning/OPskeletonAnimation.h"

void OPskeletonAnimationAddLoader();
OPint OPloaderOPanimationLoad(OPstream* filename, OPskeletonAnimation** animation);
OPint OPloaderOPanimationUnload(OPskeletonAnimation* animation);