#ifndef OPEngine_Human_Renderer_Buffer
#define OPEngine_Human_Renderer_Buffer
	
#include "./Core/include/Types.h"

#ifdef OPIFEX_OPENGL_ES_2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#endif

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
extern OPrenderBuffer* OPRENDER_CURR_VB;
extern OPrenderBuffer* OPRENDER_CURR_IB;

//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _               
// |  __ \                   |  __ \                |  __ \(_)             | | (_)              
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___ 
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/   
#define OPvertexBuffer GL_ARRAY_BUFFER
#define OPindexBuffer  GL_ELEMENT_ARRAY_BUFFER

//#define OPrenderBindBuffer(buffer){\
//	glBindbuffer(buffer->Type, buffer->Handle);\
//	if(buffer->Type == OPvertexBuffer)\
//		OPRENDER_CURR_VB = buffer;\
//	else\
//		OPRENDER_CURR_IB = buffer;\
//}\
//
//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPrenderBuffer OPrenderGenBuffer(ui32 type);
void           OPrenderDelBuffer(OPrenderBuffer* buff);
void OPrenderSetBufferData(OPrenderBuffer* buff, ui32 elementSize, ui32 count, const void* data);
void OPrenderBindBuffer(OPrenderBuffer* buffer);

void OPrenderDrawBufferIndexed(ui32 offset);
void OPrenderDrawBuffer(ui32 offset);

// prevent name mangling if compiling with c++
#ifdef __cplusplus
};
#endif

#endif