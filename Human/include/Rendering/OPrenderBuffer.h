#ifndef OPENGINE_HUMAN_RENDERER_BUFFER
#define OPENGINE_HUMAN_RENDERER_BUFFER

#include "./Human/include/Rendering/OpenGL.h"
#include "./Core/include/OPtypes.h"


//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
typedef struct{
	ui32 Type;
	ui32 ElementSize;
	ui32 ElementCount;
	ui32 Handle;
}OPrenderBuffer;

//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
extern IMPORT_METHOD OPrenderBuffer* OPRENDER_CURR_VB;
extern IMPORT_METHOD OPrenderBuffer* OPRENDER_CURR_IB;

//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _               
// |  __ \                   |  __ \                |  __ \(_)             | | (_)              
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___ 
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/   
#define OPvertexBuffer GL_ARRAY_BUFFER
#define OPindexBuffer  GL_ELEMENT_ARRAY_BUFFER

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
EXPORT_METHOD OPrenderBuffer OPrenderGenBuffer(ui32 type);
EXPORT_METHOD void OPrenderDelBuffer(OPrenderBuffer* buff);
EXPORT_METHOD void OPrenderSetBufferData(OPrenderBuffer* buff, ui32 elementSize, ui32 count, const void* data);
EXPORT_METHOD void OPrenderSetBufferSubData(OPrenderBuffer* buff, ui32 elementSize, ui32 offsetCount, ui32 count, const void* data);
EXPORT_METHOD void OPrenderBindBuffer(OPrenderBuffer* buffer);

EXPORT_METHOD void OPrenderDrawBufferIndexed(ui32 offset);
EXPORT_METHOD void OPrenderDrawBuffer(ui32 offset);
EXPORT_METHOD void OPrenderDrawIndexed(ui32 offset, ui32 count);
EXPORT_METHOD void OPrenderDrawUserArray(void* vertices, ui32 attrs, ui32 offset, ui32 count);


#endif