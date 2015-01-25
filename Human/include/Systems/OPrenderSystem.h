#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/OPcam.h"
#include "./Human/include/Rendering/OPrenderBuffer.h"
#include "./Human/include/Rendering/OPmesh.h"
#include "./Human/include/Rendering/OPmeshPacked.h"
#include "./Human/include/Rendering/OPmeshPacker.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPframeBuffer.h"

#include "./Human/include/Rendering/Primitives/OPquad.h"
#include "./Human/include/Rendering/Primitives/OPsphericalCube.h"

OPeffect OPrenderSystemLoadTextured2D(ui32 vertexStride);
OPeffect OPrenderSystemLoadTextured3D(ui32 vertexStride);