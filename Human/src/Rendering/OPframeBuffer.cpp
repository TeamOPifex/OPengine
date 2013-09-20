#include "./Human/include/Rendering/OPframeBuffer.h"

//-----------------------------------------------------------------------------
// ______                _   _                 
//|  ____|              | | (_)                
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
OPframeBuffer OPframeBufferCreate(OPtextureDescription desc){
	OPframeBuffer fb = {
		desc,
		OPtextureCreate(desc),
		0
	};

	glGenFrameBuffers(1, &fb.Handle);
	OPtextureBind(&fb.Handle, 0);
	OPtextureSetData(NULL);

	return fb;
}
//-----------------------------------------------------------------------------
void OPframeBufferDestroy(OPframeBuffer* fb){
	OPtextureDestroy(&fb->Texture);
	glDeleteFrameBuffers(1, &fb->Handle);
}
//-----------------------------------------------------------------------------
void OPframeBufferBind(OPframeBuffer* fb){
	OPRENDER_CURR_FRAMEBUFFER = fb;
	
}
//-----------------------------------------------------------------------------
void OPframeBufferBindTex(OPframeBuffer* fb, OPint slot){

}
//-----------------------------------------------------------------------------
void OPframeBufferUnbind(){

}

