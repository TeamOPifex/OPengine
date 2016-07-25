#ifndef OPENGINE_HUMAN_RENDERER_MESH_PACKER
#define OPENGINE_HUMAN_RENDERER_MESH_PACKER

#include "./Human/include/Rendering/OPrenderBuffer.h"
#include "./Data/include/OPstream.h"
#include "./Core/include/OPmemory.h"

//-----------------------------------------------------------------------------
//   _____ _                   _
//  / ____| |                 | |
// | (___ | |_ _ __ _   _  ___| |_ ___
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
struct OPmeshPacker{
	OPvertexBuffer VertexBuffer;
	OPindexBuffer IndexBuffer;
	OPuint vertexElementOffset;
	OPuint vertexOffset;
	OPuint indexOffset;
	OPstream vertices;
	OPstream indices;
	OPint built;
};
typedef struct OPmeshPacker OPmeshPacker;

//-----------------------------------------------------------------------------
//   _____ _       _           _
//  / ____| |     | |         | |
// | |  __| | ___ | |__   __ _| |___
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
extern OPmeshPacker* OPMESHPACKER_ACTIVE;

//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _
// |  __ \                   |  __ \                |  __ \(_)             | | (_)
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/


//-----------------------------------------------------------------------------
// ______                _   _
//|  ____|              | | (_)
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPmeshPacker OPmeshPackerCreate();
OPint OPmeshPackerDestroy(OPmeshPacker* packer);

OPuint OPmeshPackerAddVB(ui32 vertexSize, void* verticesData, OPuint vertexCount);
OPuint OPmeshPackerAddIB(OPindexSize indexSize, void* indicesData, OPuint indexCount);
void OPmeshPackerBuild();
void OPmeshPackerBind(OPmeshPacker* packer);


#endif
