#include "./Human/include/Rendering/Renderer.h"
#include "./Human/include/Rendering/Camera.h"
#include "./Human/include/Rendering/Buffer.h"
#include "./Human/include/Rendering/Mesh.h"
#include "./Human/include/Rendering/MeshPacked.h"
#include "./Human/include/Rendering/MeshPacker.h"
#include "./Human/include/Rendering/Effect.h"
#include "./Human/include/Rendering/FrameBuffer.h"

#include "./Human/include/Rendering/Primitives/Quad.h"

OPeffect OPeffectLoadTextured2D(ui32 vertexStride);
OPeffect OPeffectLoadTextured3D(ui32 vertexStride);