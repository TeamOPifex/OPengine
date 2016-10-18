#pragma once

struct OPeffect;
typedef struct OPeffect OPeffect;

#include "./Human/include/Rendering/OPshader.h"
#include "./Human/include/Rendering/Enums/OPattributes.h"
#include "./Human/include/Rendering/OPvertexLayout.h"
#include "./Human/include/Rendering/OPrenderBuffer.h"
#include "./Human/include/Rendering/OPtexture.h"
#include "./Human/include/Rendering/OPcam.h"
#include "./Human/include/Rendering/OPtextureCube.h"
#include "./Math/include/Vectors.h"
#include "./Math/include/OPmat4.h"
#include "./Data/include/OPhashMap.h"
#include "./Data/include/OPlist.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPcman.h"
#include "./Core/include/OPtypes.h"
#include "./Core/include/OPlog.h"
#include "./Core/include/OPmemory.h"

struct OPeffect {
	void* internalPtr;
	OPshader* vertexShader;
	OPshader* fragmentShader;
	OPhashMap uniforms;

	OPeffect() {}
	OPeffect(OPshader* vert, OPshader* frag) { Init(vert, frag, NULL); }
	OPeffect(const OPchar* vert, const OPchar* frag) { Init(vert, frag, NULL); }
	OPeffect(OPshader* vert, OPshader* frag, OPvertexLayout* vertexLayout) { Init(vert, frag, vertexLayout); }
	OPeffect(const OPchar* vert, const OPchar* frag, OPvertexLayout* vertexLayout) { Init(vert, frag, vertexLayout); }

	void Init(OPshader* vert, OPshader* frag, OPvertexLayout* vertexLayout);
	inline void Init(OPshader* vert, OPshader* frag) {
		Init(vert, frag, NULL);
	}
	inline void Init(const OPchar* vert, const OPchar* frag, OPvertexLayout* vertexLayout) {
		Init((OPshader*)OPCMAN.LoadGet(vert), (OPshader*)OPCMAN.LoadGet(frag), vertexLayout);
	}
	inline void Init(const OPchar* vert, const OPchar* frag) {
		Init(vert, frag, NULL);
	}

	inline void Bind() { 
		OPRENDERER_ACTIVE->Effect.Bind(this); 
	}
	inline void Unbind() { 
		OPRENDERER_ACTIVE->Effect.Unbind(this); 
	}
	inline void Destroy() {
		OPRENDERER_ACTIVE->Effect.Destroy(this);
	}

	inline bool AddUniform(const OPchar* name) { return OPRENDERER_ACTIVE->Effect.AddUniform(this, name); }

	inline void Set(OPshaderUniform* shaderUniform, f32 val) { OPRENDERER_ACTIVE->ShaderUniform.SetF(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, f32* val) { OPRENDERER_ACTIVE->ShaderUniform.SetFv(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, bool val) { OPRENDERER_ACTIVE->ShaderUniform.SetBool(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, i32 val) { OPRENDERER_ACTIVE->ShaderUniform.SetI(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, i32* val) { OPRENDERER_ACTIVE->ShaderUniform.SetIv(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPvec2* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec2(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPvec2* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec2v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPvec3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec3(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPvec3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec3v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPvec4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec4(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPvec4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec4v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPmat3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat3(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPmat3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat3v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPmat4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat4(shaderUniform, val); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPmat4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat4v(shaderUniform, count, val); }
	inline void Set(OPshaderUniform* shaderUniform, OPtexture* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTexture(shaderUniform, val, slot); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPtexture* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTexturev(shaderUniform, count, val, slot); }
	inline void Set(OPshaderUniform* shaderUniform, OPtextureCube* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTextureCube(shaderUniform, val, slot); }
	inline void Set(OPshaderUniform* shaderUniform, ui32 count, OPtextureCube* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTextureCubev(shaderUniform, count, val, slot); }

	OPshaderUniform* GetUniform(const OPchar* name);
	inline void Set(const OPchar* name, f32 val) { OPRENDERER_ACTIVE->ShaderUniform.SetF(GetUniform(name), val); }
	inline void Set(const OPchar* name, ui32 count, f32* val) { OPRENDERER_ACTIVE->ShaderUniform.SetFv(GetUniform(name), count, val); }
	inline void Set(const OPchar* name, bool val) { OPRENDERER_ACTIVE->ShaderUniform.SetF(GetUniform(name), val); }
	inline void Set(const OPchar* name, i32 val) { OPRENDERER_ACTIVE->ShaderUniform.SetI(GetUniform(name), val); }
	inline void Set(const OPchar* name, ui32 count, i32* val) { OPRENDERER_ACTIVE->ShaderUniform.SetIv(GetUniform(name), count, val); }
	inline void Set(const OPchar* name, OPvec2* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec2(GetUniform(name), val); }
	inline void Set(const OPchar* name, ui32 count, OPvec2* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec2v(GetUniform(name), count, val); }
	inline void Set(const OPchar* name, OPvec3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec3(GetUniform(name), val); }
	inline void Set(const OPchar* name, ui32 count, OPvec3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec3v(GetUniform(name), count, val); }
	inline void Set(const OPchar* name, OPvec4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec4(GetUniform(name), val); }
	inline void Set(const OPchar* name, ui32 count, OPvec4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec4v(GetUniform(name), count, val); }
	inline void Set(const OPchar* name, OPmat3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat3(GetUniform(name), val); }
	inline void Set(const OPchar* name, ui32 count, OPmat3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat3v(GetUniform(name), count, val); }
	inline void Set(const OPchar* name, OPmat4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat4(GetUniform(name), val); }
	inline void Set(const OPchar* name, ui32 count, OPmat4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat4v(GetUniform(name), count, val); }
	inline void Set(const OPchar* name, OPtexture* val, ui32 slot) { 
		OPshaderUniform* uniform = GetUniform(name);
		if (uniform == NULL) {
			//OPlogErr("Shader Uniform %s not found", name);
			return;
		}
		OPRENDERER_ACTIVE->ShaderUniform.SetTexture(uniform, val, slot);
	}
	inline void Set(const OPchar* name, ui32 count, OPtexture* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTexturev(GetUniform(name), count, val, slot); }
	inline void Set(const OPchar* name, OPtextureCube* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTextureCube(GetUniform(name), val, slot); }
	inline void Set(const OPchar* name, ui32 count, OPtextureCube* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTextureCubev(GetUniform(name), count, val, slot); }

	void UpdateVertexLayout(OPvertexLayout* vertexLayout);

	inline static OPeffect* Create(OPshader* vert, OPshader* frag) {
		OPeffect* effect = (OPeffect*)OPalloc(sizeof(OPeffect));
		effect->Init(vert, frag);
		return effect;
	}
};


inline void OPeffectSet(const OPchar* name, bool val) { OPRENDERER_ACTIVE->ShaderUniform.SetBool(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val); }
inline void OPeffectSet(const OPchar* name, f32 val) { OPRENDERER_ACTIVE->ShaderUniform.SetF(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val); }
inline void OPeffectSet(const OPchar* name, ui32 count, f32* val) { OPRENDERER_ACTIVE->ShaderUniform.SetFv(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val); }
inline void OPeffectSet(const OPchar* name, i32 val) { OPRENDERER_ACTIVE->ShaderUniform.SetI(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val); }
inline void OPeffectSet(const OPchar* name, ui32 count, i32* val) { OPRENDERER_ACTIVE->ShaderUniform.SetIv(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val); }
inline void OPeffectSet(const OPchar* name, OPvec2* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec2(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val); }
inline void OPeffectSet(const OPchar* name, ui32 count, OPvec2* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec2v(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val); }
inline void OPeffectSet(const OPchar* name, OPvec3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec3(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val); }
inline void OPeffectSet(const OPchar* name, ui32 count, OPvec3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec3v(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val); }
inline void OPeffectSet(const OPchar* name, OPvec4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec4(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val); }
inline void OPeffectSet(const OPchar* name, ui32 count, OPvec4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetVec4v(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val); }
inline void OPeffectSet(const OPchar* name, OPmat3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat3(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val); }
inline void OPeffectSet(const OPchar* name, ui32 count, OPmat3* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat3v(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val); }
inline void OPeffectSet(const OPchar* name, OPmat4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat4(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val); }
inline void OPeffectSet(const OPchar* name, ui32 count, OPmat4* val) { OPRENDERER_ACTIVE->ShaderUniform.SetMat4v(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val); }
inline void OPeffectSet(const OPchar* name, OPtexture* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTexture(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val, slot); }
inline void OPeffectSet(const OPchar* name, ui32 count, OPtexture* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTexturev(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val, slot); }
inline void OPeffectSet(const OPchar* name, OPtextureCube* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTextureCube(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), val, slot); }
inline void OPeffectSet(const OPchar* name, ui32 count, OPtextureCube* val, ui32 slot) { OPRENDERER_ACTIVE->ShaderUniform.SetTextureCubev(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform(name), count, val, slot); }
inline void OPeffectSet(OPcam* val) {
	OPRENDERER_ACTIVE->ShaderUniform.SetMat4(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform("uView"), &val->view);
	OPRENDERER_ACTIVE->ShaderUniform.SetMat4(OPRENDERER_ACTIVE->OPEFFECT_ACTIVE->GetUniform("uProj"), &val->proj);
}