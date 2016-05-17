#ifndef OPENGINE_HUMAN_RENDERER_FRAMEBUFFER
#define OPENGINE_HUMAN_RENDERER_FRAMEBUFFER

#include "./Human/include/Rendering/OPtexture.h"
#include "OPrender.h"
#include "./Core/include/OPtypes.h"


//-----------------------------------------------------------------------------
//   _____ _                   _
//  / ____| |                 | |
// | (___ | |_ _ __ _   _  ___| |_ ___
//  \___ \| __| '__| | | |/ __| __/ __|
//  ____) | |_| |  | |_| | (__| |_\__ \
// |_____/ \__|_|   \__,_|\___|\__|___/
struct OPframeBuffer;
typedef struct OPframeBuffer OPframeBuffer;

//-----------------------------------------------------------------------------
//   _____ _       _           _
//  / ____| |     | |         | |
// | |  __| | ___ | |__   __ _| |___
// | | |_ | |/ _ \| '_ \ / _` | / __|
// | |__| | | (_) | |_) | (_| | \__ \
//  \_____|_|\___/|_.__/ \__,_|_|___/
extern OPframeBuffer* OPRENDER_CURR_FRAMEBUFFER;

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
OPframeBuffer OPframeBufferCreateShadow(ui32 width, ui32 height);
OPframeBuffer OPframeBufferCreateDepth(OPtextureDescription desc);
OPframeBuffer OPframeBufferCreate(OPtextureDescription desc);
OPframeBuffer OPframeBufferCreateVR(ui32 width, ui32 height);
void OPframeBufferDestroy(OPframeBuffer* fb);
void OPframeBufferAttach(OPtexture* texture, ui16 pos);
void OPframeBufferBind(OPframeBuffer* fb);
void OPframeBufferBindRead(OPframeBuffer* fb);
void OPframeBufferSetReadBuffer(ui16 pos);
void OPframeBufferSetReadBufferDepth();
void OPframeBufferBindTex(OPframeBuffer* fb);
void OPframeBufferUnbind();
void OPframeBufferAttachDepth(OPtexture* texture);



struct OPframeBuffer {
	OPtextureDescription Description;
	OPtexture Texture;
	ui32 Handle;

	ui32 DepthBufferId;
	ui32 RenderTextureId;
	ui32 RenderFramebufferId;
	ui32 ResolveTextureId;
	ui32 ResolveFramebufferId;

	void Bind() {
		OPframeBufferBind(this);
	}

	void Unbind() {
		OPframeBufferUnbind();
	}
};

#endif
