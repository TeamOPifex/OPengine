#pragma once

struct OPshaderUniformAPI;
typedef struct OPshaderUniformAPI OPshaderUniformAPI;

#include "./Math/include/OPmat3.h"
#include "./Math/include/OPmat4.h"

struct OPshaderUniform;
struct OPeffect;
struct OPtexture;
struct OPtextureCube;

struct OPshaderUniformAPI {
	OPshaderUniform*(*Create)(OPeffect* effect, const OPchar* name) = 0;
	OPshaderUniform*(*Init)(OPshaderUniform* shaderUniform, OPeffect* effect, const OPchar* name) = 0;
	void(*Destroy)(OPshaderUniform* shaderUniform) = 0;

	void(*SetBool)(OPshaderUniform* shaderUniform, bool val);
	void(*SetF)(OPshaderUniform* shaderUniform, f32 val);
	void(*SetFv)(OPshaderUniform* shaderUniform, ui32 count, f32* val);
	void(*SetI)(OPshaderUniform* shaderUniform, i32 val);
	void(*SetIv)(OPshaderUniform* shaderUniform, ui32 count, i32* val);
	void(*SetVec2)(OPshaderUniform* shaderUniform, OPvec2* val);
	void(*SetVec2v)(OPshaderUniform* shaderUniform, ui32 count, OPvec2* val);
	void(*SetVec3)(OPshaderUniform* shaderUniform, OPvec3* val);
	void(*SetVec3v)(OPshaderUniform* shaderUniform, ui32 count, OPvec3* val);
	void(*SetVec4)(OPshaderUniform* shaderUniform, OPvec4* val);
	void(*SetVec4v)(OPshaderUniform* shaderUniform, ui32 count, OPvec4* val);
	void(*SetMat3)(OPshaderUniform* shaderUniform, OPmat3* val);
	void(*SetMat3v)(OPshaderUniform* shaderUniform, ui32 count, OPmat3* val);
	void(*SetMat4)(OPshaderUniform* shaderUniform, OPmat4* val);
	void(*SetMat4v)(OPshaderUniform* shaderUniform, ui32 count, OPmat4* val);
	void(*SetTexture)(OPshaderUniform* shaderUniform, OPtexture* val, ui32 slot);
	void(*SetTexturev)(OPshaderUniform* shaderUniform, ui32 count, OPtexture* val, ui32 slot);
	void(*SetTextureCube)(OPshaderUniform* shaderUniform, OPtextureCube* val, ui32 slot);
	void(*SetTextureCubev)(OPshaderUniform* shaderUniform, ui32 count, OPtextureCube* val, ui32 slot);

	inline void Set(OPshaderUniform* shaderUniform, bool val) { SetBool(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, f32 val) { SetF(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, f32* val) { SetFv(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, i32 val) { SetI(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, i32* val) { SetIv(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPvec2* val) { SetVec2(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPvec2* val) { SetVec2v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPvec3* val) { SetVec3(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPvec3* val) { SetVec3v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPvec4* val) { SetVec4(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPvec4* val) { SetVec4v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPmat3* val) { SetMat3(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPmat3* val) { SetMat3v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPmat4* val) { SetMat4(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPmat4* val) { SetMat4v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPtexture* val, ui32 slot) { SetTexture(shaderUniform, val, slot); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPtexture* val, ui32 slot) { SetTexturev(shaderUniform, count, val, slot); }
	inline void Set(OPshaderUniform* shaderUniform, OPtextureCube* val, ui32 slot) { SetTextureCube(shaderUniform, val, slot); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPtextureCube* val, ui32 slot) { SetTextureCubev(shaderUniform, count, val, slot); }
};