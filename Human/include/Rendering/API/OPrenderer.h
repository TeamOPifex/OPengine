#pragma once

struct OPrenderAPI;
typedef struct OPrenderAPI OPrenderAPI;

#include "./Human/include/Rendering/API/OPcontext.h"
#include "./Human/include/Rendering/API/OPeffectAPI.h"
#include "./Human/include/Rendering/API/OPframeBufferAPI.h"
#include "./Human/include/Rendering/API/OPframeBufferDepthAPI.h"
#include "./Human/include/Rendering/API/OPindexBufferAPI.h"
#include "./Human/include/Rendering/API/OPmonitorAPI.h"
#include "./Human/include/Rendering/API/OPshaderAPI.h"
#include "./Human/include/Rendering/API/OPshaderAttributeAPI.h"
#include "./Human/include/Rendering/API/OPshaderResource.h"
#include "./Human/include/Rendering/API/OPshaderUniformAPI.h"
#include "./Human/include/Rendering/API/OPtextureAPI.h"
#include "./Human/include/Rendering/API/OPtextureCubeAPI.h"
#include "./Human/include/Rendering/API/OPvertexArrayAPI.h"
#include "./Human/include/Rendering/API/OPvertexBufferAPI.h"
#include "./Human/include/Rendering/API/OPwindowAPI.h"
#include "./Human/include/Rendering/Enums/OPblendFunction.h"
#include "./Human/include/Rendering/Enums/OPcullFace.h"
#include "./Math/include/OPvec4.h"

struct OPrenderAPI {
	i8(*Init)(OPwindow* window) = 0;
	void(*_Clear)(OPvec4 color) = 0;
	void(*_ClearColor)(OPvec4 color) = 0;
	void(*ClearDepth)() = 0;
	void(*Present)() = 0;
	void(*SetDepthTesting)(bool state) = 0;
	void(*SetDepthWrite)(bool state) = 0;
	void(*SetCull)(bool state) = 0;
	void(*SetCullMode)(OPcullFace state) = 0;
	void(*SetWireframe)(bool state) = 0;
	void(*SetBlend)(bool state) = 0;
	void(*SetBlendMode)(OPblendFunction src, OPblendFunction dst) = 0;
	void(*SetViewport)(ui32 x, ui32 y, ui32 width, ui32 height) = 0;
	void(*SwapBuffer)() = 0;
	void(*Shutdown)() = 0;

	OPcontext Context;
	OPeffectAPI Effect;
	OPframeBufferAPI FrameBuffer;
	OPframeBufferDepthAPI FrameBufferDepth;
	OPindexBufferAPI IndexBuffer;
	OPmonitorAPI Monitor;
	OPshaderAPI Shader;
	OPshaderAttributeAPI ShaderAttribute;
	OPshaderResource ShaderResource;
	OPshaderUniformAPI ShaderUniform;
	OPtextureAPI Texture;
	OPtextureCubeAPI TextureCube;
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

	inline void ClearColor(OPvec4 v) { _ClearColor(v); }
	inline void ClearColor(OPfloat r) { _ClearColor(OPvec4(r)); }
	inline void ClearColor(OPfloat r, OPfloat g, OPfloat b) { _ClearColor(OPvec4(r, g, b, 1.0)); }
	inline void ClearColor(OPfloat r, OPfloat g, OPfloat b, OPfloat a) { _ClearColor(OPvec4(r, g, b, a)); }
	inline void ClearColor(OPvec3 v) { _ClearColor(OPvec4(v.x, v.y, v.z, 1.0)); }

	inline void SetBlendModeAlpha() {
		SetBlendMode(OPblendFunction::SRC_ALPHA, OPblendFunction::ONE_MINUS_SRC_ALPHA);
	}

	inline void SetBlendModeAdditive() {
		SetBlendMode(OPblendFunction::SRC_ALPHA, OPblendFunction::ONE);
	}
};