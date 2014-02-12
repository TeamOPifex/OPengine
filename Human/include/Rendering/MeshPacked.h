#ifndef OPEngine_Human_Renderer_Mesh_Packed
#define OPEngine_Human_Renderer_Mesh_Packed
	
#include "./Human/include/Rendering/MeshPacker.h"
#include "./Human/include/Rendering/Buffer.h"

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
	ui32 offset;
	ui32 count;
	ui32 elementCount;
}OPmeshPacked;

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
OPmeshPacked OPrenderCreateMeshPacked(ui32 vertSize, ui32 indSize, ui32 vertCount, ui32 indCount, void* vertices, void* indices);
void OPrenderMeshPacked(OPmeshPacked* mesh);

// prevent name mangling if compiling with c++
#ifdef __cplusplus
};
#endif

#endif