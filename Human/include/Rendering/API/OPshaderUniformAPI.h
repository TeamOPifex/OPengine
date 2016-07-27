#ifndef OPENGINE_HUMAN_RENDERING_SHADER_UNIFORM_API
#define OPENGINE_HUMAN_RENDERING_SHADER_UNIFORM_API

#include "./Core/include/OPtypes.h"

struct OPshaderUniformAPI;
typedef struct OPshaderUniformAPI OPshaderUniformAPI;

struct OPshaderUniform;

#include "./Math/include/OPmat3.h"
#include "./Math/include/OPmat4.h"

struct OPeffect;
struct OPtexture;
struct OPtextureCube;

struct OPshaderUniformAPI {
	OPshaderUniform*(*Create)(OPeffect* effect, const OPchar* name) = 0;
	OPshaderUniform*(*Init)(OPshaderUniform* shaderUniform, OPeffect* effect, const OPchar* name) = 0;

	void(*SetF)(OPshaderUniform* shaderUniform, f32 val);
	void(*SetFv)(OPshaderUniform* shaderUniform, OPuint count, f32* val);
	void(*SetI)(OPshaderUniform* shaderUniform, i32 val);
	void(*SetIv)(OPshaderUniform* shaderUniform, OPuint count, i32* val);
	void(*SetVec2)(OPshaderUniform* shaderUniform, OPvec2* val);
	void(*SetVec2v)(OPshaderUniform* shaderUniform, OPuint count, OPvec2* val);
	void(*SetVec3)(OPshaderUniform* shaderUniform, OPvec3* val);
	void(*SetVec3v)(OPshaderUniform* shaderUniform, OPuint count, OPvec3* val);
	void(*SetVec4)(OPshaderUniform* shaderUniform, OPvec4* val);
	void(*SetVec4v)(OPshaderUniform* shaderUniform, OPuint count, OPvec4* val);
	void(*SetMat3)(OPshaderUniform* shaderUniform, OPmat3* val);
	void(*SetMat3v)(OPshaderUniform* shaderUniform, OPuint count, OPmat3* val);
	void(*SetMat4)(OPshaderUniform* shaderUniform, OPmat4* val);
	void(*SetMat4v)(OPshaderUniform* shaderUniform, OPuint count, OPmat4* val);
	void(*SetTexture)(OPshaderUniform* shaderUniform, OPtexture* val);
	void(*SetTexturev)(OPshaderUniform* shaderUniform, OPuint count, OPtexture* val);
	void(*SetTextureCube)(OPshaderUniform* shaderUniform, OPtextureCube* val);
	void(*SetTextureCubev)(OPshaderUniform* shaderUniform, OPuint count, OPtextureCube* val);

	inline void Set(OPshaderUniform* shaderUniform, f32 val) { SetF(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPuint count, f32* val) { SetFv(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, i32 val) { SetI(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPuint count, i32* val) { SetIv(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPvec2* val) { SetVec2(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPuint count, OPvec2* val) { SetVec2v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPvec3* val) { SetVec3(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPuint count, OPvec3* val) { SetVec3v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPvec4* val) { SetVec4(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPuint count, OPvec4* val) { SetVec4v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPmat3* val) { SetMat3(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPuint count, OPmat3* val) { SetMat3v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPmat4* val) { SetMat4(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPuint count, OPmat4* val) { SetMat4v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPtexture* val) { SetTexture(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPuint count, OPtexture* val) { SetTexturev(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPtextureCube* val) { SetTextureCube(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPuint count, OPtextureCube* val) { SetTextureCubev(shaderUniform, count, val); }
};

#endif