#ifndef OPEngine_Human_Renderer_Mesh_Packer
#define OPEngine_Human_Renderer_Mesh_Packer
	
#include "./Core/include/DynamicMemory.h"
#include "./Data/include/Stream.h"
#include "./Human/include/Rendering/OPbuffer.h"

// prevent name mangling if compiling with c++
#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
typedef struct{
	OPrenderBuffer VertexBuffer;
	OPrenderBuffer IndexBuffer;
	ui32 vertexElementOffset;
	ui32 vertexOffset;
	ui32 indexOffset;
	OPstream vertices;
	OPstream indices;
	bool built;
}OPmeshPacker;

//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
extern OPmeshPacker* OPRENDER_CURR_PACKER;

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

ui32 OPmeshPackerAddVB(ui32 vertexSize, void* verticesData, ui32 vertexCount);
ui32 OPmeshPackerAddIB(ui32 indexSize, void* indicesData, ui32 indexCount);
void OPmeshPackerBuild();
void OPmeshPackerBind(OPmeshPacker* packer);

// prevent name mangling if compiling with c++
#ifdef __cplusplus
};
#endif

#endif