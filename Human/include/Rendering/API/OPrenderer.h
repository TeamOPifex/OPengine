#ifndef OPENGINE_HUMAN_RENDERING_API_RENDERER
#define OPENGINE_HUMAN_RENDERING_API_RENDERER

#include "./Human/include/Rendering/API/OPcontext.h"
#include "./Human/include/Rendering/API/OPeffectAPI.h"
#include "./Human/include/Rendering/API/OPframeBuffer2D.h"
#include "./Human/include/Rendering/API/OPframeBufferDepth.h"
#include "./Human/include/Rendering/API/OPindexBufferAPI.h"
#include "./Human/include/Rendering/API/OPmonitorAPI.h"
#include "./Human/include/Rendering/API/OPshaderAPI.h"
#include "./Human/include/Rendering/API/OPshaderAttributeAPI.h"
#include "./Human/include/Rendering/API/OPshaderResource.h"
#include "./Human/include/Rendering/API/OPshaderUniformAPI.h"
#include "./Human/include/Rendering/API/OPtextureAPI.h"
#include "./Human/include/Rendering/API/OPtexture2D.h"
#include "./Human/include/Rendering/API/OPtextureCube.h"
#include "./Human/include/Rendering/API/OPtextureDepth.h"
#include "./Human/include/Rendering/API/OPvertexArrayAPI.h"
#include "./Human/include/Rendering/API/OPvertexBufferAPI.h"
#include "./Human/include/Rendering/API/OPwindowAPI.h"
#include "./Human/include/Rendering/Enums/OPblendFunction.h"
#include "./Math/include/OPvec4.h"

struct OPrenderAPI {
	i8(*Init)(OPwindow* window) = 0;
	void(*_Clear)(OPvec4 color) = 0;
	void(*Present)() = 0;
	void(*SetDepthTesting)(bool state) = 0;
	void(*SetDepthWrite)(bool state) = 0;
	void(*SetCull)(bool state) = 0;
	void(*SetCullMode)(i8 state) = 0;
	void(*SetBlend)(bool state) = 0;
	void(*SetBlendMode)(OPblendFunction src, OPblendFunction dst) = 0;
	void(*SetViewport)(OPuint x, OPuint y, OPuint width, OPuint height) = 0;
	void(*SwapBuffer)() = 0;
	void(*Shutdown)() = 0;

	OPcontext Context;
	OPeffectAPI Effect;
	OPframeBuffer2D FrameBuffer2D;
	OPframeBufferDepth FrameBufferDepth;
	OPindexBufferAPI IndexBuffer;
	OPmonitorAPI Monitor;
	OPshaderAPI Shader;
	OPshaderAttributeAPI ShaderAttribute;
	OPshaderResource ShaderResource;
	OPshaderUniformAPI ShaderUniform;
	OPtextureAPI Texture;
	OPtexture2D Texture2D;
	OPtextureCube TextureCube;
	OPtextureDepth TextureDepth;
	OPvertexArrayAPI VertexArray;
	OPvertexBufferAPI VertexBuffer;
	OPwindowAPI Window;

	OPwindow* OPWINDOW_ACTIVE = 0;
	OPvertexArray* OPVERTEXARRAY_ACTIVE = 0;
	OPvertexBuffer* OPVERTEXBUFFER_ACTIVE = 0;
	OPindexBuffer* OPINDEXBUFFER_ACTIVE = 0;
	OPeffect* OPEFFECT_ACTIVE = 0;

	inline void Clear(OPvec4 v) { _Clear(v); }
	inline void Clear(OPfloat r) { _Clear(OPvec4(r)); }
	inline void Clear(OPfloat r, OPfloat g, OPfloat b) { _Clear(OPvec4(r, g, b, 1.0)); }
	inline void Clear(OPfloat r, OPfloat g, OPfloat b, OPfloat a) { _Clear(OPvec4(r, g, b, a)); }
	inline void Clear(OPvec3 v) { _Clear(OPvec4(v.x, v.y, v.z, 1.0)); }

	inline void SetBlendModeAlpha() {
		SetBlendMode(OPblendFunction::SRC_ALPHA, OPblendFunction::ONE_MINUS_SRC_ALPHA);
	}

	inline void SetBlendModeAdditive() {
		SetBlendMode(OPblendFunction::SRC_ALPHA, OPblendFunction::ONE);
	}
};

#endif