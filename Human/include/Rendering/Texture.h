#ifndef OPENGINE_HUMAN_RENDERER_TEXTURE
#define OPENGINE_HUMAN_RENDERER_TEXTURE

#include "./Human/include/Rendering/OpenGL.h"
#include "./Core/include/Types.h"

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
	ui16 Width, Height;
	ui32 InternalFormat, Format, DataType;
	ui32 MinFilter, MagFilter;
	ui32 WrapX, WrapY;
}OPtextureDescription;


typedef struct{
	OPtextureDescription Description;
	ui32 Handle;
}OPtexture;

//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
extern OPtexture* OPRENDER_CURR_TEX;

//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _               
// |  __ \                   |  __ \                |  __ \(_)             | | (_)              
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___ 
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/   
#define OPtextureNearest    GL_NEAREST
#define OPtextureLinear     GL_LINEAR
#define OPtextureNearestMip GL_NEAREST_MIPMAP_NEAREST
#define OPtextureLinearMip  GL_LINEAR_MIPMAP_LINEAR

#define OPtextureRepeat GL_REPEAT
#define OPtextureClamp  GL_CLAMP_TO_EDGE

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPtexture OPtextureCreate(OPtextureDescription desc);
void OPtextureDestroy(OPtexture* tex);
ui32 OPtextureBind(OPtexture* tex);
void OPtextureClearActive();
void OPtextureSetData(void* data);
void OPtexturePixelate();
void OPtextureSmooth();

// prevent name mangling if compiling with c++
#ifdef __cplusplus
};
#endif

#endif
 
