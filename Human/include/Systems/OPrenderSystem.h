#pragma once

#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/OPcam.h"
#include "./Human/include/Rendering/OPrenderCommandBucket.h"
#include "./Human/include/Rendering/Camera/OPcamFreeFlight.h"
#include "./Human/include/Rendering/OPrenderBuffer.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPmeshBuilder.h"
#include "./Human/include/Rendering/OPmodel.h"
#include "./Human/include/Rendering/OPmaterial.h"
#include "./Human/include/Rendering/OPmeshPacked.h"
#include "./Human/include/Rendering/OPmeshPacker.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPframeBuffer.h"
#include "./Human/include/Rendering/UI/OPimgui.h"
#include "./Human/include/Rendering/OPimage.h"
#include "./Human/include/Rendering/OPwindow.h"
#include "./Human/include/Rendering/OPwindowSystem.h"
#include "./Human/include/Rendering/OPscene.h"
#include "./Human/include/Rendering/OPrenderer.h"
#include "./Human/include/Rendering/OPrendererForward.h"

#include "./Human/include/Rendering/Commands/OPrenderCommandDrawIndexed.h"

#include "./Human/include/Rendering/Primitives/OPquad.h"
#include "./Human/include/Rendering/Primitives/OPcube.h"
#include "./Human/include/Rendering/Primitives/OPsphericalCube.h"

#include "./Human/include/Rendering/Skinning/OPskeletonAnimationTransition.h"
#include "./Human/include/Rendering/Skinning/OPskeletonAnimationManager.h"

#include "./Human/include/Utilities/OPimagePNG.h"

OPeffect OPrenderSystemLoadTextured2D(ui32 vertexStride);
OPeffect OPrenderSystemLoadTextured3D(ui32 vertexStride);
