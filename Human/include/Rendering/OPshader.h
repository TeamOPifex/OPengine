#ifndef OPENGINE_HUMAN_RENDERER_SHADER
#define OPENGINE_HUMAN_RENDERER_SHADER

#include "./Human/include/Rendering/OpenGL.h"
#include "./Core/include/OPtypes.h"

//-----------------------------------------------------------------------------
//  _____                     _____                  _____  _               _   _               
// |  __ \                   |  __ \                |  __ \(_)             | | (_)              
// | |__) _ __ ___   ______  | |__) _ __ ___   ___  | |  | |_ _ __ ___  ___| |_ ___   _____ ___ 
// |  ___| '__/ _ \ |______| |  ___| '__/ _ \ / __| | |  | | | '__/ _ \/ __| __| \ \ / / _ / __|
// | |   | | |  __/          | |   | | | (_) | (__  | |__| | | | |  __| (__| |_| |\ V |  __\__ \
// |_|   |_|  \___|          |_|   |_|  \___/ \___| |_____/|_|_|  \___|\___|\__|_| \_/ \___|___/   

typedef GLenum OPshader;

#ifdef OPIFEX_OPENGL_ES_2
#define OPvertexShader   GL_VERTEX_SHADER
#define OPfragmentShader GL_FRAGMENT_SHADER
#else
#define OPvertexShader   GL_VERTEX_SHADER
#define OPfragmentShader GL_FRAGMENT_SHADER
#endif


//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/

typedef struct{
	const OPchar* Name;
	ui32         Type;
	ui32         Elements;
	OPuint        Offset;
	OPuint		Handle;
}OPshaderAttribute;


//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/

OPint OPshaderLoadVertex(const OPchar* filename, OPshader** shader);
OPint OPshaderLoadFragment(const OPchar* filename, OPshader** shader);
OPint OPshaderUnload(OPshader* shader);


#endif