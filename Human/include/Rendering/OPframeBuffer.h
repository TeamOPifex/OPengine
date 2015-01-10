#ifndef OPENGINE_HUMAN_RENDERER_FRAMEBUFFER
#define OPENGINE_HUMAN_RENDERER_FRAMEBUFFER

#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Core/include/OPtypes.h"


//-----------------------------------------------------------------------------
//   _____ _                   _       
//  / ____| |                 | |      
// | (___ | |_ _ __ _   _  ___| |_ ___ 
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
typedef struct{
	OPtextureDescription Description;
	OPtexture Texture;
	ui32 Handle;
}OPframeBuffer;

//-----------------------------------------------------------------------------
//   _____ _       _           _     
//  / ____| |     | |         | |    
// | |  __| | ___ | |__   __ _| |___ 
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
extern IMPORT_METHOD OPframeBuffer* OPRENDER_CURR_FRAMEBUFFER;

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

EXPORT_METHOD OPframeBuffer OPframeBufferCreateDepth(OPtextureDescription desc);
EXPORT_METHOD OPframeBuffer OPframeBufferCreate(OPtextureDescription desc);
EXPORT_METHOD void OPframeBufferDestroy(OPframeBuffer* fb);
EXPORT_METHOD void OPframeBufferAttach(OPtexture* texture, ui16 pos);
EXPORT_METHOD void OPframeBufferBind(OPframeBuffer* fb);
EXPORT_METHOD void OPframeBufferBindRead(OPframeBuffer* fb);
EXPORT_METHOD void OPframeBufferSetReadBuffer(ui16 pos);
EXPORT_METHOD void OPframeBufferSetReadBufferDepth();
EXPORT_METHOD void OPframeBufferBindTex(OPframeBuffer* fb);
EXPORT_METHOD void OPframeBufferUnbind();
EXPORT_METHOD void OPframeBufferAttachDepth(OPtexture* texture);

#endif
