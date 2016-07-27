#ifndef OPENGINE_HUMAN_RENDERING_API_RENDERER
#define OPENGINE_HUMAN_RENDERING_API_RENDERER

#include "./Human/include/Rendering/OPwindow.h"
#include "./Math/include/OPvec4.h"

struct OPrenderer;
typedef struct OPrenderer OPrenderer;

#include "./Human/include/Rendering/API/OPcontext.h"
#include "./Human/include/Rendering/API/OPframeBuffer2D.h"
#include "./Human/include/Rendering/API/OPframeBufferDepth.h"
#include "./Human/include/Rendering/API/OPindexBufferAPI.h"
#include "./Human/include/Rendering/API/OPshaderAPI.h"
#include "./Human/include/Rendering/API/OPshaderResource.h"
#include "./Human/include/Rendering/API/OPshaderUniformAPI.h"
#include "./Human/include/Rendering/API/OPtextureAPI.h"
#include "./Human/include/Rendering/API/OPtexture2D.h"
#include "./Human/include/Rendering/API/OPtextureCube.h"
#include "./Human/include/Rendering/API/OPtextureDepth.h"
#include "./Human/include/Rendering/API/OPvertexArray.h"
#include "./Human/include/Rendering/API/OPvertexBufferAPI.h"

struct OPrenderer {
	i8(*Init)(OPwindow* window) = 0;
	void(*_Clear)(OPvec4 color) = 0;
	void(*Present)() = 0;
	void(*SetDepthTesting)(bool state) = 0;
	void(*SetDepthWrite)(bool state) = 0;
	void(*SetCull)(bool state) = 0;
	void(*SetCullMode)(i8 state) = 0;
	void(*SetBlend)(bool state) = 0;
	void(*SwapBuffer)() = 0;
	void(*Shutdown)() = 0;

	OPcontext Context;
	OPframeBuffer2D FrameBuffer2D;
	OPframeBufferDepth FrameBufferDepth;
	OPindexBufferAPI IndexBuffer;
	OPshaderAPI Shader;
	OPshaderResource ShaderResource;
	OPshaderUniformAPI ShaderUniform;
	OPtextureAPI Texture;
	OPtexture2D Texture2D;
	OPtextureCube TextureCube;
	OPtextureDepth TextureDepth;
	OPvertexArray VertexArray;
	OPvertexBufferAPI VertexBuffer;

	inline void Clear(OPvec4 v) { _Clear(v); }
	inline void Clear(OPfloat r) { _Clear(OPvec4(r)); }
	inline void Clear(OPfloat r, OPfloat g, OPfloat b) { _Clear(OPvec4(r, g, b, 1.0)); }
	inline void Clear(OPfloat r, OPfloat g, OPfloat b, OPfloat a) { _Clear(OPvec4(r, g, b, a)); }
	inline void Clear(OPvec3 v) { _Clear(OPvec4(v.x, v.y, v.z, 1.0)); }
};

#endif